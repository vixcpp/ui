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
#include <string>

#include <vix/ui/assets/Asset.hpp>
#include <vix/ui/assets/AssetManager.hpp>
#include <vix/ui/assets/AssetManifest.hpp>
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

  assert(asset.to_html() == R"(<script src="/assets/app.js"></script>)");
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

static void test_asset_manager_base_url()
{
  AssetManager manager("/static");

  assert(manager.base_url() == "/static");
  assert(manager.resolve("app.css") == "/static/app.css");
  assert(manager.resolve("/app.css") == "/static/app.css");
  assert(manager.resolve("https://example.com/app.css") == "https://example.com/app.css");
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

int main()
{
  test_asset_type_to_string();
  test_asset_loading_to_string();

  test_stylesheet_asset();
  test_script_asset();
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

  test_asset_manager_base_url();
  test_asset_manager_add_and_get();
  test_asset_manager_replace_asset();
  test_asset_manager_remove();
  test_asset_manager_manifest();
  test_asset_manager_render_type();
  test_asset_manager_empty_name_throws();

  std::cout << "[OK] ui asset tests passed\n";
  return 0;
}
