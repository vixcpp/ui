/**
 *
 *  @file desktop_shell_basic.cpp
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
#include <iostream>

#include <vix/ui/all.hpp>

int main()
{
  vix::ui::ShellConfig config = vix::ui::ShellConfig::make()
                                    .set_name("Vix Desktop App")
                                    .set_title("Vix Desktop Shell")
                                    .set_host("127.0.0.1")
                                    .set_port(8080)
                                    .set_width(1200)
                                    .set_height(800)
                                    .set_resizable(true)
                                    .set_fullscreen(false)
                                    .set_devtools(true)
                                    .set_start_server(false);

  vix::ui::AppShell shell = vix::ui::AppShell::make(config);

  const vix::ui::Result<void> result = shell.start();

  if (result.is_failed())
  {
    std::cerr << "failed to start desktop shell: "
              << result.error_message()
              << '\n';

    return 1;
  }

  std::cout << "desktop shell started\n";
  std::cout << "target url: " << shell.target_url() << '\n';
  std::cout << "status: " << shell.status() << '\n';

  return 0;
}
