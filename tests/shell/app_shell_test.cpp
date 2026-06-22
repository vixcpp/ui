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
#include <chrono>
#include <iostream>
#include <string>
#include <utility>

#include <vix/ui/platform/Platform.hpp>
#include <vix/ui/shell/AppShell.hpp>
#include <vix/ui/shell/ShellConfig.hpp>
#include <vix/ui/support/Result.hpp>

using namespace vix::ui;

static ShellConfig make_test_shell_config(
    std::string name = "Vix UI Test",
    std::string title = "Vix UI Test",
    std::string url = "http://127.0.0.1:8080")
{
  ShellConfig config;

  config.set_name(std::move(name))
      .set_title(std::move(title))
      .set_url(std::move(url))
      .set_platform(Platform::web_platform_descriptor());

  return config;
}

static AppShell make_test_shell()
{
  return AppShell(make_test_shell_config());
}

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
  Platform linux = Platform::linux_platform();
  Platform windows = Platform::windows_platform();
  Platform macos = Platform::macos_platform();

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
  Platform android = Platform::android_platform();
  Platform ios = Platform::ios_platform();

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
  Platform web = Platform::web_platform_descriptor();

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
      .set_platform(Platform::web_platform_descriptor())
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

static void test_shell_config_metadata()
{
  ShellConfig config;

  assert(config.app_id().empty());
  assert(config.app_version().empty());
  assert(config.vendor().empty());

  assert(!config.has_app_id());
  assert(!config.has_app_version());
  assert(!config.has_vendor());

  config.set_app_id("com.softadastra.vix.demo")
      .set_app_version("0.4.0")
      .set_vendor("Softadastra");

  assert(config.app_id() == "com.softadastra.vix.demo");
  assert(config.app_version() == "0.4.0");
  assert(config.vendor() == "Softadastra");

  assert(config.has_app_id());
  assert(config.has_app_version());
  assert(config.has_vendor());
}

static void test_shell_config_icon_path()
{
  ShellConfig config;

  assert(config.icon_path().empty());
  assert(!config.has_icon_path());

  config.set_icon_path("assets/icon.png");

  assert(config.icon_path() == "assets/icon.png");
  assert(config.has_icon_path());
}

static void test_shell_config_readiness_url()
{
  ShellConfig config;

  assert(config.readiness_url().empty());
  assert(!config.has_readiness_url());

  config.set_readiness_url("http://127.0.0.1:8080/health");

  assert(config.readiness_url() == "http://127.0.0.1:8080/health");
  assert(config.has_readiness_url());
  assert(config.effective_readiness_url() == "http://127.0.0.1:8080/health");
}

static void test_shell_config_effective_readiness_url_uses_effective_url()
{
  ShellConfig config;

  config.set_url("http://localhost:3000");

  assert(config.readiness_url().empty());
  assert(config.effective_url() == "http://localhost:3000");
  assert(config.effective_readiness_url() == "http://localhost:3000");
}

static void test_shell_config_wait_for_server_flag()
{
  ShellConfig config;

  assert(config.wait_for_server());

  config.set_wait_for_server(false);

  assert(!config.wait_for_server());

  config.set_wait_for_server(true);

  assert(config.wait_for_server());
}

static void test_shell_config_startup_timeout()
{
  ShellConfig config;

  assert(config.startup_timeout() == std::chrono::milliseconds(5000));

  config.set_startup_timeout(std::chrono::milliseconds(2500));

  assert(config.startup_timeout() == std::chrono::milliseconds(2500));
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
  ShellConfig config =
      make_test_shell_config(
          "Vix Admin",
          "Admin Dashboard",
          "http://localhost:8080");

  AppShell shell = AppShell::make(config);

  assert(shell.config().name() == "Vix Admin");
  assert(shell.config().title() == "Admin Dashboard");
  assert(shell.target_url() == "http://localhost:8080");
  assert(shell.stopped());
}

static void test_app_shell_set_config()
{
  AppShell shell;

  ShellConfig config =
      make_test_shell_config(
          "New App",
          "New Title",
          "http://localhost:9090");

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
  AppShell shell = make_test_shell();

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
  config.set_platform(Platform::web_platform_descriptor());

  AppShell shell(config);

  Result<void> result = shell.start();

  assert(result.is_failed());
  assert(!shell.running());
  assert(shell.stopped());
}

static void test_app_shell_stop()
{
  AppShell shell = make_test_shell();

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
  AppShell shell = make_test_shell();

  Result<void> result = shell.restart();

  assert(result.is_ok());
  assert(shell.running());
  assert(shell.status() == "running");
}

static void test_app_shell_platform()
{
  ShellConfig config;
  config.set_platform(Platform::web_platform_descriptor());

  AppShell shell(config);

  assert(shell.platform().web_platform());
  assert(shell.platform().name() == "web");
}

static void test_app_shell_start_is_idempotent()
{
  AppShell shell = make_test_shell();

  Result<void> first = shell.start();
  Result<void> second = shell.start();

  assert(first.is_ok());
  assert(second.is_ok());
  assert(shell.running());
  assert(shell.status() == "running");
}

static void test_app_shell_stop_is_idempotent()
{
  AppShell shell = make_test_shell();

  Result<void> first = shell.stop();
  Result<void> second = shell.stop();

  assert(first.is_ok());
  assert(second.is_ok());
  assert(shell.stopped());
  assert(shell.status() == "stopped");
}

static void test_app_shell_copy_constructor()
{
  ShellConfig config =
      make_test_shell_config(
          "Copied App",
          "Copied Title",
          "http://localhost:5050");

  AppShell original(config);
  AppShell copy(original);

  assert(copy.config().name() == "Copied App");
  assert(copy.config().title() == "Copied Title");
  assert(copy.target_url() == "http://localhost:5050");
  assert(copy.stopped());

  Result<void> result = copy.start();

  assert(result.is_ok());
  assert(copy.running());
  assert(original.stopped());
}

