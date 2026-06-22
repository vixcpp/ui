/**
 *
 *  @file 14_web_app_manifest.cpp
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

#include <vix/ui/pwa/WebAppManifest.hpp>

int main()
{
  vix::ui::WebAppManifest manifest = vix::ui::WebAppManifest::app("Vix Mobile", "Vix");

  manifest.set_description("A mobile-ready Vix UI application")
      .set_start_url("/")
      .set_scope("/")
      .set_id("com.vix.mobile")
      .set_lang("en")
      .set_display(vix::ui::WebAppDisplayMode::Standalone)
      .set_orientation(vix::ui::WebAppOrientation::Portrait)
      .set_background_color("#ffffff")
      .set_theme_color("#111111");

  manifest.add_icon(
      "/icons/icon-192.png",
      "192x192",
      "image/png");

  manifest.add_icon(
      "/icons/icon-512.png",
      "512x512",
      "image/png",
      "any maskable");

  std::cout << manifest.render() << "\n";

  return 0;
}
