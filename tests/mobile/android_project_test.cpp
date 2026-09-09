/**
 *
 *  @file android_project_test.cpp
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
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include <vix/ui/mobile/AndroidProject.hpp>

using namespace vix::ui;
namespace fs = std::filesystem;

static fs::path test_directory()
{
  return fs::temp_directory_path() / "vix_ui_android_project_test";
}

static std::string read_file(const fs::path &path)
{
  std::ifstream input(path, std::ios::binary);
  return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

static void write_file(const fs::path &path, const std::string &content)
{
  std::ofstream output(path, std::ios::binary | std::ios::trunc);
  output << content;
  assert(output.good());
}

static MobileProject make_valid_mobile_project()
{
  MobileConfig config;
  config.set_name("Vix Mobile Demo")
      .set_app_id("com.softadastra.vix.mobile.demo")
      .set_version("1.2.3")
      .set_url("https://example.test");
  return MobileProject(std::move(config));
}

static AndroidProject make_valid_project()
{
  return AndroidProject(make_valid_mobile_project());
}

static void test_default_values()
{
  AndroidProject project;

  assert(project.min_sdk() == 23);
  assert(project.target_sdk() == 36);
  assert(project.compile_sdk() == 36);
  assert(project.version_code() == 1);
  assert(project.android_gradle_plugin_version() == "8.13.2");
}

static void test_common_configuration_access()
{
  AndroidProject project(make_valid_mobile_project());

  assert(project.config().name() == "Vix Mobile Demo");
  assert(project.config().app_id() == "com.softadastra.vix.mobile.demo");

  project.config().set_version("2.0.0");

  const AndroidProject &const_project = project;
  assert(const_project.config().version() == "2.0.0");
}

static void test_setters_getters_and_chaining()
{
  AndroidProject project = make_valid_project();

  AndroidProject &result = project.set_min_sdk(24)
                               .set_target_sdk(35)
                               .set_compile_sdk(36)
                               .set_version_code(7)
                               .set_android_gradle_plugin_version("8.14.0");

  assert(&result == &project);
  assert(project.min_sdk() == 24);
  assert(project.target_sdk() == 35);
  assert(project.compile_sdk() == 36);
  assert(project.version_code() == 7);
  assert(project.android_gradle_plugin_version() == "8.14.0");
}

static void test_validation_succeeds()
{
  AndroidProject project = make_valid_project();

  assert(project.validate().is_ok());
}

static void test_min_sdk_must_not_exceed_target_sdk()
{
  AndroidProject project = make_valid_project();
  project.set_min_sdk(37);

  Result<void> result = project.validate();
  assert(result.is_failed());
  assert(result.error_message() ==
         "Android project min SDK must not exceed target SDK");
}

static void test_target_sdk_must_not_exceed_compile_sdk()
{
  AndroidProject project = make_valid_project();
  project.set_target_sdk(37);

  Result<void> result = project.validate();
  assert(result.is_failed());
  assert(result.error_message() ==
         "Android project target SDK must not exceed compile SDK");
}

static void test_positive_android_values_are_required()
{
  AndroidProject project = make_valid_project();

  project.set_min_sdk(0);
  assert(project.validate().is_failed());

  project.set_min_sdk(23).set_target_sdk(0);
  assert(project.validate().is_failed());

  project.set_target_sdk(36).set_compile_sdk(0);
  assert(project.validate().is_failed());

  project.set_compile_sdk(36).set_version_code(0);
  assert(project.validate().is_failed());
}

static void test_agp_version_is_required()
{
  AndroidProject project = make_valid_project();
  project.set_android_gradle_plugin_version("");

  Result<void> result = project.validate();
  assert(result.is_failed());
  assert(result.error_message() ==
         "Android project Android Gradle Plugin version must not be empty");
}

static void test_invalid_mobile_configuration_is_propagated()
{
  MobileConfig config;
  config.set_app_id("");

  AndroidProject project(MobileProject(std::move(config)));
  Result<void> result = project.validate();

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(result.error_message() == "mobile application id must not be empty");
}

static void test_generates_base_android_files()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  AndroidProject project = make_valid_project();
  Result<void> result = project.generate(directory);

  assert(result.is_ok());
  assert(fs::is_regular_file(directory / "settings.gradle"));
  assert(fs::is_regular_file(directory / "build.gradle"));
  assert(fs::is_regular_file(directory / "gradle.properties"));
  assert(fs::is_regular_file(directory / "app" / "build.gradle"));
  assert(fs::is_regular_file(directory / "app" / "src" / "main" /
                             "AndroidManifest.xml"));
  assert(fs::is_regular_file(directory / "app" / "src" / "main" / "java" /
                             "com" / "softadastra" / "vix" / "mobile" /
                             "demo" / "MainActivity.java"));
  assert(fs::is_regular_file(directory / "app" / "src" / "main" / "res" /
                             "values" / "strings.xml"));
  assert(fs::is_regular_file(directory / "app" / "src" / "main" / "res" /
                             "values" / "colors.xml"));
  assert(fs::is_regular_file(directory / "app" / "src" / "main" / "res" /
                             "values" / "styles.xml"));

  assert(read_file(directory / "settings.gradle") ==
         "pluginManagement {\n"
         "    repositories {\n"
         "        google()\n"
         "        mavenCentral()\n"
         "        gradlePluginPortal()\n"
         "    }\n"
         "}\n\n"
         "dependencyResolutionManagement {\n"
         "    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)\n"
         "    repositories {\n"
         "        google()\n"
         "        mavenCentral()\n"
         "    }\n"
         "}\n\n"
         "rootProject.name = 'Vix_Mobile_Demo'\n"
         "include ':app'\n");

  assert(read_file(directory / "build.gradle") ==
         "plugins {\n"
         "    id 'com.android.application' version '8.13.2' apply false\n"
         "}\n");

  const fs::path main_root = directory / "app" / "src" / "main";
  const std::string manifest = read_file(main_root / "AndroidManifest.xml");
  const std::string activity = read_file(
      main_root / "java" / "com" / "softadastra" / "vix" / "mobile" /
      "demo" / "MainActivity.java");
  assert(manifest.find("android.permission.INTERNET") != std::string::npos);
  assert(manifest.find("android:label=\"@string/app_name\"") != std::string::npos);
  assert(manifest.find("android:icon=\"@mipmap/ic_launcher\"") !=
         std::string::npos);
  assert(manifest.find("android:theme=\"@style/AppTheme\"") != std::string::npos);
  assert(manifest.find("android:usesCleartextTraffic") == std::string::npos);
  assert(manifest.find("READ_EXTERNAL_STORAGE") == std::string::npos);
  assert(manifest.find("WRITE_EXTERNAL_STORAGE") == std::string::npos);
  const std::size_t permission = manifest.find("<uses-permission");
  assert(permission != std::string::npos);
  assert(manifest.find("<uses-permission", permission + 1) ==
         std::string::npos);
  assert(activity.find("package com.softadastra.vix.mobile.demo;") != std::string::npos);
  assert(activity.find("https://example.test") != std::string::npos);
  assert(activity.find("settings.setJavaScriptEnabled(true);") != std::string::npos);
  assert(activity.find("settings.setDomStorageEnabled(true);") != std::string::npos);
  assert(activity.find("settings.setAllowFileAccess(false);") != std::string::npos);
  assert(activity.find("settings.setAllowContentAccess(false);") != std::string::npos);
  assert(activity.find("setAllowUniversalAccessFromFileURLs") ==
         std::string::npos);
  assert(activity.find("setAllowFileAccessFromFileURLs") == std::string::npos);
  assert(activity.find("WebChromeClient") != std::string::npos);
  assert(activity.find("onShowFileChooser") != std::string::npos);
  assert(activity.find("ValueCallback<Uri[]>") != std::string::npos);
  assert(activity.find("FileChooserParams") != std::string::npos);
  assert(activity.find("Intent.EXTRA_ALLOW_MULTIPLE") != std::string::npos);
  assert(activity.find("FileChooserParams.MODE_OPEN_MULTIPLE") !=
         std::string::npos);
  assert(activity.find("WebChromeClient.FileChooserParams.parseResult") !=
         std::string::npos);
  assert(activity.find("fileChooserCallback.onReceiveValue(null);") !=
         std::string::npos);
  assert(activity.find("setDownloadListener") != std::string::npos);
  assert(activity.find("DownloadManager.Request") != std::string::npos);
  assert(activity.find("URLUtil.guessFileName") != std::string::npos);
  assert(activity.find("CookieManager.getInstance().getCookie") !=
         std::string::npos);
  assert(activity.find("VISIBILITY_VISIBLE_NOTIFY_COMPLETED") !=
         std::string::npos);
  assert(activity.find("if (!isHttpOrHttps(downloadUri))") !=
         std::string::npos);
  assert(activity.find("return handleNavigation(request.getUrl());") !=
         std::string::npos);
  assert(activity.find("initialHost.equalsIgnoreCase(host)") !=
         std::string::npos);
  assert(activity.find("if (isHttpOrHttps(uri))") != std::string::npos);
  assert(activity.find("private boolean isHttpOrHttps(Uri uri)") !=
         std::string::npos);
  assert(activity.find("Intent.ACTION_VIEW") != std::string::npos);
  assert(activity.find("Intent.CATEGORY_BROWSABLE") != std::string::npos);
  assert(activity.find("mailto") != std::string::npos);
  assert(activity.find("tel") != std::string::npos);
  assert(activity.find("onReceivedError") != std::string::npos);
  assert(activity.find("request.isForMainFrame()") != std::string::npos);
  assert(activity.find("onReceivedHttpError") != std::string::npos);
  assert(activity.find("ERROR_PAGE") != std::string::npos);
  assert(activity.find("vix://retry") != std::string::npos);
  assert(activity.find("loadDataWithBaseURL") != std::string::npos);
  assert(activity.find("webView.restoreState(savedInstanceState)") !=
         std::string::npos);
  assert(activity.find("webView.saveState(outState)") != std::string::npos);
  assert(activity.find("webView.onPause()") != std::string::npos);
  assert(activity.find("webView.onResume()") != std::string::npos);
  assert(activity.find("webView.canGoBack()") != std::string::npos);
  assert(activity.find("webView.stopLoading()") != std::string::npos);
  assert(activity.find("webView.removeAllViews()") != std::string::npos);
  assert(activity.find("webView.destroy();") != std::string::npos);
  assert(activity.find("onReceivedSslError") == std::string::npos);
  assert(activity.find("SslErrorHandler") == std::string::npos);

  assert(read_file(main_root / "res" / "values" / "strings.xml") ==
         "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
         "<resources>\n"
         "    <string name=\"app_name\">Vix Mobile Demo</string>\n"
         "</resources>\n");
  assert(read_file(main_root / "res" / "values" / "colors.xml") ==
         "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
         "<resources>\n"
         "    <color name=\"vix_accent\">#f37726</color>\n"
         "</resources>\n");
  assert(read_file(main_root / "res" / "values" / "styles.xml").find(
             "<style name=\"AppTheme\"") != std::string::npos);
  assert(read_file(main_root / "res" / "mipmap" / "ic_launcher.xml").find(
             "<vector") != std::string::npos);

  const fs::path local_properties = directory / "local.properties";
  if (fs::exists(local_properties))
  {
    assert(read_file(local_properties).rfind("sdk.dir=", 0) == 0);
  }

  assert(read_file(directory / "gradle.properties") ==
         "org.gradle.jvmargs=-Xmx2048m -Dfile.encoding=UTF-8\n"
         "android.useAndroidX=false\n");

  assert(read_file(directory / "app" / "build.gradle") ==
         "plugins {\n"
         "    id 'com.android.application'\n"
         "}\n\n"
         "android {\n"
         "    namespace 'com.softadastra.vix.mobile.demo'\n"
         "    compileSdk 36\n\n"
         "    defaultConfig {\n"
         "        applicationId 'com.softadastra.vix.mobile.demo'\n"
         "        minSdk 23\n"
         "        targetSdk 36\n"
         "        versionCode 1\n"
         "        versionName '1.2.3'\n"
         "    }\n"
         "}\n");

  fs::remove_all(directory, error);
  assert(!error);
}

static void test_generation_is_deterministic()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  AndroidProject project = make_valid_project();
  assert(project.generate(directory).is_ok());
  const std::string first_settings = read_file(directory / "settings.gradle");
  const std::string first_root_build = read_file(directory / "build.gradle");
  const std::string first_properties = read_file(directory / "gradle.properties");
  const std::string first_app_build = read_file(directory / "app" / "build.gradle");
  const std::string first_manifest = read_file(
      directory / "app" / "src" / "main" / "AndroidManifest.xml");
  const std::string first_activity = read_file(
      directory / "app" / "src" / "main" / "java" / "com" /
      "softadastra" / "vix" / "mobile" / "demo" / "MainActivity.java");
  const std::string first_icon = read_file(
      directory / "app" / "src" / "main" / "res" / "mipmap" /
      "ic_launcher.xml");

  assert(project.generate(directory).is_ok());
  assert(read_file(directory / "settings.gradle") == first_settings);
  assert(read_file(directory / "build.gradle") == first_root_build);
  assert(read_file(directory / "gradle.properties") == first_properties);
  assert(read_file(directory / "app" / "build.gradle") == first_app_build);
  assert(read_file(directory / "app" / "src" / "main" / "AndroidManifest.xml") ==
         first_manifest);
  assert(read_file(directory / "app" / "src" / "main" / "java" / "com" /
                   "softadastra" / "vix" / "mobile" / "demo" /
                   "MainActivity.java") == first_activity);
  assert(read_file(directory / "app" / "src" / "main" / "res" / "mipmap" /
                   "ic_launcher.xml") == first_icon);

  fs::remove_all(directory, error);
  assert(!error);
}

static void test_generation_validates_before_writing()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  AndroidProject project;
  Result<void> result = project.generate(directory);

  assert(result.is_failed());
  assert(result.error_message() == "mobile application id must not be empty");
  assert(!fs::exists(directory));
}

static void test_http_enables_cleartext_and_https_does_not()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  AndroidProject project = make_valid_project();
  project.config().set_url("http://127.0.0.1:8080");
  assert(project.generate(directory).is_ok());
  assert(read_file(directory / "app" / "src" / "main" /
                   "AndroidManifest.xml")
             .find("android:usesCleartextTraffic=\"true\"") !=
         std::string::npos);

  fs::remove_all(directory, error);
  assert(!error);

  project.config().set_url("https://example.test");
  assert(project.generate(directory).is_ok());
  assert(read_file(directory / "app" / "src" / "main" /
                   "AndroidManifest.xml")
             .find("android:usesCleartextTraffic") == std::string::npos);

  fs::remove_all(directory, error);
  assert(!error);
}

static void test_invalid_package_name_is_rejected_before_writing()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  AndroidProject project = make_valid_project();
  project.config().set_app_id("com.example.invalid-package");
  Result<void> result = project.generate(directory);

  assert(result.is_failed());
  assert(result.error_message() ==
         "Android project application id is not a valid Java package name");
  assert(!fs::exists(directory));
}

static void test_configured_png_icon_is_copied_and_referenced()
{
  const fs::path directory = test_directory();
  const fs::path source = fs::temp_directory_path() /
                          "vix_ui_android_project_icon_source.png";
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);
  fs::remove(source, error);
  assert(!error);

  const std::string png{"\x89PNG\r\n\x1a\nVix", 12};
  write_file(source, png);

  AndroidProject project = make_valid_project();
  project.config().set_icon_path(source.string());
  assert(project.generate(directory).is_ok());

  const fs::path icon = directory / "app" / "src" / "main" / "res" /
                        "mipmap" / "ic_launcher.png";
  assert(fs::is_regular_file(icon));
  assert(read_file(icon) == png);
  assert(read_file(directory / "app" / "src" / "main" /
                   "AndroidManifest.xml")
             .find("android:icon=\"@mipmap/ic_launcher\"") !=
         std::string::npos);

  fs::remove_all(directory, error);
  assert(!error);
  fs::remove(source, error);
  assert(!error);
}

static void test_missing_icon_is_rejected_before_writing()
{
  const fs::path directory = test_directory();
  const fs::path missing = fs::temp_directory_path() /
                           "vix_ui_android_project_missing_icon.png";
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);
  fs::remove(missing, error);
  assert(!error);

  AndroidProject project = make_valid_project();
  project.config().set_icon_path(missing.string());
  Result<void> result = project.generate(directory);

  assert(result.is_failed());
  assert(result.error_message() ==
         "Android project icon path must reference an existing file");
  assert(!fs::exists(directory));
}

int main()
{
  test_default_values();
  test_common_configuration_access();
  test_setters_getters_and_chaining();
  test_validation_succeeds();
  test_min_sdk_must_not_exceed_target_sdk();
  test_target_sdk_must_not_exceed_compile_sdk();
  test_positive_android_values_are_required();
  test_agp_version_is_required();
  test_invalid_mobile_configuration_is_propagated();
  test_generates_base_android_files();
  test_generation_is_deterministic();
  test_generation_validates_before_writing();
  test_http_enables_cleartext_and_https_does_not();
  test_invalid_package_name_is_rejected_before_writing();
  test_configured_png_icon_is_copied_and_referenced();
  test_missing_icon_is_rejected_before_writing();

  std::cout << "android_project_test: all tests passed\n";
  return 0;
}
