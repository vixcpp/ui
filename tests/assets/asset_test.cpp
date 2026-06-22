/**
 *
 *  @file asset_test.cpp
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
#include <cassert>
#include <iostream>
#include <map>
#include <string>

#include <vix/ui/assets/Asset.hpp>
#include <vix/ui/assets/AssetManager.hpp>
#include <vix/ui/assets/AssetManifest.hpp>
#include <vix/ui/assets/AssetMap.hpp>
#include <vix/ui/assets/AssetMode.hpp>
#include <vix/ui/support/Error.hpp>

using namespace vix::ui;

static void test_asset_type_to_string()
{
  assert(to_string(AssetType::Unknown) == "unknown");
  assert(to_string(AssetType::Stylesheet) == "stylesheet");
  assert(to_string(AssetType::Script) == "script");
  assert(to_string(AssetType::Image) == "image");
  assert(to_string(AssetType::Font) == "font");
  assert(to_string(AssetType::Other) == "other");
}

static void test_asset_loading_to_string()
{
  assert(to_string(AssetLoading::Default) == "default");
  assert(to_string(AssetLoading::Deferred) == "deferred");
  assert(to_string(AssetLoading::Async) == "async");
}

static void test_asset_mode_to_string()
{
  assert(to_string(AssetMode::Development) == "development");
  assert(to_string(AssetMode::Production) == "production");
}

static void test_stylesheet_asset()
{
  Asset asset = Asset::stylesheet("/assets/app.css");

  assert(asset.type() == AssetType::Stylesheet);
  assert(asset.url() == "/assets/app.css");
  assert(asset.media() == "all");
  assert(asset.has_url());
  assert(asset.has_media());

  assert(
      asset.to_html() ==
      R"(<link href="/assets/app.css" media="all" rel="stylesheet">)");
}

static void test_script_asset()
{
  Asset asset = Asset::script("/assets/app.js");

  assert(asset.type() == AssetType::Script);
  assert(asset.url() == "/assets/app.js");
  assert(asset.loading() == AssetLoading::Default);
  assert(!asset.module());

  assert(asset.to_html() == R"(<script src="/assets/app.js"></script>)");
}

static void test_module_script_asset()
{
  Asset asset = Asset::module_script("/assets/app.js");

  assert(asset.type() == AssetType::Script);
  assert(asset.url() == "/assets/app.js");
  assert(asset.loading() == AssetLoading::Default);
  assert(asset.module());

  assert(
      asset.to_html() ==
      R"(<script src="/assets/app.js" type="module"></script>)");
}

static void test_script_asset_can_be_marked_as_module()
{
  Asset asset = Asset::script("/assets/app.js");

  asset.set_module(true);

  assert(asset.module());

  assert(
      asset.to_html() ==
      R"(<script src="/assets/app.js" type="module"></script>)");
}

static void test_deferred_script_asset()
{
  Asset asset = Asset::script("/assets/app.js", AssetLoading::Deferred);

  assert(asset.loading() == AssetLoading::Deferred);
  assert(asset.to_html() == R"(<script defer src="/assets/app.js"></script>)");
}

static void test_async_script_asset()
{
  Asset asset = Asset::script("/assets/app.js", AssetLoading::Async);

  assert(asset.loading() == AssetLoading::Async);
  assert(asset.to_html() == R"(<script async src="/assets/app.js"></script>)");
}

static void test_image_asset()
{
  Asset asset = Asset::image("/assets/logo.png", "Logo");

  assert(asset.type() == AssetType::Image);
  assert(asset.url() == "/assets/logo.png");
  assert(asset.attrs().has("alt"));

  assert(asset.to_html() == R"(<img alt="Logo" src="/assets/logo.png">)");
}

static void test_font_asset()
{
  Asset asset = Asset::font("/assets/inter.woff2");

  assert(asset.type() == AssetType::Font);
  assert(asset.url() == "/assets/inter.woff2");
  assert(asset.crossorigin() == "anonymous");

  assert(
      asset.to_html() ==
      R"(<link as="font" crossorigin="anonymous" href="/assets/inter.woff2" rel="preload">)");
}

static void test_asset_metadata()
{
  Asset asset = Asset::stylesheet("/assets/app.css");

  asset.set_integrity("sha384-test");
  asset.set_crossorigin("anonymous");

  assert(asset.has_integrity());
  assert(asset.has_crossorigin());
  assert(asset.integrity() == "sha384-test");
  assert(asset.crossorigin() == "anonymous");

  assert(
      asset.to_html() ==
      R"(<link crossorigin="anonymous" href="/assets/app.css" integrity="sha384-test" media="all" rel="stylesheet">)");
}

static void test_asset_custom_attrs()
{
  Asset asset = Asset::image("/assets/logo.png");

  asset.set_attr("class", "logo");
  asset.set_bool_attr("hidden", true);

  assert(asset.attrs().has("class"));
  assert(asset.attrs().has("hidden"));

  assert(asset.to_html() == R"(<img class="logo" hidden src="/assets/logo.png">)");
}

static void test_asset_without_url_throws()
{
  Asset asset;

  bool thrown = false;

  try
  {
    (void)asset.to_html();
  }
  catch (const AssetError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_unknown_asset_throws()
{
  Asset asset(AssetType::Unknown, "/assets/file.bin");

  bool thrown = false;

  try
  {
    (void)asset.to_html();
  }
  catch (const AssetError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_manifest_add_and_render()
{
  AssetManifest manifest;

  manifest.add_stylesheet("/assets/app.css");
  manifest.add_script("/assets/app.js", AssetLoading::Deferred);

  assert(!manifest.empty());
  assert(manifest.size() == 2);
  assert(manifest.contains("/assets/app.css"));
  assert(manifest.contains("/assets/app.js"));

  const std::string rendered = manifest.render();

  assert(
      rendered ==
      R"(<link href="/assets/app.css" media="all" rel="stylesheet">)"
      "\n"
      R"(<script defer src="/assets/app.js"></script>)");
}

static void test_manifest_find()
{
  AssetManifest manifest;

  manifest.add_stylesheet("/assets/app.css");

  const Asset *asset = manifest.find("/assets/app.css");

  assert(asset != nullptr);
  assert(asset->type() == AssetType::Stylesheet);

  assert(manifest.find("/missing.css") == nullptr);
}

static void test_manifest_remove()
{
  AssetManifest manifest;

  manifest.add_stylesheet("/assets/app.css");
  manifest.add_script("/assets/app.js");

  const std::size_t removed = manifest.remove("/assets/app.css");

  assert(removed == 1);
  assert(!manifest.contains("/assets/app.css"));
  assert(manifest.size() == 1);
}

static void test_manifest_render_type()
{
  AssetManifest manifest;

  manifest.add_stylesheet("/assets/app.css");
  manifest.add_script("/assets/app.js");
  manifest.add_font("/assets/inter.woff2");

  assert(
      manifest.render_stylesheets() ==
      R"(<link href="/assets/app.css" media="all" rel="stylesheet">)");

  assert(
      manifest.render_scripts() ==
      R"(<script src="/assets/app.js"></script>)");

  assert(
      manifest.render_fonts() ==
      R"(<link as="font" crossorigin="anonymous" href="/assets/inter.woff2" rel="preload">)");
}

static void test_manifest_add_empty_url_throws()
{
  AssetManifest manifest;

  bool thrown = false;

  try
  {
    manifest.add(Asset::stylesheet(""));
  }
  catch (const AssetError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_asset_map_empty()
{
  AssetMap map;

  assert(map.empty());
  assert(map.size() == 0);
  assert(!map.has("css/app.css"));
  assert(map.get("css/app.css") == nullptr);
  assert(map.resolve("css/app.css") == "css/app.css");
}

static void test_asset_map_set_and_resolve()
{
  AssetMap map;

  map.set("css/app.css", "css/app.8fd21c.css");
  map.set("js/app.js", "js/app.91af03.js");

  assert(!map.empty());
  assert(map.size() == 2);

  assert(map.has("css/app.css"));
  assert(map.has("js/app.js"));

  assert(map.get("css/app.css") != nullptr);
  assert(*map.get("css/app.css") == "css/app.8fd21c.css");

  assert(map.resolve("css/app.css") == "css/app.8fd21c.css");
  assert(map.resolve("js/app.js") == "js/app.91af03.js");
}

static void test_asset_map_from_entries()
{
  std::map<std::string, std::string> entries;
  entries["css/app.css"] = "css/app.8fd21c.css";
  entries["js/app.js"] = "js/app.91af03.js";

  AssetMap map = AssetMap::from(entries);

  assert(map.size() == 2);
  assert(map.resolve("css/app.css") == "css/app.8fd21c.css");
  assert(map.resolve("js/app.js") == "js/app.91af03.js");
}

static void test_asset_map_get_or()
{
  AssetMap map;

  assert(map.get_or("missing.css", "fallback.css") == "fallback.css");

  map.set("css/app.css", "css/app.8fd21c.css");

  assert(map.get_or("css/app.css", "fallback.css") == "css/app.8fd21c.css");
}

static void test_asset_map_missing_returns_original()
{
  AssetMap map;

  map.set("css/app.css", "css/app.8fd21c.css");

  assert(map.resolve("missing.css") == "missing.css");
}

static void test_asset_map_remove()
{
  AssetMap map;

  map.set("css/app.css", "css/app.8fd21c.css");

  assert(map.has("css/app.css"));

  const bool removed = map.remove("css/app.css");

  assert(removed);
  assert(!map.has("css/app.css"));
  assert(map.empty());
}

static void test_asset_map_clear()
{
  AssetMap map;

  map.set("css/app.css", "css/app.8fd21c.css");
  map.set("js/app.js", "js/app.91af03.js");

  assert(map.size() == 2);

  map.clear();

  assert(map.empty());
  assert(map.size() == 0);
}

static void test_asset_map_empty_logical_path_throws()
{
  AssetMap map;

  bool thrown = false;

  try
  {
    map.set("", "css/app.8fd21c.css");
  }
  catch (const AssetError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_asset_map_empty_resolved_path_throws()
{
  AssetMap map;

  bool thrown = false;

  try
  {
    map.set("css/app.css", "");
  }
  catch (const AssetError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_asset_manager_base_url()
{
  AssetManager manager("/static");

  assert(manager.base_url() == "/static");
  assert(manager.resolve("app.css") == "/static/app.css");
  assert(manager.resolve("/app.css") == "/static/app.css");
  assert(manager.resolve("https://example.com/app.css") == "https://example.com/app.css");
}

static void test_asset_manager_default_asset_mode()
{
  AssetManager manager("/assets");

  assert(manager.mode() == AssetMode::Development);
  assert(manager.version().empty());
  assert(!manager.has_version());
  assert(!manager.has_asset_map());
}

static void test_asset_manager_set_mode_and_version()
{
  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Production)
      .set_version("0.7.0");

  assert(manager.mode() == AssetMode::Production);
  assert(manager.version() == "0.7.0");
  assert(manager.has_version());
}

static void test_asset_manager_development_path()
{
  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Development);
  manager.set_version("0.7.0");

  assert(manager.path("css/app.css") == "/assets/css/app.css");
  assert(manager.path("/css/app.css") == "/assets/css/app.css");
}

static void test_asset_manager_production_versioned_path()
{
  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Production);
  manager.set_version("0.7.0");

  assert(manager.path("css/app.css") == "/assets/css/app.css?v=0.7.0");
}

static void test_asset_manager_production_versioned_path_with_query()
{
  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Production);
  manager.set_version("0.7.0");

  assert(
      manager.path("css/app.css?theme=dark") ==
      "/assets/css/app.css?theme=dark&v=0.7.0");
}

static void test_asset_manager_production_without_version_keeps_path()
{
  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Production);

  assert(manager.path("css/app.css") == "/assets/css/app.css");
}

static void test_asset_manager_production_absolute_url_is_not_versioned()
{
  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Production);
  manager.set_version("0.7.0");

  assert(
      manager.path("https://example.com/app.css") ==
      "https://example.com/app.css");
}

static void test_asset_manager_production_manifest_path()
{
  AssetMap map;

  map.set("css/app.css", "css/app.8fd21c.css");
  map.set("js/app.js", "js/app.91af03.js");

  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Production);
  manager.set_version("0.7.0");
  manager.set_asset_map(map);

  assert(manager.has_asset_map());
  assert(manager.path("css/app.css") == "/assets/css/app.8fd21c.css");
  assert(manager.path("js/app.js") == "/assets/js/app.91af03.js");
}

static void test_asset_manager_manifest_alias()
{
  AssetMap map;

  map.set("css/app.css", "css/app.8fd21c.css");

  AssetManager manager("/assets");

  manager.set_mode(AssetMode::Production);
  manager.set_manifest(map);

  assert(manager.has_asset_map());
  assert(manager.path("css/app.css") == "/assets/css/app.8fd21c.css");
}

static void test_asset_manager_clear_asset_map()
{
  AssetMap map;

  map.set("css/app.css", "css/app.8fd21c.css");

  AssetManager manager("/assets");

  manager.set_asset_map(map);

  assert(manager.has_asset_map());

  manager.clear_asset_map();

  assert(!manager.has_asset_map());
}

static void test_asset_manager_add_and_get()
{
  AssetManager manager("/assets");

  manager.add_stylesheet("app_css", "app.css");
  manager.add_script("app_js", "app.js");

  assert(manager.size() == 2);
  assert(manager.has("app_css"));
  assert(manager.has("app_js"));

  const Asset *css = manager.get("app_css");
  const Asset *js = manager.get("app_js");

  assert(css != nullptr);
  assert(js != nullptr);

  assert(css->url() == "/assets/app.css");
  assert(js->url() == "/assets/app.js");
}

static void test_asset_manager_add_module_script()
{
  AssetManager manager("/assets");

  manager.add_module_script("app_module", "app.js");

  assert(manager.size() == 1);
  assert(manager.has("app_module"));

  const Asset *asset = manager.get("app_module");

  assert(asset != nullptr);
  assert(asset->type() == AssetType::Script);
  assert(asset->module());
  assert(asset->url() == "/assets/app.js");

  assert(
      manager.render_scripts() ==
      R"(<script src="/assets/app.js" type="module"></script>)");
}

static void test_asset_manager_replace_asset()
{
  AssetManager manager("/assets");

  manager.add_stylesheet("app", "old.css");
  manager.add_stylesheet("app", "new.css");

  assert(manager.size() == 1);

  const Asset *asset = manager.get("app");

  assert(asset != nullptr);
  assert(asset->url() == "/assets/new.css");
}

static void test_asset_manager_remove()
{
  AssetManager manager("/assets");

  manager.add_stylesheet("app", "app.css");

  assert(manager.has("app"));

  const bool removed = manager.remove("app");

  assert(removed);
  assert(!manager.has("app"));
  assert(manager.empty());
}

static void test_asset_manager_manifest()
{
  AssetManager manager("/assets");

  manager.add_stylesheet("app_css", "app.css");
  manager.add_script("app_js", "app.js");

  AssetManifest manifest = manager.manifest();

  assert(manifest.size() == 2);
  assert(manifest.contains("/assets/app.css"));
  assert(manifest.contains("/assets/app.js"));
}

static void test_asset_manager_render_type()
{
  AssetManager manager("/assets");

  manager.add_stylesheet("app_css", "app.css");
  manager.add_script("app_js", "app.js");

  assert(
      manager.render_stylesheets() ==
      R"(<link href="/assets/app.css" media="all" rel="stylesheet">)");

  assert(
      manager.render_scripts() ==
      R"(<script src="/assets/app.js"></script>)");
}

static void test_asset_manager_grouped_stylesheets()
{
  AssetManager manager("/assets");

  manager.add_stylesheet_to("dashboard", "base_css", "base.css");
  manager.add_stylesheet_to("dashboard", "dashboard_css", "dashboard.css");

  assert(manager.has_group("dashboard"));

  const auto *group = manager.group("dashboard");

  assert(group != nullptr);
  assert(group->size() == 2);
  assert((*group)[0] == "base_css");
  assert((*group)[1] == "dashboard_css");

  assert(
      manager.render_stylesheets("dashboard") ==
      R"(<link href="/assets/base.css" media="all" rel="stylesheet">)"
      "\n"
      R"(<link href="/assets/dashboard.css" media="all" rel="stylesheet">)");
}

static void test_asset_manager_grouped_scripts()
{
  AssetManager manager("/assets");

  manager.add_script_to("dashboard", "base_js", "base.js");
  manager.add_script_to(
      "dashboard",
      "dashboard_js",
      "dashboard.js",
      AssetLoading::Deferred);

  assert(manager.has_group("dashboard"));

  assert(
      manager.render_scripts("dashboard") ==
      R"(<script src="/assets/base.js"></script>)"
      "\n"
      R"(<script defer src="/assets/dashboard.js"></script>)");
}

static void test_asset_manager_grouped_module_scripts()
{
  AssetManager manager("/assets");

  manager.add_module_script_to("app", "app_module", "app.js");

  assert(manager.has_group("app"));

  assert(
      manager.render_scripts("app") ==
      R"(<script src="/assets/app.js" type="module"></script>)");
}

static void test_asset_manager_render_group()
{
  AssetManager manager("/assets");

  manager.add_stylesheet_to("app", "app_css", "app.css");
  manager.add_module_script_to("app", "app_js", "app.js");

  assert(
      manager.render_group("app") ==
      R"(<link href="/assets/app.css" media="all" rel="stylesheet">)"
      "\n"
      R"(<script src="/assets/app.js" type="module"></script>)");
}

static void test_asset_manager_manifest_group()
{
  AssetManager manager("/assets");

  manager.add_stylesheet_to("app", "app_css", "app.css");
  manager.add_script_to("app", "app_js", "app.js");

  AssetManifest manifest = manager.manifest("app");

  assert(manifest.size() == 2);
  assert(manifest.contains("/assets/app.css"));
  assert(manifest.contains("/assets/app.js"));
}

static void test_asset_manager_missing_group_renders_empty()
{
  AssetManager manager("/assets");

  manager.add_stylesheet("app_css", "app.css");

  assert(manager.render_group("missing").empty());
  assert(manager.render_stylesheets("missing").empty());
  assert(manager.render_scripts("missing").empty());

  AssetManifest manifest = manager.manifest("missing");

  assert(manifest.empty());
}

static void test_asset_manager_remove_asset_removes_from_groups()
{
  AssetManager manager("/assets");

  manager.add_stylesheet_to("app", "app_css", "app.css");

  assert(manager.has("app_css"));
  assert(manager.has_group("app"));

  const bool removed = manager.remove("app_css");

  assert(removed);
  assert(!manager.has("app_css"));
  assert(!manager.has_group("app"));
}

static void test_asset_manager_remove_group_keeps_assets()
{
  AssetManager manager("/assets");

  manager.add_stylesheet_to("app", "app_css", "app.css");

  assert(manager.has("app_css"));
  assert(manager.has_group("app"));

  const bool removed = manager.remove_group("app");

  assert(removed);
  assert(manager.has("app_css"));
  assert(!manager.has_group("app"));
}

static void test_asset_manager_empty_name_throws()
{
  AssetManager manager;

  bool thrown = false;

  try
  {
    manager.add("", Asset::stylesheet("/assets/app.css"));
  }
  catch (const AssetError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_asset_manager_empty_group_throws()
{
  AssetManager manager("/assets");

  bool thrown = false;

  try
  {
    manager.add_stylesheet_to("", "app_css", "app.css");
  }
  catch (const AssetError &)
  {
    thrown = true;
  }

  assert(thrown);
}

int main()
{
  test_asset_type_to_string();
  test_asset_loading_to_string();
  test_asset_mode_to_string();

  test_stylesheet_asset();
  test_script_asset();
  test_module_script_asset();
  test_script_asset_can_be_marked_as_module();
  test_deferred_script_asset();
  test_async_script_asset();
  test_image_asset();
  test_font_asset();
  test_asset_metadata();
  test_asset_custom_attrs();
  test_asset_without_url_throws();
  test_unknown_asset_throws();

  test_manifest_add_and_render();
  test_manifest_find();
  test_manifest_remove();
  test_manifest_render_type();
  test_manifest_add_empty_url_throws();

  test_asset_map_empty();
  test_asset_map_set_and_resolve();
  test_asset_map_from_entries();
  test_asset_map_get_or();
  test_asset_map_missing_returns_original();
  test_asset_map_remove();
  test_asset_map_clear();
  test_asset_map_empty_logical_path_throws();
  test_asset_map_empty_resolved_path_throws();

  test_asset_manager_base_url();
  test_asset_manager_default_asset_mode();
  test_asset_manager_set_mode_and_version();
  test_asset_manager_development_path();
  test_asset_manager_production_versioned_path();
  test_asset_manager_production_versioned_path_with_query();
  test_asset_manager_production_without_version_keeps_path();
  test_asset_manager_production_absolute_url_is_not_versioned();
  test_asset_manager_production_manifest_path();
  test_asset_manager_manifest_alias();
  test_asset_manager_clear_asset_map();

  test_asset_manager_add_and_get();
  test_asset_manager_add_module_script();
  test_asset_manager_replace_asset();
  test_asset_manager_remove();
  test_asset_manager_manifest();
  test_asset_manager_render_type();

  test_asset_manager_grouped_stylesheets();
  test_asset_manager_grouped_scripts();
  test_asset_manager_grouped_module_scripts();
  test_asset_manager_render_group();
  test_asset_manager_manifest_group();
  test_asset_manager_missing_group_renders_empty();
  test_asset_manager_remove_asset_removes_from_groups();
  test_asset_manager_remove_group_keeps_assets();

  test_asset_manager_empty_name_throws();
  test_asset_manager_empty_group_throws();

  std::cout << "[OK] ui asset tests passed\n";
  return 0;
}
