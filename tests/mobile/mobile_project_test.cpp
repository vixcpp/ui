/**
 *
 *  @file mobile_project_test.cpp
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

#include <vix/ui/mobile/MobileProject.hpp>

using namespace vix::ui;

static MobileConfig make_valid_config()
{
  MobileConfig config;
  config.set_name("Vix Mobile Demo")
      .set_app_id("com.softadastra.vix.mobile.demo")
      .set_version("1.2.3")
      .set_url("https://example.test")
      .set_icon_path("assets/mobile-icon.png");
  return config;
}

static void test_construct_with_config()
{
  MobileProject project(make_valid_config());

  assert(project.config().name() == "Vix Mobile Demo");
  assert(project.config().app_id() == "com.softadastra.vix.mobile.demo");
  assert(project.config().version() == "1.2.3");
  assert(project.config().url() == "https://example.test");
  assert(project.config().icon_path() == "assets/mobile-icon.png");
}

static void test_configuration_access()
{
  MobileProject project;

  project.config().set_app_id("com.softadastra.vix.mobile.demo");

  const MobileProject &const_project = project;
  assert(const_project.config().app_id() == "com.softadastra.vix.mobile.demo");
}

static void test_validation_succeeds()
{
  MobileProject project(make_valid_config());

  Result<void> result = project.validate();
  assert(result.is_ok());
}

static void test_invalid_configuration_is_propagated()
{
  MobileConfig config = make_valid_config();
  config.set_app_id("");

  MobileProject project(std::move(config));
  Result<void> result = project.validate();

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(result.error_message() == "mobile application id must not be empty");
}

int main()
{
  test_construct_with_config();
  test_configuration_access();
  test_validation_succeeds();
  test_invalid_configuration_is_propagated();

  std::cout << "mobile_project_test: all tests passed\n";
  return 0;
}
