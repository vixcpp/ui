/**
 *
 *  @file desktop_shell_with_server.cpp
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
#include <cstdlib>
#include <iostream>
#include <string>

#include <vix/ui/all.hpp>

namespace
{
  std::string server_command()
  {
    const char *value = std::getenv("VIX_UI_SERVER_CMD");

    if (value == nullptr || std::string(value).empty())
    {
      return {};
    }

    return std::string(value);
  }
}

int main()
{
  const std::string command = server_command();

  if (command.empty())
  {
    std::cerr << "missing server command\n";
    std::cerr << "usage:\n";
    std::cerr << "  VIX_UI_SERVER_CMD=\"<server command>\" ./ui_example_desktop_shell_with_server\n";
    return 1;
  }

  vix::ui::ShellConfig config = vix::ui::ShellConfig::make()
                                    .set_name("Vix UI Server App")
                                    .set_title("Vix UI Desktop Shell")
                                    .set_host("127.0.0.1")
                                    .set_port(8080)
                                    .set_width(1200)
                                    .set_height(800)
                                    .set_resizable(true)
                                    .set_fullscreen(false)
                                    .set_devtools(true)
                                    .set_start_server(true)
                                    .set_server_command(command);

  vix::ui::AppShell shell = vix::ui::AppShell::make(config);

  std::cout << "starting local server:\n";
  std::cout << "  " << command << '\n';
  std::cout << "opening target url:\n";
  std::cout << "  " << shell.target_url() << '\n';

  const vix::ui::Result<void> result = shell.start();

  if (result.is_failed())
  {
    std::cerr << "failed to start desktop shell: "
              << result.error_message()
              << '\n';

    return 1;
  }

  std::cout << "desktop shell closed\n";
  std::cout << "status: " << shell.status() << '\n';

  return 0;
}
