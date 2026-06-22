/**
 *
 *  @file 13_pwa_helpers.cpp
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

#include <vix/ui/pwa/PwaMeta.hpp>
#include <vix/ui/pwa/SafeArea.hpp>
#include <vix/ui/pwa/Viewport.hpp>

int main()
{
  vix::ui::Viewport viewport = vix::ui::Viewport::mobile_app();

  std::cout << "Viewport meta:\n";
  std::cout << viewport.render() << "\n\n";

  vix::ui::SafeArea safe_area = vix::ui::SafeArea::vertical();

  std::cout << "Safe-area CSS:\n";
  std::cout << safe_area.render() << "\n\n";

  vix::ui::PwaMeta meta = vix::ui::PwaMeta::mobile_app("Vix Mobile", "#111111");

  meta.set_color_scheme("light dark")
      .set_format_detection("telephone=no");

  std::cout << "PWA meta tags:\n";
  std::cout << meta.render() << "\n";

  return 0;
}
