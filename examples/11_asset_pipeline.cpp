/**
 *
 *  @file 11_asset_pipeline.cpp
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
#include <vix/ui/assets/AssetMode.hpp>

int main()
{
  vix::ui::AssetManager assets("/assets");

  assets.set_mode(vix::ui::AssetMode::Development);

  assets.add_stylesheet_to("app", "app_css", "css/app.css");
  assets.add_stylesheet_to("app", "dashboard_css", "css/dashboard.css");

  assets.add_script_to(
      "app",
      "runtime_js",
      "js/runtime.js",
      vix::ui::AssetLoading::Deferred);

  assets.add_module_script_to("app", "app_js", "js/app.js");

  std::cout << "Development assets:\n";
  std::cout << assets.render_group("app") << "\n\n";

  assets.clear();

  assets.set_mode(vix::ui::AssetMode::Production);
  assets.set_version("0.7.0");

  assets.add_stylesheet_to("app", "app_css", "css/app.css");
  assets.add_module_script_to("app", "app_js", "js/app.js");

  std::cout << "Production assets with versioning:\n";
  std::cout << assets.render_group("app") << "\n";

  return 0;
}
