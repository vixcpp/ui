/**
 *
 *  @file IOSProject.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */
#include <vix/ui/mobile/IOSProject.hpp>

#include <cctype>
#include <fstream>
#include <sstream>
#include <utility>

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] std::string safe_project_name(std::string_view name)
    {
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

    [[nodiscard]] std::string swift_string(std::string_view value)
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

    [[nodiscard]] std::string pbx_string(std::string_view value)
    {
      return "\"" + swift_string(value) + "\"";
    }

    [[nodiscard]] bool is_http_url(std::string_view url)
    {
      return url.rfind("http://", 0) == 0;
    }

    [[nodiscard]] bool is_http_or_https_url(std::string_view url)
    {
      return is_http_url(url) || url.rfind("https://", 0) == 0;
    }

    [[nodiscard]] std::string url_host(std::string_view url)
    {
      const std::size_t scheme_end = url.find("://");
      if (scheme_end == std::string_view::npos)
      {
        return {};
      }

      const std::size_t authority_start = scheme_end + 3;
      const std::size_t authority_end = url.find_first_of("/?#", authority_start);
      std::string authority(url.substr(
          authority_start,
          authority_end == std::string_view::npos
              ? std::string_view::npos
              : authority_end - authority_start));

      const std::size_t credentials = authority.rfind('@');
      if (credentials != std::string::npos)
      {
        authority.erase(0, credentials + 1);
      }

      if (authority.empty())
      {
        return {};
      }

      if (authority.front() == '[')
      {
        const std::size_t end = authority.find(']');
        return end == std::string::npos ? std::string{} :
                                          authority.substr(0, end + 1);
      }

      const std::size_t port = authority.find(':');
      return authority.substr(0, port);
    }

    [[nodiscard]] bool valid_bundle_identifier_part(std::string_view part)
    {
      if (part.empty() ||
          !std::isalpha(static_cast<unsigned char>(part.front())))
      {
        return false;
      }

      for (char character : part)
      {
        const unsigned char value = static_cast<unsigned char>(character);
        if (!std::isalnum(value) && character != '-')
        {
          return false;
        }
      }

      return true;
    }

    [[nodiscard]] bool valid_bundle_identifier(std::string_view identifier)
    {
      std::size_t start = 0;
      int parts = 0;

      while (start < identifier.size())
      {
        const std::size_t dot = identifier.find('.', start);
        const std::string_view part =
            dot == std::string_view::npos
                ? identifier.substr(start)
                : identifier.substr(start, dot - start);

        if (!valid_bundle_identifier_part(part))
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

    [[nodiscard]] bool valid_deployment_target(std::string_view target)
    {
      if (target.empty())
      {
        return false;
      }

      int component = 0;
      int major = 0;
      bool has_digit = false;

      for (char character : target)
      {
        if (character == '.')
        {
          if (!has_digit)
          {
            return false;
          }

          ++component;
          has_digit = false;
          continue;
        }

        if (!std::isdigit(static_cast<unsigned char>(character)))
        {
          return false;
        }

        has_digit = true;
        if (component == 0)
        {
          major = major * 10 + (character - '0');
        }
      }

      return has_digit && component <= 2 && major >= 13;
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
            "cannot create iOS project directory: " +
                path.parent_path().string() + ": " + error.message());
      }

      std::ofstream output(path, std::ios::binary | std::ios::trunc);
      if (!output.is_open())
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot write iOS project file: " + path.string());
      }

      output << content;
      if (!output.good())
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "failed while writing iOS project file: " + path.string());
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
            "cannot create iOS icon directory: " +
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
            "cannot copy iOS application icon: " + source.string() +
                ": " + error.message());
      }

      return Result<void>::ok();
    }

    [[nodiscard]] Result<void> remove_file_if_exists(
        const std::filesystem::path &path)
    {
      std::error_code error;
      std::filesystem::remove(path, error);
      if (error)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot remove obsolete iOS project file: " +
                path.string() + ": " + error.message());
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

    [[nodiscard]] std::string render_info_plist(const IOSProject &project)
    {
      const MobileConfig &config = project.config();
      std::ostringstream output;
      output
          << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
          << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
             "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
          << "<plist version=\"1.0\">\n"
          << "<dict>\n"
          << "    <key>CFBundleDisplayName</key>\n"
          << "    <string>" << xml_escape(config.name()) << "</string>\n"
          << "    <key>CFBundleIdentifier</key>\n"
          << "    <string>$(PRODUCT_BUNDLE_IDENTIFIER)</string>\n"
          << "    <key>CFBundleShortVersionString</key>\n"
          << "    <string>" << xml_escape(config.version()) << "</string>\n"
          << "    <key>CFBundleVersion</key>\n"
          << "    <string>1</string>\n"
          << "    <key>UILaunchStoryboardName</key>\n"
          << "    <string></string>\n"
          << "    <key>UIApplicationSceneManifest</key>\n"
          << "    <dict>\n"
          << "        <key>UIApplicationSupportsMultipleScenes</key>\n"
          << "        <false/>\n"
          << "        <key>UISceneConfigurations</key>\n"
          << "        <dict>\n"
          << "            <key>UIWindowSceneSessionRoleApplication</key>\n"
          << "            <array>\n"
          << "                <dict>\n"
          << "                    <key>UISceneConfigurationName</key>\n"
          << "                    <string>Default Configuration</string>\n"
          << "                    <key>UISceneDelegateClassName</key>\n"
          << "                    <string>$(PRODUCT_MODULE_NAME).SceneDelegate</string>\n"
          << "                </dict>\n"
          << "            </array>\n"
          << "        </dict>\n"
          << "    </dict>\n";

      if (is_http_url(config.url()))
      {
        output
            << "    <key>NSAppTransportSecurity</key>\n"
            << "    <dict>\n"
            << "        <key>NSExceptionDomains</key>\n"
            << "        <dict>\n"
            << "            <key>" << xml_escape(url_host(config.url()))
            << "</key>\n"
            << "            <dict>\n"
            << "                <key>NSExceptionAllowsInsecureHTTPLoads</key>\n"
            << "                <true/>\n"
            << "            </dict>\n"
            << "        </dict>\n"
            << "    </dict>\n";
      }

      output
          << "</dict>\n"
          << "</plist>\n";
      return output.str();
    }

    [[nodiscard]] std::string render_app_delegate()
    {
      return "import UIKit\n\n"
             "@main\n"
             "class AppDelegate: UIResponder, UIApplicationDelegate {\n"
             "    func application(\n"
             "        _ application: UIApplication,\n"
             "        configurationForConnecting connectingSceneSession: UISceneSession,\n"
             "        options: UIScene.ConnectionOptions\n"
             "    ) -> UISceneConfiguration {\n"
             "        UISceneConfiguration(\n"
             "            name: \"Default Configuration\",\n"
             "            sessionRole: connectingSceneSession.role\n"
             "        )\n"
             "    }\n"
             "}\n";
    }

    [[nodiscard]] std::string render_scene_delegate()
    {
      return "import UIKit\n\n"
             "class SceneDelegate: UIResponder, UIWindowSceneDelegate {\n"
             "    var window: UIWindow?\n\n"
             "    func scene(\n"
             "        _ scene: UIScene,\n"
             "        willConnectTo session: UISceneSession,\n"
             "        options connectionOptions: UIScene.ConnectionOptions\n"
             "    ) {\n"
             "        guard let windowScene = scene as? UIWindowScene else { return }\n"
             "        let window = UIWindow(windowScene: windowScene)\n"
             "        window.rootViewController = ViewController()\n"
             "        self.window = window\n"
             "        window.makeKeyAndVisible()\n"
             "    }\n"
             "}\n";
    }

    [[nodiscard]] std::string render_view_controller(const IOSProject &project)
    {
      return "import UIKit\n"
             "import UniformTypeIdentifiers\n"
             "import WebKit\n\n"
             "final class ViewController: UIViewController, WKNavigationDelegate, WKUIDelegate, UIDocumentPickerDelegate, WKDownloadDelegate {\n"
             "    private let appURL = URL(string: \"" +
             swift_string(project.config().url()) +
             "\")!\n"
             "    private let retryURL = URL(string: \"vix://retry\")!\n"
             "    private var openPanelCompletionHandler: (([URL]?) -> Void)?\n"
             "    private weak var documentPicker: UIDocumentPickerViewController?\n"
             "    private lazy var webView: WKWebView = {\n"
             "        let configuration = WKWebViewConfiguration()\n"
             "        configuration.preferences.javaScriptEnabled = true\n"
             "        let view = WKWebView(frame: .zero, configuration: configuration)\n"
             "        view.navigationDelegate = self\n"
             "        view.uiDelegate = self\n"
             "        view.allowsBackForwardNavigationGestures = true\n"
             "        return view\n"
             "    }()\n\n"
             "    override func viewDidLoad() {\n"
             "        super.viewDidLoad()\n"
             "        view = webView\n"
             "        webView.load(URLRequest(url: appURL))\n"
             "    }\n\n"
             "    private func loadInitialURL() {\n"
             "        webView.load(URLRequest(url: appURL))\n"
             "    }\n\n"
             "    func webView(\n"
             "        _ webView: WKWebView,\n"
             "        decidePolicyFor navigationAction: WKNavigationAction,\n"
             "        decisionHandler: @escaping (WKNavigationActionPolicy) -> Void\n"
             "    ) {\n"
             "        guard let url = navigationAction.request.url,\n"
             "              let scheme = url.scheme?.lowercased() else {\n"
             "            decisionHandler(.cancel)\n"
             "            return\n"
             "        }\n\n"
             "        if url == retryURL {\n"
             "            loadInitialURL()\n"
             "            decisionHandler(.cancel)\n"
             "            return\n"
             "        }\n\n"
             "        if scheme == \"http\" || scheme == \"https\" {\n"
             "            if let host = url.host,\n"
             "               let appHost = appURL.host,\n"
             "               host.caseInsensitiveCompare(appHost) == .orderedSame {\n"
             "                decisionHandler(.allow)\n"
             "            } else {\n"
             "                openExternally(url)\n"
             "                decisionHandler(.cancel)\n"
             "            }\n"
             "            return\n"
             "        }\n\n"
             "        if scheme == \"mailto\" || scheme == \"tel\" {\n"
             "            openExternally(url)\n"
             "        }\n"
             "        decisionHandler(.cancel)\n"
             "    }\n\n"
             "    func webView(\n"
             "        _ webView: WKWebView,\n"
             "        decidePolicyFor navigationResponse: WKNavigationResponse,\n"
             "        decisionHandler: @escaping (WKNavigationResponsePolicy) -> Void\n"
             "    ) {\n"
             "        decisionHandler(navigationResponse.canShowMIMEType ? .allow : .download)\n"
             "    }\n\n"
             "    func webView(\n"
             "        _ webView: WKWebView,\n"
             "        navigationAction: WKNavigationAction,\n"
             "        didBecome download: WKDownload\n"
             "    ) {\n"
             "        download.delegate = self\n"
             "    }\n\n"
             "    func webView(\n"
             "        _ webView: WKWebView,\n"
             "        navigationResponse: WKNavigationResponse,\n"
             "        didBecome download: WKDownload\n"
             "    ) {\n"
             "        download.delegate = self\n"
             "    }\n\n"
             "    func download(\n"
             "        _ download: WKDownload,\n"
             "        decideDestinationUsing response: URLResponse,\n"
             "        suggestedFilename: String,\n"
             "        completionHandler: @escaping (URL?) -> Void\n"
             "    ) {\n"
             "        guard let url = response.url,\n"
             "              url.scheme == \"http\" || url.scheme == \"https\" else {\n"
             "            completionHandler(nil)\n"
             "            return\n"
             "        }\n"
             "        let destination = FileManager.default.urls(\n"
             "            for: .documentDirectory,\n"
             "            in: .userDomainMask\n"
             "        )[0].appendingPathComponent(suggestedFilename)\n"
             "        completionHandler(destination)\n"
             "    }\n\n"
             "    func webView(\n"
             "        _ webView: WKWebView,\n"
             "        runOpenPanelWith parameters: WKOpenPanelParameters,\n"
             "        initiatedByFrame frame: WKFrameInfo,\n"
             "        completionHandler: @escaping ([URL]?) -> Void\n"
             "    ) {\n"
             "        openPanelCompletionHandler?(nil)\n"
             "        openPanelCompletionHandler = completionHandler\n"
             "        let picker = UIDocumentPickerViewController(\n"
             "            forOpeningContentTypes: [UTType.item],\n"
             "            asCopy: true\n"
             "        )\n"
             "        picker.allowsMultipleSelection = parameters.allowsMultipleSelection\n"
             "        picker.delegate = self\n"
             "        documentPicker = picker\n"
             "        present(picker, animated: true)\n"
             "    }\n\n"
             "    func documentPicker(\n"
             "        _ controller: UIDocumentPickerViewController,\n"
             "        didPickDocumentsAt urls: [URL]\n"
             "    ) {\n"
             "        openPanelCompletionHandler?(urls)\n"
             "        openPanelCompletionHandler = nil\n"
             "    }\n\n"
             "    func documentPickerWasCancelled(\n"
             "        _ controller: UIDocumentPickerViewController\n"
             "    ) {\n"
             "        openPanelCompletionHandler?(nil)\n"
             "        openPanelCompletionHandler = nil\n"
             "    }\n\n"
             "    func webView(\n"
             "        _ webView: WKWebView,\n"
             "        didFailProvisionalNavigation navigation: WKNavigation?,\n"
             "        withError error: Error\n"
             "    ) {\n"
             "        showErrorPage(for: error)\n"
             "    }\n\n"
             "    func webView(\n"
             "        _ webView: WKWebView,\n"
             "        didFail navigation: WKNavigation?,\n"
             "        withError error: Error\n"
             "    ) {\n"
             "        showErrorPage(for: error)\n"
             "    }\n\n"
             "    private func showErrorPage(for error: Error) {\n"
             "        guard (error as NSError).code != NSURLErrorCancelled else { return }\n"
             "        let page = \"<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'></head><body><h1>Unable to load the application</h1><p>Please check your connection and try again.</p><button onclick=\\\"location.href='vix://retry'\\\">Retry</button></body></html>\"\n"
             "        webView.loadHTMLString(page, baseURL: nil)\n"
             "    }\n\n"
             "    private func openExternally(_ url: URL) {\n"
             "        guard UIApplication.shared.canOpenURL(url) else { return }\n"
             "        UIApplication.shared.open(url)\n"
             "    }\n\n"
             "    deinit {\n"
             "        webView.stopLoading()\n"
             "        openPanelCompletionHandler?(nil)\n"
             "        documentPicker?.delegate = nil\n"
             "        webView.navigationDelegate = nil\n"
             "        webView.uiDelegate = nil\n"
             "    }\n"
             "}\n";
    }

    [[nodiscard]] std::string render_app_icon_contents()
    {
      return "{\n"
             "  \"images\" : [\n\n"
             "  ],\n"
             "  \"info\" : {\n"
             "    \"author\" : \"xcode\",\n"
             "    \"version\" : 1\n"
             "  }\n"
             "}\n";
    }

    [[nodiscard]] std::string render_branding_contents()
    {
      return "{\n"
             "  \"images\" : [\n"
             "    {\n"
             "      \"filename\" : \"Icon.png\",\n"
             "      \"idiom\" : \"universal\",\n"
             "      \"scale\" : \"1x\"\n"
             "    }\n"
             "  ],\n"
             "  \"info\" : {\n"
             "    \"author\" : \"xcode\",\n"
             "    \"version\" : 1\n"
             "  }\n"
             "}\n";
    }

    [[nodiscard]] std::string render_xcode_project(const IOSProject &project,
                                                    std::string_view name)
    {
      const std::string bundle = pbx_string(project.config().app_id());
      const std::string version = pbx_string(project.config().version());
      const std::string target = pbx_string(project.deployment_target());
      const std::string project_name(name);

      return "// !$*UTF8*$!\n"
             "{\n"
             "\tarchiveVersion = 1;\n"
             "\tclasses = {};\n"
             "\tobjectVersion = 56;\n"
             "\tobjects = {\n\n"
             "\t\t000000000000000000000001 /* AppDelegate.swift */ = {isa = PBXFileReference; lastKnownFileType = sourcecode.swift; path = AppDelegate.swift; sourceTree = \"<group>\"; };\n"
             "\t\t000000000000000000000002 /* SceneDelegate.swift */ = {isa = PBXFileReference; lastKnownFileType = sourcecode.swift; path = SceneDelegate.swift; sourceTree = \"<group>\"; };\n"
             "\t\t000000000000000000000003 /* ViewController.swift */ = {isa = PBXFileReference; lastKnownFileType = sourcecode.swift; path = ViewController.swift; sourceTree = \"<group>\"; };\n"
             "\t\t000000000000000000000004 /* Info.plist */ = {isa = PBXFileReference; lastKnownFileType = text.plist.xml; path = Info.plist; sourceTree = \"<group>\"; };\n"
             "\t\t000000000000000000000005 /* Assets.xcassets */ = {isa = PBXFileReference; lastKnownFileType = folder.assetcatalog; path = Assets.xcassets; sourceTree = \"<group>\"; };\n"
             "\t\t000000000000000000000006 /* " + project_name + ".app */ = {isa = PBXFileReference; explicitFileType = wrapper.application; path = " + project_name + ".app; sourceTree = BUILT_PRODUCTS_DIR; };\n\n"
             "\t\t000000000000000000000007 /* WebKit.framework */ = {isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = WebKit.framework; path = System/Library/Frameworks/WebKit.framework; sourceTree = SDKROOT; };\n\n"
             "\t\t000000000000000000000101 /* AppDelegate.swift in Sources */ = {isa = PBXBuildFile; fileRef = 000000000000000000000001 /* AppDelegate.swift */; };\n"
             "\t\t000000000000000000000102 /* SceneDelegate.swift in Sources */ = {isa = PBXBuildFile; fileRef = 000000000000000000000002 /* SceneDelegate.swift */; };\n"
             "\t\t000000000000000000000103 /* ViewController.swift in Sources */ = {isa = PBXBuildFile; fileRef = 000000000000000000000003 /* ViewController.swift */; };\n"
             "\t\t000000000000000000000104 /* Assets.xcassets in Resources */ = {isa = PBXBuildFile; fileRef = 000000000000000000000005 /* Assets.xcassets */; };\n"
             "\t\t000000000000000000000105 /* WebKit.framework in Frameworks */ = {isa = PBXBuildFile; fileRef = 000000000000000000000007 /* WebKit.framework */; };\n\n"
             "\t\t000000000000000000000201 = {isa = PBXGroup; children = (000000000000000000000202 /* " + project_name + " */, 000000000000000000000203 /* Products */, 000000000000000000000204 /* Frameworks */); sourceTree = \"<group>\"; };\n"
             "\t\t000000000000000000000202 /* " + project_name + " */ = {isa = PBXGroup; children = (000000000000000000000001, 000000000000000000000002, 000000000000000000000003, 000000000000000000000004, 000000000000000000000005); path = " + project_name + "; sourceTree = \"<group>\"; };\n"
             "\t\t000000000000000000000203 /* Products */ = {isa = PBXGroup; children = (000000000000000000000006 /* " + project_name + ".app */); name = Products; sourceTree = \"<group>\"; };\n\n"
             "\t\t000000000000000000000204 /* Frameworks */ = {isa = PBXGroup; children = (000000000000000000000007 /* WebKit.framework */); name = Frameworks; sourceTree = \"<group>\"; };\n\n"
             "\t\t000000000000000000000301 = {isa = PBXSourcesBuildPhase; buildActionMask = 2147483647; files = (000000000000000000000101, 000000000000000000000102, 000000000000000000000103); runOnlyForDeploymentPostprocessing = 0; };\n"
             "\t\t000000000000000000000302 = {isa = PBXResourcesBuildPhase; buildActionMask = 2147483647; files = (000000000000000000000104); runOnlyForDeploymentPostprocessing = 0; };\n"
             "\t\t000000000000000000000303 = {isa = PBXFrameworksBuildPhase; buildActionMask = 2147483647; files = (000000000000000000000105); runOnlyForDeploymentPostprocessing = 0; };\n"
             "\t\t000000000000000000000401 = {isa = PBXNativeTarget; buildConfigurationList = 000000000000000000000501; buildPhases = (000000000000000000000301, 000000000000000000000303, 000000000000000000000302); buildRules = (); dependencies = (); name = " + project_name + "; productName = " + project_name + "; productReference = 000000000000000000000006; productType = \"com.apple.product-type.application\"; };\n"
             "\t\t000000000000000000000601 = {isa = PBXProject; attributes = { LastUpgradeCheck = 1600; }; buildConfigurationList = 000000000000000000000502; compatibilityVersion = \"Xcode 14.0\"; developmentRegion = en; hasScannedForEncodings = 0; knownRegions = (en, Base); mainGroup = 000000000000000000000201; productRefGroup = 000000000000000000000203; projectDirPath = \"\"; projectRoot = \"\"; targets = (000000000000000000000401); };\n\n"
             "\t\t000000000000000000000701 = {isa = XCBuildConfiguration; buildSettings = { CLANG_ENABLE_MODULES = YES; }; name = Debug; };\n"
             "\t\t000000000000000000000702 = {isa = XCBuildConfiguration; buildSettings = { CLANG_ENABLE_MODULES = YES; }; name = Release; };\n"
             "\t\t000000000000000000000703 = {isa = XCBuildConfiguration; buildSettings = { CURRENT_PROJECT_VERSION = 1; GENERATE_INFOPLIST_FILE = NO; INFOPLIST_FILE = " + project_name + "/Info.plist; IPHONEOS_DEPLOYMENT_TARGET = " + target + "; MARKETING_VERSION = " + version + "; PRODUCT_BUNDLE_IDENTIFIER = " + bundle + "; PRODUCT_NAME = " + project_name + "; SDKROOT = iphoneos; SWIFT_VERSION = 5.0; TARGETED_DEVICE_FAMILY = \"1,2\"; }; name = Debug; };\n"
             "\t\t000000000000000000000704 = {isa = XCBuildConfiguration; buildSettings = { CURRENT_PROJECT_VERSION = 1; GENERATE_INFOPLIST_FILE = NO; INFOPLIST_FILE = " + project_name + "/Info.plist; IPHONEOS_DEPLOYMENT_TARGET = " + target + "; MARKETING_VERSION = " + version + "; PRODUCT_BUNDLE_IDENTIFIER = " + bundle + "; PRODUCT_NAME = " + project_name + "; SDKROOT = iphoneos; SWIFT_VERSION = 5.0; TARGETED_DEVICE_FAMILY = \"1,2\"; }; name = Release; };\n"
             "\t\t000000000000000000000501 = {isa = XCConfigurationList; buildConfigurations = (000000000000000000000703, 000000000000000000000704); defaultConfigurationIsVisible = 0; defaultConfigurationName = Release; };\n"
             "\t\t000000000000000000000502 = {isa = XCConfigurationList; buildConfigurations = (000000000000000000000701, 000000000000000000000702); defaultConfigurationIsVisible = 0; defaultConfigurationName = Release; };\n"
             "\t};\n"
             "\trootObject = 000000000000000000000601;\n"
             "}\n";
    }
  } // namespace

  IOSProject::IOSProject(MobileProject project)
      : MobileProject(std::move(project))
  {
  }

  IOSProject &IOSProject::set_deployment_target(std::string target)
  {
    deployment_target_ = std::move(target);
    return *this;
  }

  const std::string &IOSProject::deployment_target() const noexcept
  {
    return deployment_target_;
  }

  Result<void> IOSProject::generate(const std::filesystem::path &directory) const
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
          "iOS project output directory must not be empty");
    }

    const std::string name = safe_project_name(config().name());
    const std::filesystem::path project_root = directory / name;
    const std::filesystem::path icon_root =
        project_root / "Assets.xcassets" / "AppIcon.appiconset";
    const std::filesystem::path branding_root =
        project_root / "Assets.xcassets" / "Branding.imageset";

    const std::pair<std::filesystem::path, std::string> files[]{
        {directory / (name + ".xcodeproj") / "project.pbxproj",
         render_xcode_project(*this, name)},
        {project_root / "AppDelegate.swift", render_app_delegate()},
        {project_root / "SceneDelegate.swift", render_scene_delegate()},
        {project_root / "ViewController.swift", render_view_controller(*this)},
        {project_root / "Info.plist", render_info_plist(*this)},
        {icon_root / "Contents.json", render_app_icon_contents()}};

    for (const auto &[path, content] : files)
    {
      Result<void> write_result = write_text_file(path, content);
      if (write_result.is_failed())
      {
        return write_result;
      }
    }

    if (config().has_icon_path())
    {
      Result<void> icon_result = copy_icon_file(
          config().icon_path(),
          branding_root / "Icon.png");
      if (icon_result.is_failed())
      {
        return icon_result;
      }

      Result<void> contents_result = write_text_file(
          branding_root / "Contents.json",
          render_branding_contents());
      if (contents_result.is_failed())
      {
        return contents_result;
      }
    }
    else
    {
      Result<void> remove_result = remove_file_if_exists(
          branding_root / "Icon.png");
      if (remove_result.is_failed())
      {
        return remove_result;
      }

      remove_result = remove_file_if_exists(branding_root / "Contents.json");
      if (remove_result.is_failed())
      {
        return remove_result;
      }
    }

    return Result<void>::ok();
  }

  Result<void> IOSProject::validate() const
  {
    Result<void> mobile_validation = MobileProject::validate();
    if (mobile_validation.is_failed())
    {
      return mobile_validation;
    }

    if (!valid_bundle_identifier(config().app_id()))
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "iOS project application id is not a valid bundle identifier");
    }

    if (!is_http_or_https_url(config().url()) || url_host(config().url()).empty())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "iOS project URL must use http:// or https:// with a host");
    }

    if (!valid_deployment_target(deployment_target_))
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "iOS project deployment target must be iOS 13.0 or later");
    }

    if (config().has_icon_path())
    {
      const std::filesystem::path icon_path(config().icon_path());
      std::error_code error;
      if (!std::filesystem::is_regular_file(icon_path, error) || error)
      {
        return Result<void>::fail(
            ErrorCode::ConfigError,
            "iOS project icon path must reference an existing file");
      }

      if (!is_supported_icon_path(icon_path))
      {
        return Result<void>::fail(
            ErrorCode::ConfigError,
            "iOS project icon must be a PNG file");
      }
    }

    return Result<void>::ok();
  }
} // namespace vix::ui
