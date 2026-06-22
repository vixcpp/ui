/**
 *
 *  @file 05_shell_config_v04.cpp
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
// Run:
//   vix run examples/05_shell_config_v04.cpp

#include <chrono>
#include <iostream>

#include <vix/ui/all.hpp>

int main()
{
  vix::ui::ShellConfig config = vix::ui::ShellConfig::make()
                                    .set_name("Vix UI Demo")
                                    .set_title("Vix UI v0.4.0 Desktop Demo")
                                    .set_app_id("com.vix.ui.demo")
                                    .set_app_version("0.4.0")
                                    .set_vendor("Vix.cpp")
                                    .set_icon_path("assets/icon.png")
                                    .set_host("127.0.0.1")
                                    .set_port(8080)
                                    .set_readiness_url("http://127.0.0.1:8080/health")
                                    .set_startup_timeout(std::chrono::milliseconds(5000))
                                    .set_wait_for_server(true)
                                    .set_width(1200)
                                    .set_height(800)
                                    .set_resizable(true)
                                    .set_devtools(true);

  std::cout << "name: " << config.name() << "\n";
  std::cout << "title: " << config.title() << "\n";
  std::cout << "app id: " << config.app_id() << "\n";
  std::cout << "version: " << config.app_version() << "\n";
  std::cout << "vendor: " << config.vendor() << "\n";
  std::cout << "icon: " << config.icon_path() << "\n";
  std::cout << "target url: " << config.effective_url() << "\n";
  std::cout << "readiness url: " << config.effective_readiness_url() << "\n";
  std::cout << "startup timeout: "
            << config.startup_timeout().count()
            << "ms\n";
  std::cout << "wait for server: "
            << (config.wait_for_server() ? "yes" : "no")
            << "\n";

  return 0;
}
