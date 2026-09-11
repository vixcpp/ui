/**
 *
 *  @file ios_project_test.cpp
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
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include <vix/ui/all.hpp>

using namespace vix::ui;
namespace fs = std::filesystem;

static fs::path test_directory()
{
  return fs::temp_directory_path() / "vix_ui_ios_project_test";
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
  config.set_name("Vix iOS Demo")
      .set_app_id("com.softadastra.vix.iosdemo")
      .set_version("1.2.3")
      .set_url("https://example.test");
  return MobileProject(std::move(config));
}

static IOSProject make_valid_project()
{
  return IOSProject(make_valid_mobile_project());
}

static void test_default_values_and_configuration()
{
  IOSProject defaults;
  assert(defaults.deployment_target() == "15.0");

  IOSProject project = make_valid_project();
  assert(project.config().name() == "Vix iOS Demo");
  assert(project.config().app_id() == "com.softadastra.vix.iosdemo");

  IOSProject &result = project.set_deployment_target("16.4");
  assert(&result == &project);
  assert(project.deployment_target() == "16.4");
}

static void test_validation()
{
  IOSProject project = make_valid_project();
  assert(project.validate().is_ok());

  project.set_deployment_target("12.4");
  assert(project.validate().is_failed());
  assert(project.validate().error_message() ==
         "iOS project deployment target must be iOS 13.0 or later");
}

static void test_invalid_identity_and_url_are_rejected_before_writing()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  IOSProject project = make_valid_project();
  project.config().set_app_id("com.example.invalid_package");
  Result<void> result = project.generate(directory);
  assert(result.is_failed());
  assert(result.error_message() ==
         "iOS project application id is not a valid bundle identifier");
  assert(!fs::exists(directory));

  project = make_valid_project();
  project.config().set_url("file:///tmp/application.html");
  result = project.generate(directory);
  assert(result.is_failed());
  assert(result.error_message() ==
         "iOS project URL must use http:// or https:// with a host");
  assert(!fs::exists(directory));
}

static void test_generates_complete_https_project()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  IOSProject project = make_valid_project();
  assert(project.generate(directory).is_ok());

  const fs::path root = directory / "Vix_iOS_Demo";
  assert(fs::is_regular_file(
      directory / "Vix_iOS_Demo.xcodeproj" / "project.pbxproj"));
  assert(fs::is_regular_file(root / "AppDelegate.swift"));
  assert(fs::is_regular_file(root / "SceneDelegate.swift"));
  assert(fs::is_regular_file(root / "ViewController.swift"));
  assert(fs::is_regular_file(root / "Info.plist"));
  assert(fs::is_regular_file(
      root / "Assets.xcassets" / "AppIcon.appiconset" / "Contents.json"));

  const std::string xcode = read_file(
      directory / "Vix_iOS_Demo.xcodeproj" / "project.pbxproj");
  const std::string plist = read_file(root / "Info.plist");
  const std::string controller = read_file(root / "ViewController.swift");

  assert(xcode.find("PRODUCT_BUNDLE_IDENTIFIER = \"com.softadastra.vix.iosdemo\"") !=
         std::string::npos);
  assert(xcode.find("MARKETING_VERSION = \"1.2.3\"") != std::string::npos);
  assert(xcode.find("IPHONEOS_DEPLOYMENT_TARGET = \"15.0\"") !=
         std::string::npos);
  assert(xcode.find("WebKit.framework in Frameworks") != std::string::npos);
  assert(plist.find("<string>Vix iOS Demo</string>") != std::string::npos);
  assert(plist.find("<string>1.2.3</string>") != std::string::npos);
  assert(plist.find("NSAppTransportSecurity") == std::string::npos);
  assert(plist.find("NSAllowsArbitraryLoads") == std::string::npos);
  assert(controller.find("import WebKit") != std::string::npos);
  assert(controller.find("WKWebView") != std::string::npos);
  assert(controller.find("javaScriptEnabled = true") != std::string::npos);
  assert(controller.find("https://example.test") != std::string::npos);
  assert(controller.find("openExternally(url)") != std::string::npos);
  assert(controller.find("mailto") != std::string::npos);
  assert(controller.find("tel") != std::string::npos);
  assert(controller.find("canOpenURL") != std::string::npos);
  assert(controller.find("allowsBackForwardNavigationGestures = true") !=
         std::string::npos);
  assert(controller.find("navigationDelegate = nil") != std::string::npos);
  assert(controller.find("URLSession") == std::string::npos);
  assert(controller.find("serverTrust") == std::string::npos);

  fs::remove_all(directory, error);
  assert(!error);
}

static void test_http_has_only_a_domain_ats_exception()
{
  const fs::path directory = test_directory();
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  IOSProject project = make_valid_project();
  project.config().set_url("http://example.test:8080/path?one=two");
  assert(project.generate(directory).is_ok());

  const std::string plist = read_file(
      directory / "Vix_iOS_Demo" / "Info.plist");
  assert(plist.find("NSAppTransportSecurity") != std::string::npos);
  assert(plist.find("NSExceptionDomains") != std::string::npos);
  assert(plist.find("<key>example.test</key>") != std::string::npos);
  assert(plist.find("NSExceptionAllowsInsecureHTTPLoads") !=
         std::string::npos);
  assert(plist.find("NSAllowsArbitraryLoads") == std::string::npos);

  fs::remove_all(directory, error);
  assert(!error);
}

static void test_png_icon_is_copied_and_default_remains_generable()
{
  const fs::path directory = test_directory();
  const fs::path source = fs::temp_directory_path() /
                          "vix_ui_ios_project_icon_source.png";
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);
  fs::remove(source, error);
  assert(!error);
  const std::string png{"\x89PNG\r\n\x1a\nVix", 12};
  write_file(source, png);

  IOSProject project = make_valid_project();
  assert(project.generate(directory).is_ok());
  const fs::path icon_root = directory / "Vix_iOS_Demo" /
                             "Assets.xcassets" / "AppIcon.appiconset";
  assert(!fs::exists(icon_root / "AppIcon.png"));

  project.config().set_icon_path(source.string());
  assert(project.generate(directory).is_ok());
  assert(read_file(icon_root / "AppIcon.png") == png);
  assert(read_file(icon_root / "Contents.json").find("AppIcon.png") !=
         std::string::npos);

  project.config().set_icon_path("");
  assert(project.generate(directory).is_ok());
  assert(!fs::exists(icon_root / "AppIcon.png"));

  fs::remove_all(directory, error);
  assert(!error);
  fs::remove(source, error);
  assert(!error);
}

static void test_generation_is_deterministic_and_supports_spaces()
{
  const fs::path directory = fs::temp_directory_path() /
                             "vix ui ios project output";
  std::error_code error;
  fs::remove_all(directory, error);
  assert(!error);

  IOSProject project = make_valid_project();
  assert(project.generate(directory).is_ok());
  const fs::path root = directory / "Vix_iOS_Demo";
  const std::string first_project = read_file(
      directory / "Vix_iOS_Demo.xcodeproj" / "project.pbxproj");
  const std::string first_plist = read_file(root / "Info.plist");
  const std::string first_controller = read_file(root / "ViewController.swift");

  assert(project.generate(directory).is_ok());
  assert(read_file(directory / "Vix_iOS_Demo.xcodeproj" / "project.pbxproj") ==
         first_project);
  assert(read_file(root / "Info.plist") == first_plist);
  assert(read_file(root / "ViewController.swift") == first_controller);

  fs::remove_all(directory, error);
  assert(!error);
}

int main()
{
  test_default_values_and_configuration();
  test_validation();
  test_invalid_identity_and_url_are_rejected_before_writing();
  test_generates_complete_https_project();
  test_http_has_only_a_domain_ats_exception();
  test_png_icon_is_copied_and_default_remains_generable();
  test_generation_is_deterministic_and_supports_spaces();

  std::cout << "ios_project_test: all tests passed\n";
  return 0;
}
