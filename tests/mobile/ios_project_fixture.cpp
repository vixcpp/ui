/**
 *
 *  @file ios_project_fixture.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */
#include <filesystem>
#include <iostream>
#include <string>
#include <utility>

#include <vix/ui/mobile/IOSProject.hpp>

int main(int argc, char **argv)
{
  if (argc != 2 &&
      (argc != 3 || std::string(argv[2]) != "--build"))
  {
    std::cerr << "Usage: ui_ios_project_fixture <output-directory> [--build]\n";
    return 2;
  }

  vix::ui::MobileConfig config;
  config.set_name("Vix iOS CI")
      .set_app_id("com.vixcpp.iosci")
      .set_version("1.0.0")
      .set_url("https://example.com");

  vix::ui::IOSProject project(
      vix::ui::MobileProject(std::move(config)));
  const vix::ui::Result<void> result =
      project.generate(std::filesystem::path(argv[1]));
  if (result.is_failed())
  {
    std::cerr << result.error_message() << '\n';
    return 1;
  }

  if (argc == 3)
  {
    const vix::ui::Result<std::filesystem::path> built =
        project.build(std::filesystem::path(argv[1]));
    if (built.is_failed())
    {
      std::cerr << built.error_message() << '\n';
      return 1;
    }
  }

  return 0;
}
