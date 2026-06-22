/**
 *
 *  @file 08_flash_and_toast.cpp
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
//   vix run examples/08_flash_and_toast.cpp

#include <chrono>
#include <iostream>

#include <vix/ui/live/FlashMessage.hpp>
#include <vix/ui/live/Toast.hpp>

int main()
{
  vix::ui::FlashMessage flash =
      vix::ui::FlashMessage::success("Profile updated successfully.")
          .set_title("Saved")
          .set_dismissible(true);

  vix::ui::Toast toast =
      vix::ui::Toast::info("Background sync completed.")
          .set_title("Sync")
          .set_position(vix::ui::ToastPosition::BottomRight)
          .set_timeout(std::chrono::milliseconds(3000));

  std::cout << "Flash message:\n";
  std::cout << flash.render() << "\n\n";

  std::cout << "Toast:\n";
  std::cout << toast.render() << "\n";

  return 0;
}
