/**
 *
 *  @file 03_assets.cpp
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
//   vix run examples/03_assets.cpp

#include <iostream>
#include <vix/ui/assets/AssetManager.hpp>

int main()
{
  vix::ui::AssetManager assets("/assets");

  assets.add_stylesheet("app_css", "app.css");
  assets.add_script(
      "app_js",
      "app.js",
      vix::ui::AssetLoading::Deferred);
  assets.add_image("logo", "logo.png", "Vix UI logo");
  assets.add_font("inter", "fonts/inter.woff2");

  std::cout << "All assets:\n";
  std::cout << assets.render() << "\n\n";

  std::cout << "Stylesheets:\n";
  std::cout << assets.render_stylesheets() << "\n\n";

  std::cout << "Scripts:\n";
  std::cout << assets.render_scripts() << "\n";

  return 0;
}
