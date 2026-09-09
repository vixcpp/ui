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

  assert(project.generate(directory).is_ok());
  assert(read_file(directory / "settings.gradle") == first_settings);
  assert(read_file(directory / "build.gradle") == first_root_build);
  assert(read_file(directory / "gradle.properties") == first_properties);
  assert(read_file(directory / "app" / "build.gradle") == first_app_build);

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

  std::cout << "android_project_test: all tests passed\n";
  return 0;
}
