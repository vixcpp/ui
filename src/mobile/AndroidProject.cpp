/**
 *
 *  @file AndroidProject.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Vix.cpp
 *
 */
#include <vix/ui/mobile/AndroidProject.hpp>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

#if defined(__unix__) || defined(__APPLE__)
#include <cerrno>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] std::string safe_project_name(const std::string &name)
    {
      if (name.empty())
      {
        return "VixMobile";
      }

      std::string result;

      for (char character : name)
      {
        const unsigned char value = static_cast<unsigned char>(character);

        if (std::isalnum(value))
        {
          result.push_back(character);
        }
        else if (character == ' ' || character == '-' || character == '_')
        {
          result.push_back('_');
        }
      }

      if (result.empty())
      {
        return "VixMobile";
      }

      if (std::isdigit(static_cast<unsigned char>(result.front())))
      {
        result.insert(result.begin(), 'V');
      }

      return result;
    }

    [[nodiscard]] std::string gradle_string(std::string_view value)
    {
      std::string result;
      result.reserve(value.size());

      for (char character : value)
      {
        if (character == '\\' || character == '\'')
        {
          result.push_back('\\');
        }

        result.push_back(character);
      }

      return result;
    }

    [[nodiscard]] std::string xml_escape(std::string_view value)
    {
      std::string result;

      for (char character : value)
      {
        switch (character)
        {
        case '&':
          result += "&amp;";
          break;
        case '<':
          result += "&lt;";
          break;
        case '>':
          result += "&gt;";
          break;
        case '"':
          result += "&quot;";
          break;
        case '\'':
          result += "&apos;";
          break;
        default:
          result.push_back(character);
          break;
        }
      }

      return result;
    }

    [[nodiscard]] std::string java_string(std::string_view value)
    {
      std::string result;
      result.reserve(value.size());

      for (char character : value)
      {
        switch (character)
        {
        case '\\':
          result += "\\\\";
          break;
        case '"':
          result += "\\\"";
          break;
        case '\n':
          result += "\\n";
          break;
        case '\r':
          result += "\\r";
          break;
        default:
          result.push_back(character);
          break;
        }
      }

      return result;
    }

    [[nodiscard]] bool valid_package_part(std::string_view part)
    {
      if (part.empty())
      {
        return false;
      }

      const unsigned char first = static_cast<unsigned char>(part.front());
      if (!std::isalpha(first) && part.front() != '_')
      {
        return false;
      }

      for (char character : part)
      {
        const unsigned char value = static_cast<unsigned char>(character);
        if (!std::isalnum(value) && character != '_')
        {
          return false;
        }
      }

      return true;
    }

    [[nodiscard]] bool valid_package_name(std::string_view package_name)
    {
      std::size_t start = 0;
      int parts = 0;

      while (start < package_name.size())
      {
        const std::size_t dot = package_name.find('.', start);
        const std::string_view part =
            dot == std::string_view::npos
                ? package_name.substr(start)
                : package_name.substr(start, dot - start);

        if (!valid_package_part(part))
        {
          return false;
        }

        ++parts;

        if (dot == std::string_view::npos)
        {
          break;
        }

        start = dot + 1;
      }

      return parts >= 2;
    }

    [[nodiscard]] std::filesystem::path java_package_directory(
        const std::filesystem::path &root,
        std::string_view package_name)
    {
      std::filesystem::path result = root;
      std::size_t start = 0;

      while (start < package_name.size())
      {
        const std::size_t dot = package_name.find('.', start);
        const std::string_view part =
            dot == std::string_view::npos
                ? package_name.substr(start)
                : package_name.substr(start, dot - start);
        result /= std::string(part);

        if (dot == std::string_view::npos)
        {
          break;
        }

        start = dot + 1;
      }

      return result;
    }

    [[nodiscard]] bool uses_cleartext(std::string_view url)
    {
      return url.rfind("http://", 0) == 0;
    }

    [[nodiscard]] std::filesystem::path existing_directory(
        const char *value)
    {
      if (value == nullptr || *value == '\0')
      {
        return {};
      }

      const std::filesystem::path path(value);
      std::error_code error;
      if (std::filesystem::is_directory(path, error) && !error)
      {
        return path;
      }

      return {};
    }

    [[nodiscard]] std::filesystem::path detect_android_sdk_directory()
    {
      if (const std::filesystem::path sdk =
              existing_directory(std::getenv("ANDROID_HOME"));
          !sdk.empty())
      {
        return sdk;
      }

      if (const std::filesystem::path sdk =
              existing_directory(std::getenv("ANDROID_SDK_ROOT"));
          !sdk.empty())
      {
        return sdk;
      }

      const char *home = std::getenv("HOME");
      if (home == nullptr || *home == '\0')
      {
        return {};
      }

      const std::filesystem::path candidate =
          std::filesystem::path(home) / "Android" / "Sdk";
      std::error_code error;
      if (std::filesystem::is_directory(candidate, error) && !error)
      {
        return candidate;
      }

      return {};
    }

    [[nodiscard]] std::string local_properties_path(
        const std::filesystem::path &path)
    {
      std::string result;

      for (char character : path.string())
      {
        if (character == '\\')
        {
          result += "\\\\";
        }
        else if (character == ':')
        {
          result += "\\:";
        }
        else
        {
          result.push_back(character);
        }
      }

      return result;
    }

    [[nodiscard]] std::string render_settings_gradle(const AndroidProject &project)
    {
      std::ostringstream output;

      output
          << "pluginManagement {\n"
          << "    repositories {\n"
          << "        google()\n"
          << "        mavenCentral()\n"
          << "        gradlePluginPortal()\n"
          << "    }\n"
          << "}\n\n"
          << "dependencyResolutionManagement {\n"
          << "    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)\n"
          << "    repositories {\n"
          << "        google()\n"
          << "        mavenCentral()\n"
          << "    }\n"
          << "}\n\n"
          << "rootProject.name = '" << safe_project_name(project.config().name())
          << "'\n"
          << "include ':app'\n";

      return output.str();
    }

    [[nodiscard]] std::string render_root_build_gradle(
        const AndroidProject &project)
    {
      return "plugins {\n"
             "    id 'com.android.application' version '" +
             gradle_string(project.android_gradle_plugin_version()) +
             "' apply false\n"
             "}\n";
    }

    [[nodiscard]] std::string render_gradle_properties()
    {
      return "org.gradle.jvmargs=-Xmx2048m -Dfile.encoding=UTF-8\n"
             "android.useAndroidX=false\n";
    }

    [[nodiscard]] std::string render_app_build_gradle(
        const AndroidProject &project)
    {
      const MobileConfig &config = project.config();
      std::ostringstream output;

      output
          << "plugins {\n"
          << "    id 'com.android.application'\n"
          << "}\n\n"
          << "android {\n"
          << "    namespace '" << gradle_string(config.app_id()) << "'\n"
          << "    compileSdk " << project.compile_sdk() << "\n\n"
          << "    defaultConfig {\n"
          << "        applicationId '" << gradle_string(config.app_id()) << "'\n"
          << "        minSdk " << project.min_sdk() << "\n"
          << "        targetSdk " << project.target_sdk() << "\n"
          << "        versionCode " << project.version_code() << "\n"
          << "        versionName '" << gradle_string(config.version()) << "'\n"
          << "    }\n"
          << "}\n";

      return output.str();
    }

    [[nodiscard]] std::string render_manifest(const AndroidProject &project)
    {
      std::ostringstream output;

      output
          << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
          << "<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\">\n"
          << "    <uses-permission android:name=\"android.permission.INTERNET\" />\n\n"
          << "    <application\n"
          << "        android:allowBackup=\"true\"\n"
          << "        android:icon=\"@mipmap/ic_launcher\"\n"
          << "        android:label=\"@string/app_name\"\n"
          << "        android:supportsRtl=\"true\"\n"
          << "        android:theme=\"@style/AppTheme\"";

      if (uses_cleartext(project.config().url()))
      {
        output << "\n        android:usesCleartextTraffic=\"true\"";
      }

      output
          << ">\n"
          << "        <activity\n"
          << "            android:name=\".MainActivity\"\n"
          << "            android:exported=\"true\">\n"
          << "            <intent-filter>\n"
          << "                <action android:name=\"android.intent.action.MAIN\" />\n"
          << "                <category android:name=\"android.intent.category.LAUNCHER\" />\n"
          << "            </intent-filter>\n"
          << "        </activity>\n"
          << "    </application>\n"
          << "</manifest>\n";

      return output.str();
    }

    [[nodiscard]] std::string render_main_activity(
        const AndroidProject &project)
    {
      const MobileConfig &config = project.config();
      std::ostringstream output;

      output
          << "package " << config.app_id() << ";\n\n"
          << "import android.annotation.SuppressLint;\n"
          << "import android.app.DownloadManager;\n"
          << "import android.content.ActivityNotFoundException;\n"
          << "import android.content.Intent;\n"
          << "import android.net.Uri;\n"
          << "import android.app.Activity;\n"
          << "import android.os.Bundle;\n"
          << "import android.webkit.CookieManager;\n"
          << "import android.webkit.DownloadListener;\n"
          << "import android.webkit.ValueCallback;\n"
          << "import android.webkit.WebChromeClient;\n"
          << "import android.webkit.WebResourceError;\n"
          << "import android.webkit.WebResourceRequest;\n"
          << "import android.webkit.WebResourceResponse;\n"
          << "import android.webkit.WebSettings;\n"
          << "import android.webkit.WebView;\n"
          << "import android.webkit.WebViewClient;\n"
          << "import android.webkit.URLUtil;\n"
          << "import android.widget.Toast;\n\n"
          << "public class MainActivity extends Activity {\n"
          << "    private static final String APP_URL = \""
          << java_string(config.url()) << "\";\n"
          << "    private static final String RETRY_URL = \"vix://retry\";\n"
          << "    private static final int FILE_CHOOSER_REQUEST_CODE = 1001;\n"
          << "    private static final String ERROR_PAGE = \"<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Connection error</title></head><body><h1>Unable to load the application</h1><p>Please check your connection and try again.</p><button onclick=\\\"window.location.href='vix://retry'\\\">Retry</button></body></html>\";\n\n"
          << "    private WebView webView;\n"
          << "    private ValueCallback<Uri[]> fileChooserCallback;\n\n"
          << "    @SuppressLint(\"SetJavaScriptEnabled\")\n"
          << "    @Override\n"
          << "    protected void onCreate(Bundle savedInstanceState) {\n"
          << "        super.onCreate(savedInstanceState);\n\n"
          << "        webView = new WebView(this);\n"
          << "        setContentView(webView);\n\n"
          << "        WebSettings settings = webView.getSettings();\n"
          << "        settings.setJavaScriptEnabled(true);\n"
          << "        settings.setDomStorageEnabled(true);\n"
          << "        settings.setLoadWithOverviewMode(true);\n"
          << "        settings.setUseWideViewPort(true);\n"
          << "        settings.setAllowFileAccess(false);\n"
          << "        settings.setAllowContentAccess(false);\n\n"
          << "        webView.setWebChromeClient(new WebChromeClient() {\n"
          << "            @Override\n"
          << "            public boolean onShowFileChooser(\n"
          << "                    WebView view,\n"
          << "                    ValueCallback<Uri[]> filePathCallback,\n"
          << "                    WebChromeClient.FileChooserParams fileChooserParams) {\n"
          << "                if (fileChooserCallback != null) {\n"
          << "                    fileChooserCallback.onReceiveValue(null);\n"
          << "                }\n\n"
          << "                fileChooserCallback = filePathCallback;\n"
          << "                Intent chooserIntent;\n"
          << "                try {\n"
          << "                    chooserIntent = fileChooserParams.createIntent();\n"
          << "                    chooserIntent.addCategory(Intent.CATEGORY_OPENABLE);\n"
          << "                    chooserIntent.putExtra(\n"
          << "                            Intent.EXTRA_ALLOW_MULTIPLE,\n"
          << "                            fileChooserParams.getMode()\n"
          << "                                    == WebChromeClient.FileChooserParams.MODE_OPEN_MULTIPLE);\n"
          << "                    startActivityForResult(chooserIntent, FILE_CHOOSER_REQUEST_CODE);\n"
          << "                    return true;\n"
          << "                } catch (ActivityNotFoundException exception) {\n"
          << "                    fileChooserCallback.onReceiveValue(null);\n"
          << "                    fileChooserCallback = null;\n"
          << "                    Toast.makeText(MainActivity.this, \"No file picker is available.\", Toast.LENGTH_SHORT).show();\n"
          << "                    return false;\n"
          << "                }\n"
          << "            }\n"
          << "        });\n\n"
          << "        webView.setDownloadListener(new DownloadListener() {\n"
          << "            @Override\n"
          << "            public void onDownloadStart(\n"
          << "                    String url,\n"
          << "                    String userAgent,\n"
          << "                    String contentDisposition,\n"
          << "                    String mimeType,\n"
          << "                    long contentLength) {\n"
          << "                enqueueDownload(url, userAgent, contentDisposition, mimeType);\n"
          << "            }\n"
          << "        });\n\n"
          << "        webView.setWebViewClient(new WebViewClient() {\n"
          << "            @Override\n"
          << "            public boolean shouldOverrideUrlLoading(\n"
          << "                    WebView view,\n"
          << "                    WebResourceRequest request) {\n"
          << "                return handleNavigation(request.getUrl());\n"
          << "            }\n\n"
          << "            @Override\n"
          << "            public boolean shouldOverrideUrlLoading(WebView view, String url) {\n"
          << "                return handleNavigation(Uri.parse(url));\n"
          << "            }\n\n"
          << "            @Override\n"
          << "            public void onReceivedError(\n"
          << "                    WebView view,\n"
          << "                    WebResourceRequest request,\n"
          << "                    WebResourceError error) {\n"
          << "                if (request.isForMainFrame()) {\n"
          << "                    showErrorPage();\n"
          << "                }\n"
          << "            }\n\n"
          << "            @Override\n"
          << "            public void onReceivedHttpError(\n"
          << "                    WebView view,\n"
          << "                    WebResourceRequest request,\n"
          << "                    WebResourceResponse errorResponse) {\n"
          << "                if (request.isForMainFrame() && errorResponse.getStatusCode() >= 400) {\n"
          << "                    showErrorPage();\n"
          << "                }\n"
          << "            }\n"
          << "        });\n\n"
          << "        boolean restored = savedInstanceState != null\n"
          << "                && webView.restoreState(savedInstanceState) != null;\n"
          << "        if (!restored) {\n"
          << "            loadInitialUrl();\n"
          << "        }\n"
          << "    }\n\n"
          << "    private boolean handleNavigation(Uri uri) {\n"
          << "        if (RETRY_URL.equalsIgnoreCase(uri.toString())) {\n"
          << "            loadInitialUrl();\n"
          << "            return true;\n"
          << "        }\n\n"
          << "        String scheme = uri.getScheme();\n"
          << "        if (isHttpOrHttps(uri)) {\n"
          << "            if (isInternalUri(uri)) {\n"
          << "                return false;\n"
          << "            }\n\n"
          << "            openExternalUri(uri);\n"
          << "            return true;\n"
          << "        }\n\n"
          << "        if (\"mailto\".equalsIgnoreCase(scheme) || \"tel\".equalsIgnoreCase(scheme)) {\n"
          << "            openExternalUri(uri);\n"
          << "            return true;\n"
          << "        }\n\n"
          << "        openExternalUri(uri);\n"
          << "        return true;\n"
          << "    }\n\n"
          << "    private boolean isHttpOrHttps(Uri uri) {\n"
          << "        String scheme = uri.getScheme();\n"
          << "        return \"http\".equalsIgnoreCase(scheme)\n"
          << "                || \"https\".equalsIgnoreCase(scheme);\n"
          << "    }\n\n"
          << "    private boolean isInternalUri(Uri uri) {\n"
          << "        String initialHost = Uri.parse(APP_URL).getHost();\n"
          << "        String host = uri.getHost();\n"
          << "        return initialHost != null && host != null\n"
          << "                && initialHost.equalsIgnoreCase(host);\n"
          << "    }\n\n"
          << "    private void openExternalUri(Uri uri) {\n"
          << "        Intent intent = new Intent(Intent.ACTION_VIEW, uri);\n"
          << "        intent.addCategory(Intent.CATEGORY_BROWSABLE);\n"
          << "        if (intent.resolveActivity(getPackageManager()) == null) {\n"
          << "            Toast.makeText(this, \"No application can open this link.\", Toast.LENGTH_SHORT).show();\n"
          << "            return;\n"
          << "        }\n\n"
          << "        try {\n"
          << "            startActivity(intent);\n"
          << "        } catch (ActivityNotFoundException exception) {\n"
          << "            Toast.makeText(this, \"No application can open this link.\", Toast.LENGTH_SHORT).show();\n"
          << "        }\n"
          << "    }\n\n"
          << "    private void loadInitialUrl() {\n"
          << "        if (webView != null) {\n"
          << "            webView.loadUrl(APP_URL);\n"
          << "        }\n"
          << "    }\n\n"
          << "    private void enqueueDownload(\n"
          << "            String url,\n"
          << "            String userAgent,\n"
          << "            String contentDisposition,\n"
          << "            String mimeType) {\n"
          << "        if (url == null || url.isEmpty()) {\n"
          << "            Toast.makeText(this, \"Unable to download this file.\", Toast.LENGTH_SHORT).show();\n"
          << "            return;\n"
          << "        }\n\n"
          << "        try {\n"
          << "            Uri downloadUri = Uri.parse(url);\n"
          << "            if (!isHttpOrHttps(downloadUri)) {\n"
          << "                Toast.makeText(this, \"Unsupported download URL.\", Toast.LENGTH_SHORT).show();\n"
          << "                return;\n"
          << "            }\n\n"
          << "            DownloadManager.Request request = new DownloadManager.Request(downloadUri);\n"
          << "            String fileName = URLUtil.guessFileName(url, contentDisposition, mimeType);\n"
          << "            request.setTitle(fileName);\n"
          << "            request.setDescription(\"Downloading \" + fileName);\n"
          << "            if (mimeType != null && !mimeType.isEmpty()) {\n"
          << "                request.setMimeType(mimeType);\n"
          << "            }\n"
          << "            if (userAgent != null && !userAgent.isEmpty()) {\n"
          << "                request.addRequestHeader(\"User-Agent\", userAgent);\n"
          << "            }\n"
          << "            String cookie = CookieManager.getInstance().getCookie(url);\n"
          << "            if (cookie != null && !cookie.isEmpty()) {\n"
          << "                request.addRequestHeader(\"Cookie\", cookie);\n"
          << "            }\n"
          << "            request.setNotificationVisibility(\n"
          << "                    DownloadManager.Request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED);\n\n"
          << "            DownloadManager manager = (DownloadManager) getSystemService(DOWNLOAD_SERVICE);\n"
          << "            if (manager == null) {\n"
          << "                Toast.makeText(this, \"Downloads are unavailable on this device.\", Toast.LENGTH_SHORT).show();\n"
          << "                return;\n"
          << "            }\n\n"
          << "            manager.enqueue(request);\n"
          << "        } catch (RuntimeException exception) {\n"
          << "            Toast.makeText(this, \"Unable to start the download.\", Toast.LENGTH_SHORT).show();\n"
          << "        }\n"
          << "    }\n\n"
          << "    private void showErrorPage() {\n"
          << "        if (webView != null) {\n"
          << "            webView.loadDataWithBaseURL(null, ERROR_PAGE, \"text/html\", \"UTF-8\", null);\n"
          << "        }\n"
          << "    }\n\n"
          << "    @Override\n"
          << "    public void onBackPressed() {\n"
          << "        if (webView != null && webView.canGoBack()) {\n"
          << "            webView.goBack();\n"
          << "            return;\n"
          << "        }\n\n"
          << "        super.onBackPressed();\n"
          << "    }\n\n"
          << "    @Override\n"
          << "    protected void onPause() {\n"
          << "        if (webView != null) {\n"
          << "            webView.onPause();\n"
          << "        }\n\n"
          << "        super.onPause();\n"
          << "    }\n\n"
          << "    @Override\n"
          << "    protected void onResume() {\n"
          << "        super.onResume();\n\n"
          << "        if (webView != null) {\n"
          << "            webView.onResume();\n"
          << "        }\n"
          << "    }\n\n"
          << "    @Override\n"
          << "    protected void onSaveInstanceState(Bundle outState) {\n"
          << "        if (webView != null) {\n"
          << "            webView.saveState(outState);\n"
          << "        }\n\n"
          << "        super.onSaveInstanceState(outState);\n"
          << "    }\n\n"
          << "    @Override\n"
          << "    protected void onActivityResult(int requestCode, int resultCode, Intent data) {\n"
          << "        if (requestCode == FILE_CHOOSER_REQUEST_CODE) {\n"
          << "            if (fileChooserCallback != null) {\n"
          << "                Uri[] result = resultCode == RESULT_OK\n"
          << "                        ? WebChromeClient.FileChooserParams.parseResult(resultCode, data)\n"
          << "                        : null;\n"
          << "                fileChooserCallback.onReceiveValue(result);\n"
          << "                fileChooserCallback = null;\n"
          << "            }\n"
          << "            return;\n"
          << "        }\n\n"
          << "        super.onActivityResult(requestCode, resultCode, data);\n"
          << "    }\n\n"
          << "    @Override\n"
          << "    protected void onDestroy() {\n"
          << "        if (fileChooserCallback != null) {\n"
          << "            fileChooserCallback.onReceiveValue(null);\n"
          << "            fileChooserCallback = null;\n"
          << "        }\n\n"
          << "        if (webView != null) {\n"
          << "            webView.stopLoading();\n"
          << "            webView.loadUrl(\"about:blank\");\n"
          << "            webView.clearHistory();\n"
          << "            webView.removeAllViews();\n"
          << "            webView.destroy();\n"
          << "            webView = null;\n"
          << "        }\n\n"
          << "        super.onDestroy();\n"
          << "    }\n"
          << "}\n";

      return output.str();
    }

    [[nodiscard]] std::string render_strings_xml(
        const AndroidProject &project)
    {
      return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
             "<resources>\n"
             "    <string name=\"app_name\">" +
             xml_escape(project.config().name()) +
             "</string>\n"
             "</resources>\n";
    }

    [[nodiscard]] std::string render_colors_xml()
    {
      return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
             "<resources>\n"
             "    <color name=\"vix_accent\">#f37726</color>\n"
             "</resources>\n";
    }

    [[nodiscard]] std::string render_default_launcher_icon()
    {
      return "<vector xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
             "    android:width=\"48dp\"\n"
             "    android:height=\"48dp\"\n"
             "    android:viewportWidth=\"48\"\n"
             "    android:viewportHeight=\"48\">\n"
             "    <path\n"
             "        android:fillColor=\"#f37726\"\n"
             "        android:pathData=\"M0,0h48v48h-48z\" />\n"
             "    <path\n"
             "        android:fillColor=\"#ffffff\"\n"
             "        android:pathData=\"M14,12h20l-10,24z\" />\n"
             "</vector>\n";
    }

    [[nodiscard]] std::string render_styles_xml()
    {
      return "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
             "<resources>\n"
             "    <style name=\"AppTheme\" parent=\"android:style/Theme.Material.Light.NoActionBar\">\n"
             "        <item name=\"android:fontFamily\">sans</item>\n"
             "        <item name=\"android:windowLightStatusBar\">true</item>\n"
             "        <item name=\"android:colorAccent\">@color/vix_accent</item>\n"
             "    </style>\n"
             "</resources>\n";
    }

    [[nodiscard]] Result<void> write_text_file(
        const std::filesystem::path &path,
        const std::string &content)
    {
      std::error_code error;
      std::filesystem::create_directories(path.parent_path(), error);
      if (error)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot create Android project directory: " +
                path.parent_path().string() + ": " + error.message());
      }

      std::ofstream output(path, std::ios::binary | std::ios::trunc);

      if (!output.is_open())
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot write Android project file: " + path.string());
      }

      output << content;

      if (!output.good())
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "failed while writing Android project file: " + path.string());
      }

      return Result<void>::ok();
    }

    [[nodiscard]] Result<void> copy_icon_file(
        const std::filesystem::path &source,
        const std::filesystem::path &destination)
    {
      std::error_code error;
      std::filesystem::create_directories(destination.parent_path(), error);
      if (error)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot create Android icon directory: " +
                destination.parent_path().string() + ": " + error.message());
      }

      std::filesystem::copy_file(
          source,
          destination,
          std::filesystem::copy_options::overwrite_existing,
          error);
      if (error)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot copy Android launcher icon: " + source.string() +
                ": " + error.message());
      }

      return Result<void>::ok();
    }

    [[nodiscard]] bool is_supported_icon_path(
        const std::filesystem::path &path)
    {
      std::string extension = path.extension().string();
      for (char &character : extension)
      {
        character = static_cast<char>(
            std::tolower(static_cast<unsigned char>(character)));
      }

      return extension == ".png";
    }

    [[nodiscard]] bool is_regular_path(const std::filesystem::path &path)
    {
      std::error_code error;
      return std::filesystem::is_regular_file(path, error) && !error;
    }

    [[nodiscard]] std::filesystem::path find_command_on_path(
        const std::filesystem::path &command)
    {
      if (command.has_parent_path())
      {
        return is_regular_path(command) ? command : std::filesystem::path{};
      }

      const char *path_environment = std::getenv("PATH");
      if (path_environment == nullptr || *path_environment == '\0')
      {
        return {};
      }

#if defined(_WIN32)
      constexpr char separator = ';';
#else
      constexpr char separator = ':';
#endif

      const std::string value(path_environment);
      std::size_t start = 0;

      while (start <= value.size())
      {
        const std::size_t end = value.find(separator, start);
        const std::string entry = value.substr(start, end - start);
        const std::filesystem::path candidate =
            (entry.empty() ? std::filesystem::path{"."} :
                             std::filesystem::path{entry}) /
            command;

        if (is_regular_path(candidate))
        {
          return candidate;
        }

        if (end == std::string::npos)
        {
          break;
        }

        start = end + 1;
      }

      return {};
    }

    [[nodiscard]] Result<std::filesystem::path> select_gradle_command(
        const std::filesystem::path &directory,
        const std::filesystem::path &configured_command)
    {
#if defined(_WIN32)
      const std::filesystem::path wrapper = directory / "gradlew.bat";
#else
      const std::filesystem::path wrapper = directory / "gradlew";
#endif

      if (is_regular_path(wrapper))
      {
        return Result<std::filesystem::path>::ok(wrapper);
      }

      if (!configured_command.empty())
      {
        const std::filesystem::path command =
            find_command_on_path(configured_command);
        if (command.empty())
        {
          return Result<std::filesystem::path>::fail(
              ErrorCode::ConfigError,
              "configured Gradle command was not found: " +
                  configured_command.string());
        }

        return Result<std::filesystem::path>::ok(command);
      }

#if defined(_WIN32)
      const std::filesystem::path system_command{"gradle.bat"};
#else
      const std::filesystem::path system_command{"gradle"};
#endif
      const std::filesystem::path command = find_command_on_path(system_command);
      if (command.empty())
      {
        return Result<std::filesystem::path>::fail(
            ErrorCode::RuntimeError,
            "Gradle executable was not found; add a wrapper, configure Gradle, or add Gradle to PATH");
      }

      return Result<std::filesystem::path>::ok(command);
    }

    [[nodiscard]] Result<void> run_gradle_task(
        const std::filesystem::path &command,
        const std::filesystem::path &directory,
        const std::string &task)
    {
      std::error_code path_error;
      const std::filesystem::path executable_path =
          std::filesystem::absolute(command, path_error);
      if (path_error)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot resolve Gradle executable: " + command.string());
      }

