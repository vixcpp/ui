/**
 *
 *  @file mobile_config_test.cpp
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

#include <vix/ui/mobile/MobileConfig.hpp>

using namespace vix::ui;

static void test_default_config()
{
  MobileConfig config;

  assert(config.name() == "Vix Mobile App");
  assert(config.app_id().empty());
  assert(config.version() == "0.1.0");
  assert(config.url() == "https://example.com");
  assert(config.icon_path().empty());

  assert(config.has_name());
  assert(!config.has_app_id());
  assert(config.has_version());
  assert(config.has_url());
  assert(!config.has_icon_path());
}

static void test_make()
{
  MobileConfig config = MobileConfig::make();

  assert(config.name() == "Vix Mobile App");
  assert(config.version() == "0.1.0");
  assert(config.url() == "https://example.com");
}

static void test_setters_and_chaining()
{
  MobileConfig config;

  MobileConfig &result = config.set_name("Vix Mobile Demo")
                             .set_app_id("com.softadastra.vix.mobile.demo")
                             .set_version("1.2.3")
                             .set_url("https://example.test")
                             .set_icon_path("assets/mobile-icon.png");

  assert(&result == &config);
  assert(config.name() == "Vix Mobile Demo");
  assert(config.app_id() == "com.softadastra.vix.mobile.demo");
  assert(config.version() == "1.2.3");
  assert(config.url() == "https://example.test");
  assert(config.icon_path() == "assets/mobile-icon.png");
  assert(config.has_app_id());
  assert(config.has_icon_path());
}

static void test_validation()
{
  MobileConfig config;

  Result<void> result = config.validate();
  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(result.error_message() == "mobile application id must not be empty");

  config.set_app_id("com.softadastra.vix.mobile.demo");
  result = config.validate();
  assert(result.is_ok());

  config.set_name("");
  result = config.validate();
  assert(result.is_failed());
  assert(result.error_message() == "mobile application name must not be empty");

  config.set_name("Vix Mobile Demo").set_version("");
  result = config.validate();
  assert(result.is_failed());
  assert(result.error_message() == "mobile application version must not be empty");

  config.set_version("1.2.3").set_url("");
  result = config.validate();
  assert(result.is_failed());
  assert(result.error_message() == "mobile application URL must not be empty");
}

int main()
{
  test_default_config();
  test_make();
  test_setters_and_chaining();
  test_validation();

  std::cout << "mobile_config_test: all tests passed\n";
  return 0;
}
