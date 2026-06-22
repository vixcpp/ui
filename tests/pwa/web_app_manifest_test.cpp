/**
 *
 *  @file web_app_manifest_test.cpp
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

#include <vix/ui/pwa/WebAppManifest.hpp>

using namespace vix::ui;

static void test_display_mode_to_string()
{
  assert(to_string(WebAppDisplayMode::Browser) == "browser");
  assert(to_string(WebAppDisplayMode::MinimalUi) == "minimal-ui");
  assert(to_string(WebAppDisplayMode::Standalone) == "standalone");
  assert(to_string(WebAppDisplayMode::Fullscreen) == "fullscreen");
}

static void test_orientation_to_string()
{
  assert(to_string(WebAppOrientation::Any) == "any");
  assert(to_string(WebAppOrientation::Natural) == "natural");
  assert(to_string(WebAppOrientation::Portrait) == "portrait");
  assert(to_string(WebAppOrientation::Landscape) == "landscape");
  assert(to_string(WebAppOrientation::PortraitPrimary) == "portrait-primary");
  assert(to_string(WebAppOrientation::PortraitSecondary) == "portrait-secondary");
  assert(to_string(WebAppOrientation::LandscapePrimary) == "landscape-primary");
  assert(to_string(WebAppOrientation::LandscapeSecondary) == "landscape-secondary");
}

static void test_direction_to_string()
{
  assert(to_string(WebAppDirection::Auto) == "auto");
  assert(to_string(WebAppDirection::Ltr) == "ltr");
  assert(to_string(WebAppDirection::Rtl) == "rtl");
}

static void test_empty_icon()
{
  WebAppManifestIcon icon;

  assert(icon.src().empty());
  assert(icon.sizes().empty());
  assert(icon.type().empty());
  assert(icon.purpose().empty());

  assert(!icon.has_src());
  assert(!icon.has_sizes());
  assert(!icon.has_type());
  assert(!icon.has_purpose());
  assert(icon.empty());

  assert(icon.to_json() == "{}");
}

static void test_icon_construct_with_src()
{
  WebAppManifestIcon icon("/icons/icon-192.png");

  assert(icon.src() == "/icons/icon-192.png");
  assert(icon.sizes().empty());
  assert(icon.type().empty());

  assert(icon.has_src());
  assert(!icon.has_sizes());
  assert(!icon.has_type());
  assert(!icon.empty());

  assert(icon.to_json() == R"({"src":"/icons/icon-192.png"})");
}

static void test_icon_construct_with_fields()
{
  WebAppManifestIcon icon(
      "/icons/icon-192.png",
      "192x192",
      "image/png");

  assert(icon.src() == "/icons/icon-192.png");
  assert(icon.sizes() == "192x192");
  assert(icon.type() == "image/png");

  assert(icon.has_src());
  assert(icon.has_sizes());
  assert(icon.has_type());

  assert(
      icon.to_json() ==
      R"({"src":"/icons/icon-192.png","sizes":"192x192","type":"image/png"})");
}

static void test_icon_make()
{
  WebAppManifestIcon icon =
      WebAppManifestIcon::make("/icons/icon-512.png", "512x512");

  assert(icon.src() == "/icons/icon-512.png");
  assert(icon.sizes() == "512x512");
  assert(icon.type() == "image/png");

  assert(
      icon.to_json() ==
      R"({"src":"/icons/icon-512.png","sizes":"512x512","type":"image/png"})");
}

static void test_icon_setters()
{
  WebAppManifestIcon icon;

  icon.set_src("/icons/icon-512.png")
      .set_sizes("512x512")
      .set_type("image/png")
      .set_purpose("any maskable");

  assert(icon.src() == "/icons/icon-512.png");
  assert(icon.sizes() == "512x512");
  assert(icon.type() == "image/png");
  assert(icon.purpose() == "any maskable");

  assert(icon.has_src());
  assert(icon.has_sizes());
  assert(icon.has_type());
  assert(icon.has_purpose());

  assert(
      icon.to_json() ==
      R"({"src":"/icons/icon-512.png","sizes":"512x512","type":"image/png","purpose":"any maskable"})");
}

static void test_icon_json_escapes_values()
{
  WebAppManifestIcon icon;

  icon.set_src(R"(/icons/icon "main".png)")
      .set_sizes("192x192")
      .set_type("image/png")
      .set_purpose(R"(any "maskable")");

  assert(
      icon.to_json() ==
      R"({"src":"/icons/icon \"main\".png","sizes":"192x192","type":"image/png","purpose":"any \"maskable\""})");
}

static void test_default_manifest()
{
  WebAppManifest manifest;

  assert(manifest.name().empty());
  assert(manifest.short_name().empty());
  assert(manifest.description().empty());
  assert(manifest.start_url() == "/");
  assert(manifest.scope() == "/");
  assert(manifest.id().empty());
  assert(manifest.lang().empty());

  assert(manifest.direction() == WebAppDirection::Auto);
  assert(manifest.display() == WebAppDisplayMode::Standalone);
  assert(manifest.orientation() == WebAppOrientation::Any);

  assert(manifest.background_color().empty());
  assert(manifest.theme_color().empty());
  assert(manifest.icons().empty());

  assert(!manifest.has_name());
  assert(!manifest.has_short_name());
  assert(!manifest.has_description());
  assert(manifest.has_start_url());
  assert(manifest.has_scope());
  assert(!manifest.has_id());
  assert(!manifest.has_lang());
  assert(!manifest.has_background_color());
  assert(!manifest.has_theme_color());
  assert(!manifest.has_icons());
  assert(manifest.icon_count() == 0);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone"})");
}

static void test_make_manifest()
{
  WebAppManifest manifest = WebAppManifest::make();

  assert(manifest.start_url() == "/");
  assert(manifest.scope() == "/");
  assert(manifest.display() == WebAppDisplayMode::Standalone);

  assert(
      manifest.render() ==
      R"({"start_url":"/","scope":"/","display":"standalone"})");
}

static void test_app_factory()
{
  WebAppManifest manifest = WebAppManifest::app("Vix App", "Vix");

  assert(manifest.name() == "Vix App");
  assert(manifest.short_name() == "Vix");
  assert(manifest.display() == WebAppDisplayMode::Standalone);

  assert(
      manifest.to_json() ==
      R"({"name":"Vix App","short_name":"Vix","start_url":"/","scope":"/","display":"standalone"})");
}

static void test_setters()
{
  WebAppManifest manifest;

  manifest.set_name("Vix Dashboard")
      .set_short_name("Vix")
      .set_description("A Vix UI dashboard")
      .set_start_url("/dashboard")
      .set_scope("/")
      .set_id("com.vix.dashboard")
      .set_lang("en")
      .set_direction(WebAppDirection::Ltr)
      .set_display(WebAppDisplayMode::Fullscreen)
      .set_orientation(WebAppOrientation::Portrait)
      .set_background_color("#ffffff")
      .set_theme_color("#111111");

  assert(manifest.name() == "Vix Dashboard");
  assert(manifest.short_name() == "Vix");
  assert(manifest.description() == "A Vix UI dashboard");
  assert(manifest.start_url() == "/dashboard");
  assert(manifest.scope() == "/");
  assert(manifest.id() == "com.vix.dashboard");
  assert(manifest.lang() == "en");

  assert(manifest.direction() == WebAppDirection::Ltr);
  assert(manifest.display() == WebAppDisplayMode::Fullscreen);
  assert(manifest.orientation() == WebAppOrientation::Portrait);

  assert(manifest.background_color() == "#ffffff");
  assert(manifest.theme_color() == "#111111");

  assert(manifest.has_name());
  assert(manifest.has_short_name());
  assert(manifest.has_description());
  assert(manifest.has_start_url());
  assert(manifest.has_scope());
  assert(manifest.has_id());
  assert(manifest.has_lang());
  assert(manifest.has_background_color());
  assert(manifest.has_theme_color());
}

static void test_manifest_json_with_fields()
{
  WebAppManifest manifest;

  manifest.set_name("Vix Dashboard")
      .set_short_name("Vix")
      .set_description("A Vix UI dashboard")
      .set_start_url("/dashboard")
      .set_scope("/")
      .set_id("com.vix.dashboard")
      .set_lang("en")
      .set_direction(WebAppDirection::Ltr)
      .set_display(WebAppDisplayMode::Fullscreen)
      .set_orientation(WebAppOrientation::Portrait)
      .set_background_color("#ffffff")
      .set_theme_color("#111111");

  assert(
      manifest.to_json() ==
      R"({"name":"Vix Dashboard","short_name":"Vix","description":"A Vix UI dashboard","start_url":"/dashboard","scope":"/","id":"com.vix.dashboard","lang":"en","dir":"ltr","display":"fullscreen","orientation":"portrait","background_color":"#ffffff","theme_color":"#111111"})");
}

static void test_manifest_auto_direction_is_skipped()
{
  WebAppManifest manifest;

  manifest.set_direction(WebAppDirection::Auto);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone"})");
}

static void test_manifest_any_orientation_is_skipped()
{
  WebAppManifest manifest;

  manifest.set_orientation(WebAppOrientation::Any);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone"})");
}

static void test_manifest_rtl_direction()
{
  WebAppManifest manifest;

  manifest.set_direction(WebAppDirection::Rtl);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","dir":"rtl","display":"standalone"})");
}

static void test_manifest_landscape_orientation()
{
  WebAppManifest manifest;

  manifest.set_orientation(WebAppOrientation::Landscape);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone","orientation":"landscape"})");
}

static void test_add_icon_object()
{
  WebAppManifest manifest;

  WebAppManifestIcon icon =
      WebAppManifestIcon::make("/icons/icon-192.png", "192x192");

  manifest.add_icon(icon);

  assert(manifest.has_icons());
  assert(manifest.icon_count() == 1);
  assert(manifest.icons().size() == 1);
  assert(manifest.icons()[0].src() == "/icons/icon-192.png");

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone","icons":[{"src":"/icons/icon-192.png","sizes":"192x192","type":"image/png"}]})");
}

static void test_add_icon_fields()
{
  WebAppManifest manifest;

  manifest.add_icon(
      "/icons/icon-512.png",
      "512x512",
      "image/png",
      "any maskable");

  assert(manifest.has_icons());
  assert(manifest.icon_count() == 1);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone","icons":[{"src":"/icons/icon-512.png","sizes":"512x512","type":"image/png","purpose":"any maskable"}]})");
}

static void test_add_empty_icon_is_ignored()
{
  WebAppManifest manifest;

  manifest.add_icon(WebAppManifestIcon{});
  manifest.add_icon("", "192x192");

  assert(!manifest.has_icons());
  assert(manifest.icon_count() == 0);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone"})");
}

static void test_add_multiple_icons()
{
  WebAppManifest manifest;

  manifest.add_icon("/icons/icon-192.png", "192x192");
  manifest.add_icon(
      "/icons/icon-512.png",
      "512x512",
      "image/png",
      "any maskable");

  assert(manifest.has_icons());
  assert(manifest.icon_count() == 2);

  assert(
      manifest.to_json() ==
      R"({"start_url":"/","scope":"/","display":"standalone","icons":[{"src":"/icons/icon-192.png","sizes":"192x192","type":"image/png"},{"src":"/icons/icon-512.png","sizes":"512x512","type":"image/png","purpose":"any maskable"}]})");
}

static void test_clear_icons()
{
  WebAppManifest manifest;

  manifest.add_icon("/icons/icon-192.png", "192x192");

  assert(manifest.has_icons());

  manifest.clear_icons();

  assert(!manifest.has_icons());
  assert(manifest.icon_count() == 0);
  assert(manifest.icons().empty());
}

static void test_mutable_icons_access()
{
  WebAppManifest manifest;

  manifest.icons().push_back(
      WebAppManifestIcon::make("/icons/icon-192.png", "192x192"));

  assert(manifest.has_icons());
  assert(manifest.icon_count() == 1);
  assert(manifest.icons()[0].src() == "/icons/icon-192.png");
}

static void test_manifest_json_escapes_values()
{
  WebAppManifest manifest;

  manifest.set_name(R"(Vix "App")")
      .set_description("Line 1\nLine 2")
      .set_start_url(R"(/start?name="vix")")
      .set_scope("/")
      .set_theme_color(R"(#11"11)");

  assert(
      manifest.to_json() ==
      "{\"name\":\"Vix \\\"App\\\"\",\"description\":\"Line 1\\nLine 2\",\"start_url\":\"/start?name=\\\"vix\\\"\",\"scope\":\"/\",\"display\":\"standalone\",\"theme_color\":\"#11\\\"11\"}");
}

static void test_render_alias()
{
  WebAppManifest manifest = WebAppManifest::app("Vix App", "Vix");

  assert(manifest.render() == manifest.to_json());
}

int main()
{
  test_display_mode_to_string();
  test_orientation_to_string();
  test_direction_to_string();

  test_empty_icon();
  test_icon_construct_with_src();
  test_icon_construct_with_fields();
  test_icon_make();
  test_icon_setters();
  test_icon_json_escapes_values();

  test_default_manifest();
  test_make_manifest();
  test_app_factory();

  test_setters();
  test_manifest_json_with_fields();
  test_manifest_auto_direction_is_skipped();
  test_manifest_any_orientation_is_skipped();
  test_manifest_rtl_direction();
  test_manifest_landscape_orientation();

  test_add_icon_object();
  test_add_icon_fields();
  test_add_empty_icon_is_ignored();
  test_add_multiple_icons();
  test_clear_icons();
  test_mutable_icons_access();

  test_manifest_json_escapes_values();
  test_render_alias();

  std::cout << "[OK] ui web app manifest tests passed\n";
  return 0;
}