#if defined(__unix__) || defined(__APPLE__)
      const pid_t child = fork();
      if (child < 0)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "failed to start Gradle process");
      }

      if (child == 0)
      {
        if (chdir(directory.c_str()) != 0)
        {
          _exit(127);
        }

        const std::string executable = executable_path.string();
        char *const arguments[]{
            const_cast<char *>(executable.c_str()),
            const_cast<char *>(task.c_str()),
            nullptr};
        execv(executable.c_str(), arguments);
        _exit(127);
      }

      int status = 0;
      if (waitpid(child, &status, 0) < 0)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "failed while waiting for Gradle");
      }

      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
      {
        const int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "Gradle task failed: " + task + " (exit code " +
                std::to_string(code) + ")");
      }

      return Result<void>::ok();
#elif defined(_WIN32)
      const std::string invocation =
          "cd /d \"" + directory.string() + "\" && \"" +
          executable_path.string() + "\" " + task;
      const int status = std::system(invocation.c_str());
      if (status != 0)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "Gradle task failed: " + task + " (exit code " +
                std::to_string(status) + ")");
      }

      return Result<void>::ok();
#else
      (void)executable_path;
      (void)directory;
      (void)task;
      return Result<void>::fail(
          ErrorCode::RuntimeError,
          "Android builds are not supported on this platform yet");
