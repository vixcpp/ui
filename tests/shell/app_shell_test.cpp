/**
 *
 *  @file app_shell_test.cpp
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
#include <string>

#include <vix/ui/platform/Platform.hpp>
#include <vix/ui/shell/AppShell.hpp>
#include <vix/ui/shell/ShellConfig.hpp>
#include <vix/ui/support/Result.hpp>

using namespace vix::ui;

static void test_platform_kind_to_string()
{
  assert(to_string(PlatformKind::Unknown) == "unknown");
  assert(to_string(PlatformKind::Linux) == "linux");
  assert(to_string(PlatformKind::Windows) == "windows");
  assert(to_string(PlatformKind::MacOS) == "macos");
  assert(to_string(PlatformKind::Android) == "android");
  assert(to_string(PlatformKind::IOS) == "ios");
  assert(to_string(PlatformKind::Web) == "web");
}

static void test_platform_family_to_string()
{
  assert(to_string(PlatformFamily::Unknown) == "unknown");
  assert(to_string(PlatformFamily::Desktop) == "desktop");
  assert(to_string(PlatformFamily::Mobile) == "mobile");
  assert(to_string(PlatformFamily::Web) == "web");
}

static void test_platform_desktop()
{
  Platform linux = Platform::linux();
  Platform windows = Platform::windows();
  Platform macos = Platform::macos();

  assert(linux.known());
  assert(windows.known());
  assert(macos.known());

  assert(linux.desktop());
  assert(windows.desktop());
  assert(macos.desktop());

  assert(linux.family() == PlatformFamily::Desktop);
  assert(windows.family() == PlatformFamily::Desktop);
  assert(macos.family() == PlatformFamily::Desktop);

  assert(linux.is_linux());
  assert(windows.is_windows());
  assert(macos.is_macos());
}

static void test_platform_mobile()
{
  Platform android = Platform::android();
  Platform ios = Platform::ios();

  assert(android.known());
  assert(ios.known());

  assert(android.mobile());
  assert(ios.mobile());

  assert(android.family() == PlatformFamily::Mobile);
  assert(ios.family() == PlatformFamily::Mobile);

  assert(android.is_android());
  assert(ios.is_ios());
}

static void test_platform_web()
{
  Platform web = Platform::web();

  assert(web.known());
  assert(web.web_platform());
  assert(web.family() == PlatformFamily::Web);
  assert(web.kind() == PlatformKind::Web);
  assert(web.name() == "web");
}

static void test_platform_unknown()
{
  Platform platform;

  assert(platform.unknown());
  assert(!platform.known());
  assert(!platform.desktop());
  assert(!platform.mobile());
  assert(!platform.web_platform());
  assert(platform.family() == PlatformFamily::Unknown);
  assert(platform.name() == "unknown");
}

static void test_shell_config_defaults()
{
  ShellConfig config;

  assert(config.name() == "Vix UI App");
  assert(config.title() == "Vix UI App");
  assert(config.url().empty());
  assert(config.host() == "127.0.0.1");
  assert(config.port() == 8080);
  assert(config.width() == 1024);
  assert(config.height() == 768);
  assert(config.resizable());
  assert(!config.fullscreen());
  assert(!config.devtools());
  assert(config.start_server());

  assert(config.has_name());
  assert(config.has_title());
  assert(!config.has_url());

  assert(config.local_url() == "http://127.0.0.1:8080");
  assert(config.effective_url() == "http://127.0.0.1:8080");
}

static void test_shell_config_make()
{
  ShellConfig config = ShellConfig::make();

  assert(config.name() == "Vix UI App");
  assert(config.title() == "Vix UI App");
  assert(config.local_url() == "http://127.0.0.1:8080");
}

static void test_shell_config_setters()
{
  ShellConfig config;

  config.set_name("Pico Desktop")
      .set_title("Pico Dashboard")
      .set_url("http://localhost:3000")
      .set_host("0.0.0.0")
      .set_port(9090)
      .set_platform(Platform::web())
      .set_width(1280)
      .set_height(720)
      .set_resizable(false)
      .set_fullscreen(true)
      .set_devtools(true)
      .set_start_server(false);

  assert(config.name() == "Pico Desktop");
  assert(config.title() == "Pico Dashboard");
  assert(config.url() == "http://localhost:3000");
  assert(config.host() == "0.0.0.0");
  assert(config.port() == 9090);
  assert(config.platform().web_platform());
  assert(config.width() == 1280);
  assert(config.height() == 720);
  assert(!config.resizable());
  assert(config.fullscreen());
  assert(config.devtools());
  assert(!config.start_server());

  assert(config.has_name());
  assert(config.has_title());
  assert(config.has_url());

  assert(config.local_url() == "http://0.0.0.0:9090");
  assert(config.effective_url() == "http://localhost:3000");
}

static void test_shell_config_effective_url_uses_local_url_when_no_url()
{
  ShellConfig config;

  config.set_host("127.0.0.1");
  config.set_port(7000);

  assert(config.url().empty());
  assert(config.local_url() == "http://127.0.0.1:7000");
  assert(config.effective_url() == "http://127.0.0.1:7000");
}

static void test_app_shell_default()
{
  AppShell shell;

  assert(shell.stopped());
  assert(!shell.running());
  assert(shell.status() == "stopped");
  assert(shell.target_url() == "http://127.0.0.1:8080");
}

static void test_app_shell_make()
{
  ShellConfig config;
  config.set_name("Vix Admin");
  config.set_title("Admin Dashboard");
  config.set_url("http://localhost:8080");

  AppShell shell = AppShell::make(config);

  assert(shell.config().name() == "Vix Admin");
  assert(shell.config().title() == "Admin Dashboard");
  assert(shell.target_url() == "http://localhost:8080");
  assert(shell.stopped());
}

static void test_app_shell_set_config()
{
  AppShell shell;

  ShellConfig config;
  config.set_name("New App");
  config.set_title("New Title");
  config.set_url("http://localhost:9090");

  shell.set_config(config);

  assert(shell.config().name() == "New App");
  assert(shell.config().title() == "New Title");
  assert(shell.target_url() == "http://localhost:9090");
}

static void test_app_shell_validate_success()
{
  ShellConfig config;
  config.set_name("Vix App");
  config.set_title("Vix App");
  config.set_width(800);
  config.set_height(600);

  AppShell shell(config);

  Result<void> result = shell.validate();

  assert(result.is_ok());
  assert(!result.is_failed());
  assert(static_cast<bool>(result));
}

static void test_app_shell_validate_missing_name()
{
  ShellConfig config;
  config.set_name("");
  config.set_title("Vix App");

  AppShell shell(config);

  Result<void> result = shell.validate();

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ShellError);
  assert(!result.error_message().empty());
}

static void test_app_shell_validate_missing_title()
{
  ShellConfig config;
  config.set_name("Vix App");
  config.set_title("");

  AppShell shell(config);

  Result<void> result = shell.validate();

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ShellError);
  assert(!result.error_message().empty());
}

static void test_app_shell_validate_invalid_width()
{
  ShellConfig config;
  config.set_width(0);

  AppShell shell(config);

  Result<void> result = shell.validate();

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ShellError);
  assert(!result.error_message().empty());
}

static void test_app_shell_validate_invalid_height()
{
  ShellConfig config;
  config.set_height(0);

  AppShell shell(config);

  Result<void> result = shell.validate();

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ShellError);
  assert(!result.error_message().empty());
}

static void test_app_shell_start()
{
  AppShell shell;

  Result<void> result = shell.start();

  assert(result.is_ok());
  assert(shell.running());
  assert(!shell.stopped());
  assert(shell.status() == "running");
}

static void test_app_shell_start_invalid_config()
{
  ShellConfig config;
  config.set_name("");

  AppShell shell(config);

  Result<void> result = shell.start();

  assert(result.is_failed());
  assert(!shell.running());
  assert(shell.stopped());
}

static void test_app_shell_stop()
{
  AppShell shell;

  Result<void> start_result = shell.start();
  assert(start_result.is_ok());
  assert(shell.running());

  Result<void> stop_result = shell.stop();

  assert(stop_result.is_ok());
  assert(shell.stopped());
  assert(!shell.running());
  assert(shell.status() == "stopped");
}

static void test_app_shell_restart()
{
  AppShell shell;

  Result<void> result = shell.restart();

  assert(result.is_ok());
  assert(shell.running());
  assert(shell.status() == "running");
}

static void test_app_shell_platform()
{
  ShellConfig config;
  config.set_platform(Platform::web());

  AppShell shell(config);

  assert(shell.platform().web_platform());
  assert(shell.platform().name() == "web");
}

int main()
{
  test_platform_kind_to_string();
  test_platform_family_to_string();
  test_platform_desktop();
  test_platform_mobile();
  test_platform_web();
  test_platform_unknown();

  test_shell_config_defaults();
  test_shell_config_make();
  test_shell_config_setters();
  test_shell_config_effective_url_uses_local_url_when_no_url();

  test_app_shell_default();
  test_app_shell_make();
  test_app_shell_set_config();
  test_app_shell_validate_success();
  test_app_shell_validate_missing_name();
  test_app_shell_validate_missing_title();
  test_app_shell_validate_invalid_width();
  test_app_shell_validate_invalid_height();
  test_app_shell_start();
  test_app_shell_start_invalid_config();
  test_app_shell_stop();
  test_app_shell_restart();
  test_app_shell_platform();

  std::cout << "[OK] ui app shell tests passed\n";
  return 0;
}
