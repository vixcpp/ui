/**
 *
 *  @file desktop_shell_readiness.cpp
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
#include <chrono>
#include <iostream>
#include <string>

#include <vix/ui/all.hpp>

#include "support/Env.hpp"

int main()
{
  const std::string command = vix::ui::examples::env_or_empty("VIX_UI_SERVER_CMD");
  const std::string readiness = vix::ui::examples::env_or_empty("VIX_UI_READINESS_URL");

  if (command.empty())
  {
    std::cerr << "missing server command\n";
    std::cerr << "usage:\n";
    std::cerr << "  VIX_UI_SERVER_CMD=\"<server command>\" ";
    std::cerr << "VIX_UI_READINESS_URL=\"http://127.0.0.1:8080/health\" ";
    std::cerr << "./ui_example_desktop_shell_readiness\n";
    return 1;
  }

  vix::ui::ShellConfig config = vix::ui::ShellConfig::make()
                                    .set_name("Vix UI Readiness App")
                                    .set_title("Vix UI Desktop Readiness")
                                    .set_app_id("com.vix.ui.readiness")
                                    .set_app_version("0.4.0")
                                    .set_vendor("Vix.cpp")
                                    .set_host("127.0.0.1")
                                    .set_port(8080)
                                    .set_width(1200)
                                    .set_height(800)
                                    .set_resizable(true)
                                    .set_fullscreen(false)
                                    .set_devtools(true)
                                    .set_start_server(true)
                                    .set_wait_for_server(true)
                                    .set_startup_timeout(std::chrono::milliseconds(5000))
                                    .set_server_command(command);

  if (!readiness.empty())
  {
    config.set_readiness_url(readiness);
  }

  vix::ui::AppShell shell = vix::ui::AppShell::make(config);

  std::cout << "starting local server:\n";
  std::cout << "  " << command << "\n";
  std::cout << "waiting for readiness URL:\n";
  std::cout << "  " << config.effective_readiness_url() << "\n";
  std::cout << "opening target URL:\n";
  std::cout << "  " << shell.target_url() << "\n";

  const vix::ui::Result<void> result = shell.start();

  if (result.is_failed())
  {
    std::cerr << "failed to start desktop shell: "
              << result.error_message()
              << "\n";

    return 1;
  }

  std::cout << "desktop shell closed\n";
  std::cout << "status: " << shell.status() << "\n";

  return 0;
}
