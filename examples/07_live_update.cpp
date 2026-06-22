/**
 *
 *  @file 07_live_update.cpp
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
//   vix run examples/07_live_update.cpp

#include <iostream>

#include <vix/ui/live/Fragment.hpp>
#include <vix/ui/live/LiveUpdate.hpp>

int main()
{
  vix::ui::Fragment row =
      vix::ui::Fragment::make("user-row")
          .set_html(
              "<tr id=\"user-42\">"
              "<td>Gaspard</td>"
              "<td>online</td>"
              "</tr>");

  vix::ui::LiveUpdate update = vix::ui::LiveUpdate::replace("#user-42", row)
                                   .set_event("users.updated")
                                   .set_id("update-42");

  std::cout << "Rendered HTML:\n";
  std::cout << update.render() << "\n\n";

  std::cout << "WebSocket-friendly payload:\n";
  std::cout << update.to_json() << "\n\n";

  vix::ui::LiveUpdate remove = vix::ui::LiveUpdate::remove("#toast-old")
                                   .set_event("toast.remove");

  std::cout << "Remove payload:\n";
  std::cout << remove.to_json() << "\n";

  return 0;
}