static void test_app_shell_copy_assignment()
{
  ShellConfig config =
      make_test_shell_config(
          "Assigned App",
          "Assigned Title",
          "http://localhost:6060");

  AppShell source(config);
  AppShell target;

  target = source;

  assert(target.config().name() == "Assigned App");
  assert(target.config().title() == "Assigned Title");
  assert(target.target_url() == "http://localhost:6060");
  assert(target.stopped());

  Result<void> result = target.start();

  assert(result.is_ok());
  assert(target.running());
  assert(source.stopped());
}

static void test_app_shell_move_constructor()
{
  ShellConfig config =
      make_test_shell_config(
          "Moved App",
          "Moved Title",
          "http://localhost:7070");

  AppShell original(config);
  AppShell moved(std::move(original));

  assert(moved.config().name() == "Moved App");
  assert(moved.config().title() == "Moved Title");
  assert(moved.target_url() == "http://localhost:7070");

  Result<void> result = moved.start();

  assert(result.is_ok());
  assert(moved.running());
}

static void test_app_shell_move_assignment()
{
  ShellConfig config =
      make_test_shell_config(
          "Move Assigned App",
          "Move Assigned Title",
          "http://localhost:8088");

  AppShell source(config);
  AppShell target;

  target = std::move(source);

  assert(target.config().name() == "Move Assigned App");
  assert(target.config().title() == "Move Assigned Title");
  assert(target.target_url() == "http://localhost:8088");

  Result<void> result = target.start();

  assert(result.is_ok());
  assert(target.running());
}

static void test_app_shell_set_config_when_stopped_recreates_backend()
{
  AppShell shell;

  ShellConfig config =
      make_test_shell_config(
          "Stopped Config App",
          "Stopped Config Title",
          "http://localhost:9091");

  shell.set_config(config);

  assert(shell.stopped());
  assert(shell.config().name() == "Stopped Config App");
  assert(shell.config().title() == "Stopped Config Title");
  assert(shell.target_url() == "http://localhost:9091");

  Result<void> result = shell.start();

  assert(result.is_ok());
  assert(shell.running());
}

static void test_app_shell_set_config_while_running_keeps_shell_running()
{
  AppShell shell = make_test_shell();

  Result<void> start_result = shell.start();

  assert(start_result.is_ok());
  assert(shell.running());

  ShellConfig config =
      make_test_shell_config(
          "Running Config App",
          "Running Config Title",
          "http://localhost:9092");

  shell.set_config(config);

  assert(shell.running());
  assert(shell.config().name() == "Running Config App");
  assert(shell.config().title() == "Running Config Title");
  assert(shell.target_url() == "http://localhost:9092");
}

static void test_shell_config_server_command()
{
  ShellConfig config;

  assert(config.server_command().empty());
  assert(config.server_working_directory().empty());
  assert(!config.has_server_command());
  assert(!config.has_server_working_directory());

  config.set_server_command("vix run main.cpp --port 8080");
  config.set_server_working_directory("/tmp/app");

  assert(config.server_command() == "vix run main.cpp --port 8080");
  assert(config.server_working_directory() == "/tmp/app");
  assert(config.has_server_command());
  assert(config.has_server_working_directory());
}

static void test_app_shell_ignores_server_command_when_start_server_disabled()
{
  ShellConfig config =
      make_test_shell_config(
          "No Server App",
          "No Server Title",
          "http://localhost:9093");

  config.set_start_server(false)
      .set_server_command("this-command-should-not-run");

  AppShell shell(config);

  Result<void> start_result = shell.start();

  assert(start_result.is_ok());
  assert(shell.running());

  Result<void> stop_result = shell.stop();

  assert(stop_result.is_ok());
  assert(shell.stopped());
}

static void test_app_shell_starts_and_stops_server_process()
{
#if defined(__unix__) || defined(__APPLE__)
  ShellConfig config =
      make_test_shell_config(
          "Server Process App",
          "Server Process Title",
          "http://127.0.0.1:9094");

  config.set_start_server(true)
      .set_wait_for_server(false)
      .set_server_command("sleep 30");

  AppShell shell(config);

  Result<void> start_result = shell.start();

  assert(start_result.is_ok());
  assert(shell.running());

  Result<void> stop_result = shell.stop();

  assert(stop_result.is_ok());
  assert(shell.stopped());
#else
  ShellConfig config =
      make_test_shell_config(
          "Server Process App",
          "Server Process Title",
          "http://127.0.0.1:9094");

  config.set_start_server(false)
      .set_server_command("sleep 30");

  AppShell shell(config);

  Result<void> result = shell.start();

  assert(result.is_ok());
  assert(shell.running());

  (void)shell.stop();
#endif
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
  test_shell_config_metadata();
  test_shell_config_icon_path();
  test_shell_config_readiness_url();
  test_shell_config_effective_readiness_url_uses_effective_url();
  test_shell_config_wait_for_server_flag();
  test_shell_config_startup_timeout();
  test_shell_config_server_command();
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
  test_app_shell_start_is_idempotent();
  test_app_shell_stop_is_idempotent();
  test_app_shell_copy_constructor();
  test_app_shell_copy_assignment();
  test_app_shell_move_constructor();
  test_app_shell_move_assignment();
  test_app_shell_set_config_when_stopped_recreates_backend();
  test_app_shell_set_config_while_running_keeps_shell_running();
  test_app_shell_ignores_server_command_when_start_server_disabled();
  test_app_shell_starts_and_stops_server_process();

  std::cout << "[OK] ui app shell tests passed\n";
  return 0;
}
