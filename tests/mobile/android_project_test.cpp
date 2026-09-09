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
#include <iostream>
#include <utility>

#include <vix/ui/mobile/AndroidProject.hpp>

using namespace vix::ui;

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

  std::cout << "android_project_test: all tests passed\n";
  return 0;
}
