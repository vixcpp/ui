/**
 *
 *  @file 06_fragment.cpp
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
//   vix run examples/06_fragment.cpp

#include <iostream>

#include <vix/ui/live/Fragment.hpp>

int main()
{
  vix::ui::Fragment stats =
      vix::ui::Fragment::make("dashboard-stats")
          .set_target("#stats-card")
          .set_attr("class", "stats-fragment")
          .set_html(
              "<div class=\"stat-card\">"
              "<strong>128</strong>"
              "<span>active users</span>"
              "</div>");

  std::cout << "Raw fragment:\n";
  std::cout << stats.render() << "\n\n";

  std::cout << "Wrapped fragment:\n";
  std::cout << stats.render_wrapped() << "\n";

  return 0;
}
