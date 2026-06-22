/**
 *
 *  @file 12_asset_manifest.cpp
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

#include <vix/ui/assets/AssetManager.hpp>
#include <vix/ui/assets/AssetMap.hpp>
#include <vix/ui/assets/AssetMode.hpp>

int main()
{
  vix::ui::AssetMap manifest;

  manifest.set("css/app.css", "css/app.8fd21c.css");
  manifest.set("css/dashboard.css", "css/dashboard.4ac922.css");
  manifest.set("js/app.js", "js/app.91af03.js");

  vix::ui::AssetManager assets("/assets");

  assets.set_mode(vix::ui::AssetMode::Production);
  assets.set_manifest(manifest);

  assets.add_stylesheet_to("dashboard", "app_css", "css/app.css");
  assets.add_stylesheet_to("dashboard", "dashboard_css", "css/dashboard.css");
  assets.add_module_script_to("dashboard", "app_js", "js/app.js");

  std::cout << "Production assets resolved from manifest:\n";
  std::cout << assets.render_group("dashboard") << "\n\n";

  std::cout << "Direct path lookup:\n";
  std::cout << assets.path("css/app.css") << "\n";
  std::cout << assets.path("js/app.js") << "\n";
  std::cout << assets.path("missing.css") << "\n";

  return 0;
}