#endif
    }

    [[nodiscard]] Result<std::filesystem::path> resolve_artifact_path(
        const std::filesystem::path &directory,
        AndroidBuildType type,
        AndroidArtifact artifact)
    {
      const std::string variant =
          type == AndroidBuildType::Debug ? "debug" : "release";
      const std::filesystem::path output_directory =
          artifact == AndroidArtifact::Apk
              ? directory / "app" / "build" / "outputs" / "apk" / variant
              : directory / "app" / "build" / "outputs" / "bundle" / variant;
      const std::string extension =
          artifact == AndroidArtifact::Apk ? ".apk" : ".aab";

      std::error_code error;
      if (!std::filesystem::is_directory(output_directory, error) || error)
      {
        return Result<std::filesystem::path>::fail(
            ErrorCode::RuntimeError,
            "Gradle completed but did not create the expected artifact directory: " +
                output_directory.string());
      }

      std::vector<std::filesystem::path> artifacts;
      for (std::filesystem::recursive_directory_iterator iterator(
               output_directory,
               std::filesystem::directory_options::skip_permission_denied,
               error);
           !error && iterator != std::filesystem::recursive_directory_iterator();
           iterator.increment(error))
      {
        if (!iterator->is_regular_file(error) || error)
        {
          continue;
        }

        std::string candidate_extension = iterator->path().extension().string();
        for (char &character : candidate_extension)
        {
          character = static_cast<char>(
              std::tolower(static_cast<unsigned char>(character)));
        }

        if (candidate_extension == extension)
        {
          artifacts.push_back(iterator->path());
        }
      }

      if (error || artifacts.empty())
      {
        return Result<std::filesystem::path>::fail(
            ErrorCode::RuntimeError,
            "Gradle completed but did not produce an " + extension + " artifact");
      }

      std::sort(artifacts.begin(), artifacts.end());
      return Result<std::filesystem::path>::ok(std::move(artifacts.front()));
    }
  } // namespace

  AndroidProject::AndroidProject(MobileProject project)
      : MobileProject(std::move(project))
  {
  }

  AndroidProject &AndroidProject::set_min_sdk(int min_sdk) noexcept
  {
    min_sdk_ = min_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_target_sdk(int target_sdk) noexcept
  {
    target_sdk_ = target_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_compile_sdk(int compile_sdk) noexcept
  {
    compile_sdk_ = compile_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_version_code(int version_code) noexcept
  {
    version_code_ = version_code;
    return *this;
  }

  AndroidProject &AndroidProject::set_android_gradle_plugin_version(
      std::string version)
  {
    android_gradle_plugin_version_ = std::move(version);
    return *this;
  }

  AndroidProject &AndroidProject::set_gradle_command(
      std::filesystem::path command)
  {
    gradle_command_ = std::move(command);
    return *this;
  }

  int AndroidProject::min_sdk() const noexcept
  {
    return min_sdk_;
  }

  int AndroidProject::target_sdk() const noexcept
  {
    return target_sdk_;
  }

  int AndroidProject::compile_sdk() const noexcept
  {
    return compile_sdk_;
  }

  int AndroidProject::version_code() const noexcept
  {
    return version_code_;
  }

  const std::string &AndroidProject::android_gradle_plugin_version() const noexcept
  {
    return android_gradle_plugin_version_;
  }

  const std::filesystem::path &AndroidProject::gradle_command() const noexcept
  {
    return gradle_command_;
  }

  bool AndroidProject::has_gradle_command() const noexcept
  {
    return !gradle_command_.empty();
  }

  Result<void> AndroidProject::generate(
      const std::filesystem::path &directory) const
  {
    Result<void> validation = validate();
    if (validation.is_failed())
    {
      return validation;
    }

    if (directory.empty())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project output directory must not be empty");
    }

    const std::filesystem::path app_root = directory / "app";
    const std::filesystem::path main_root = app_root / "src" / "main";
    const std::filesystem::path mipmap_root = main_root / "res" / "mipmap";
    const std::filesystem::path package_root = java_package_directory(
        main_root / "java", config().app_id());

    const std::pair<std::filesystem::path, std::string> files[]{
        {directory / "settings.gradle", render_settings_gradle(*this)},
        {directory / "build.gradle", render_root_build_gradle(*this)},
        {directory / "gradle.properties", render_gradle_properties()},
        {app_root / "build.gradle", render_app_build_gradle(*this)},
        {main_root / "AndroidManifest.xml", render_manifest(*this)},
        {package_root / "MainActivity.java", render_main_activity(*this)},
        {main_root / "res" / "values" / "strings.xml", render_strings_xml(*this)},
        {main_root / "res" / "values" / "colors.xml", render_colors_xml()},
        {main_root / "res" / "values" / "styles.xml", render_styles_xml()}};

    for (const auto &[path, content] : files)
    {
      Result<void> write_result = write_text_file(path, content);
      if (write_result.is_failed())
      {
        return write_result;
      }
    }

    Result<void> icon_result = Result<void>::ok();
    if (config().has_icon_path())
    {
      icon_result = copy_icon_file(
          config().icon_path(),
          mipmap_root / "ic_launcher.png");
    }
    else
    {
      icon_result = write_text_file(
          mipmap_root / "ic_launcher.xml",
          render_default_launcher_icon());
    }

    if (icon_result.is_failed())
    {
      return icon_result;
    }

    const std::filesystem::path sdk_directory = detect_android_sdk_directory();
    if (!sdk_directory.empty())
    {
      Result<void> write_result = write_text_file(
          directory / "local.properties",
          "sdk.dir=" + local_properties_path(sdk_directory) + "\n");
      if (write_result.is_failed())
      {
        return write_result;
      }
    }

    return Result<void>::ok();
  }

  Result<std::filesystem::path> AndroidProject::build(
      const std::filesystem::path &directory,
      AndroidBuildType type,
      AndroidArtifact artifact) const
  {
    Result<void> validation = validate();
    if (validation.is_failed())
    {
      return Result<std::filesystem::path>::fail(
          validation.error_code(),
          validation.error_message());
    }

    if (directory.empty())
    {
      return Result<std::filesystem::path>::fail(
          ErrorCode::ConfigError,
          "Android project directory must not be empty");
    }

    if (!is_regular_path(directory / "settings.gradle") ||
        !is_regular_path(directory / "build.gradle") ||
        !is_regular_path(directory / "app" / "build.gradle"))
    {
      return Result<std::filesystem::path>::fail(
          ErrorCode::ConfigError,
          "Android project directory is missing required Gradle files");
    }

    if (artifact == AndroidArtifact::Aab && type != AndroidBuildType::Release)
    {
      return Result<std::filesystem::path>::fail(
          ErrorCode::ConfigError,
          "Android App Bundles are only supported for release builds");
    }

    Result<std::filesystem::path> command = select_gradle_command(
        directory,
        gradle_command_);
    if (command.is_failed())
    {
      return command;
    }

    const std::string task =
        artifact == AndroidArtifact::Aab
            ? "bundleRelease"
            : (type == AndroidBuildType::Debug ? "assembleDebug" :
                                                 "assembleRelease");
    Result<void> build_result = run_gradle_task(
        command.value(),
        directory,
        task);
    if (build_result.is_failed())
    {
      return Result<std::filesystem::path>::fail(
          build_result.error_code(),
          build_result.error_message());
    }

    return resolve_artifact_path(directory, type, artifact);
  }

  Result<void> AndroidProject::validate() const
  {
    Result<void> mobile_validation = MobileProject::validate();
    if (mobile_validation.is_failed())
    {
      return mobile_validation;
    }

    if (min_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project min SDK must be greater than zero");
    }

    if (target_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project target SDK must be greater than zero");
    }

    if (compile_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project compile SDK must be greater than zero");
    }

    if (version_code_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project version code must be greater than zero");
    }

    if (min_sdk_ > target_sdk_)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project min SDK must not exceed target SDK");
    }

    if (target_sdk_ > compile_sdk_)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project target SDK must not exceed compile SDK");
    }

    if (android_gradle_plugin_version_.empty())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project Android Gradle Plugin version must not be empty");
    }

    if (!valid_package_name(config().app_id()))
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project application id is not a valid Java package name");
    }

    if (config().has_icon_path())
    {
      const std::filesystem::path icon_path(config().icon_path());
      std::error_code error;
      if (!std::filesystem::is_regular_file(icon_path, error) || error)
      {
        return Result<void>::fail(
            ErrorCode::ConfigError,
            "Android project icon path must reference an existing file");
      }

      if (!is_supported_icon_path(icon_path))
      {
        return Result<void>::fail(
            ErrorCode::ConfigError,
            "Android project icon must be a PNG file");
      }
    }

    return Result<void>::ok();
  }

} // namespace vix::ui
