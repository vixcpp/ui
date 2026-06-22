/**
 *
 *  @file pwa_meta_test.cpp
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

#include <vix/ui/pwa/PwaMeta.hpp>
#include <vix/ui/pwa/Viewport.hpp>

using namespace vix::ui;

static void test_apple_status_bar_style_to_string()
{
  assert(to_string(PwaAppleStatusBarStyle::Default) == "default");
  assert(to_string(PwaAppleStatusBarStyle::Black) == "black");
  assert(
      to_string(PwaAppleStatusBarStyle::BlackTranslucent) ==
      "black-translucent");
}

static void test_default_pwa_meta()
{
  PwaMeta meta;

  assert(meta.include_viewport());
  assert(meta.include_manifest());

  assert(meta.manifest_href() == "/manifest.webmanifest");
  assert(meta.theme_color().empty());
  assert(meta.application_name().empty());
  assert(meta.apple_mobile_web_app_title().empty());
  assert(meta.color_scheme().empty());
  assert(meta.format_detection().empty());

  assert(meta.viewport().viewport_fit() == ViewportFit::Cover);

  assert(meta.has_manifest_href());
  assert(!meta.has_theme_color());
  assert(!meta.has_application_name());
  assert(!meta.has_apple_mobile_web_app_title());
  assert(!meta.has_apple_mobile_web_app_capable());
  assert(!meta.has_apple_status_bar_style());
  assert(!meta.has_color_scheme());
  assert(!meta.has_format_detection());

  assert(
      meta.render() ==
      R"(<meta content="width=device-width, initial-scale=1, viewport-fit=cover" name="viewport">)"
      "\n"
      R"(<link href="/manifest.webmanifest" rel="manifest">)");
}

static void test_make_factory()
{
  PwaMeta meta = PwaMeta::make();

  assert(meta.include_viewport());
  assert(meta.include_manifest());
  assert(meta.manifest_href() == "/manifest.webmanifest");

  assert(
      meta.render() ==
      R"(<meta content="width=device-width, initial-scale=1, viewport-fit=cover" name="viewport">)"
      "\n"
      R"(<link href="/manifest.webmanifest" rel="manifest">)");
}

static void test_mobile_app_factory()
{
  PwaMeta meta = PwaMeta::mobile_app("Vix App", "#111111");

  assert(meta.include_viewport());
  assert(meta.include_manifest());

  assert(meta.application_name() == "Vix App");
  assert(meta.apple_mobile_web_app_title() == "Vix App");
  assert(meta.theme_color() == "#111111");

  assert(meta.has_application_name());
  assert(meta.has_apple_mobile_web_app_title());
  assert(meta.has_theme_color());

  assert(meta.has_apple_mobile_web_app_capable());
  assert(meta.apple_mobile_web_app_capable());

  assert(meta.has_apple_status_bar_style());
  assert(
      meta.apple_status_bar_style() ==
      PwaAppleStatusBarStyle::BlackTranslucent);

  assert(
      meta.render() ==
      R"(<meta content="width=device-width, initial-scale=1, viewport-fit=cover" name="viewport">)"
      "\n"
      R"(<link href="/manifest.webmanifest" rel="manifest">)"
      "\n"
      R"(<meta content="#111111" name="theme-color">)"
      "\n"
      R"(<meta content="Vix App" name="application-name">)"
      "\n"
      R"(<meta content="yes" name="apple-mobile-web-app-capable">)"
      "\n"
      R"(<meta content="Vix App" name="apple-mobile-web-app-title">)"
      "\n"
      R"(<meta content="black-translucent" name="apple-mobile-web-app-status-bar-style">)");
}

static void test_setters()
{
  PwaMeta meta;

  Viewport viewport = Viewport::responsive();

  meta.set_include_viewport(false)
      .set_include_manifest(false)
      .set_viewport(viewport)
      .set_manifest_href("/app.webmanifest")
      .set_theme_color("#ffffff")
      .set_application_name("Vix")
      .set_apple_mobile_web_app_title("Vix Mobile")
      .set_apple_mobile_web_app_capable(true)
      .set_apple_status_bar_style(PwaAppleStatusBarStyle::Black)
      .set_color_scheme("light dark")
      .set_format_detection("telephone=no");

  assert(!meta.include_viewport());
  assert(!meta.include_manifest());

  assert(meta.viewport().viewport_fit() == ViewportFit::Auto);
  assert(meta.manifest_href() == "/app.webmanifest");
  assert(meta.theme_color() == "#ffffff");
  assert(meta.application_name() == "Vix");
  assert(meta.apple_mobile_web_app_title() == "Vix Mobile");
  assert(meta.apple_mobile_web_app_capable());
  assert(meta.apple_status_bar_style() == PwaAppleStatusBarStyle::Black);
  assert(meta.color_scheme() == "light dark");
  assert(meta.format_detection() == "telephone=no");

  assert(meta.has_manifest_href());
  assert(meta.has_theme_color());
  assert(meta.has_application_name());
  assert(meta.has_apple_mobile_web_app_title());
  assert(meta.has_apple_mobile_web_app_capable());
  assert(meta.has_apple_status_bar_style());
  assert(meta.has_color_scheme());
  assert(meta.has_format_detection());
}

static void test_mutable_viewport_access()
{
  PwaMeta meta;

  meta.viewport().set_user_scalable(false);

  assert(
      meta.render_viewport() ==
      R"(<meta content="width=device-width, initial-scale=1, user-scalable=no, viewport-fit=cover" name="viewport">)");
}

static void test_render_viewport()
{
  PwaMeta meta;

  assert(
      meta.render_viewport() ==
      R"(<meta content="width=device-width, initial-scale=1, viewport-fit=cover" name="viewport">)");
}

static void test_render_viewport_disabled()
{
  PwaMeta meta;

  meta.set_include_viewport(false);

  assert(meta.render_viewport().empty());
}

static void test_render_manifest_link()
{
  PwaMeta meta;

  assert(
      meta.render_manifest_link() ==
      R"(<link href="/manifest.webmanifest" rel="manifest">)");
}

static void test_render_manifest_link_custom_href()
{
  PwaMeta meta;

  meta.set_manifest_href("/site.webmanifest");

  assert(
      meta.render_manifest_link() ==
      R"(<link href="/site.webmanifest" rel="manifest">)");
}

static void test_render_manifest_link_disabled()
{
  PwaMeta meta;

  meta.set_include_manifest(false);

  assert(meta.render_manifest_link().empty());
}

static void test_render_manifest_link_empty_href()
{
  PwaMeta meta;

  meta.set_manifest_href("");

  assert(!meta.has_manifest_href());
  assert(meta.render_manifest_link().empty());
}

static void test_manifest_attrs()
{
  PwaMeta meta;

  meta.set_manifest_attr("id", "pwa-manifest");
  meta.set_manifest_attr("data-source", "vix-ui");
  meta.set_manifest_bool_attr("hidden", true);

  assert(meta.manifest_attrs().has("id"));
  assert(meta.manifest_attrs().has("data-source"));
  assert(meta.manifest_attrs().has("hidden"));
  assert(meta.manifest_attrs().size() == 3);

  assert(
      meta.render_manifest_link() ==
      R"(<link data-source="vix-ui" hidden href="/manifest.webmanifest" id="pwa-manifest" rel="manifest">)");
}

static void test_manifest_bool_attr_can_be_removed()
{
  PwaMeta meta;

  meta.set_manifest_bool_attr("hidden", true);
  assert(meta.manifest_attrs().has("hidden"));

  meta.set_manifest_bool_attr("hidden", false);
  assert(!meta.manifest_attrs().has("hidden"));
}

static void test_render_theme_color()
{
  PwaMeta meta;

  meta.set_theme_color("#111111");

  assert(
      meta.render_theme_color() ==
      R"(<meta content="#111111" name="theme-color">)");
}

static void test_render_theme_color_empty()
{
  PwaMeta meta;

  assert(meta.render_theme_color().empty());
}

static void test_render_application_name()
{
  PwaMeta meta;

  meta.set_application_name("Vix App");

  assert(
      meta.render_application_name() ==
      R"(<meta content="Vix App" name="application-name">)");
}

static void test_render_application_name_empty()
{
  PwaMeta meta;

  assert(meta.render_application_name().empty());
}

static void test_render_apple_mobile_web_app_capable_yes()
{
  PwaMeta meta;

  meta.set_apple_mobile_web_app_capable(true);

  assert(meta.has_apple_mobile_web_app_capable());
  assert(meta.apple_mobile_web_app_capable());

  assert(
      meta.render_apple_mobile_web_app_capable() ==
      R"(<meta content="yes" name="apple-mobile-web-app-capable">)");
}

static void test_render_apple_mobile_web_app_capable_no()
{
  PwaMeta meta;

  meta.set_apple_mobile_web_app_capable(false);

  assert(meta.has_apple_mobile_web_app_capable());
  assert(!meta.apple_mobile_web_app_capable());

  assert(
      meta.render_apple_mobile_web_app_capable() ==
      R"(<meta content="no" name="apple-mobile-web-app-capable">)");
}

static void test_render_apple_mobile_web_app_capable_unset()
{
  PwaMeta meta;

  assert(!meta.has_apple_mobile_web_app_capable());
  assert(meta.render_apple_mobile_web_app_capable().empty());
}

static void test_render_apple_mobile_web_app_title()
{
  PwaMeta meta;

  meta.set_apple_mobile_web_app_title("Vix Mobile");

  assert(
      meta.render_apple_mobile_web_app_title() ==
      R"(<meta content="Vix Mobile" name="apple-mobile-web-app-title">)");
}

static void test_render_apple_mobile_web_app_title_empty()
{
  PwaMeta meta;

  assert(meta.render_apple_mobile_web_app_title().empty());
}

static void test_render_apple_status_bar_style()
{
  PwaMeta meta;

  meta.set_apple_status_bar_style(PwaAppleStatusBarStyle::BlackTranslucent);

  assert(meta.has_apple_status_bar_style());

  assert(
      meta.render_apple_status_bar_style() ==
      R"(<meta content="black-translucent" name="apple-mobile-web-app-status-bar-style">)");
}

static void test_render_apple_status_bar_style_unset()
{
  PwaMeta meta;

  assert(!meta.has_apple_status_bar_style());
  assert(meta.render_apple_status_bar_style().empty());
}

static void test_render_color_scheme()
{
  PwaMeta meta;

  meta.set_color_scheme("light dark");

  assert(
      meta.render_color_scheme() ==
      R"(<meta content="light dark" name="color-scheme">)");
}

static void test_render_color_scheme_empty()
{
  PwaMeta meta;

  assert(meta.render_color_scheme().empty());
}

static void test_render_format_detection()
{
  PwaMeta meta;

  meta.set_format_detection("telephone=no");

  assert(
      meta.render_format_detection() ==
      R"(<meta content="telephone=no" name="format-detection">)");
}

static void test_render_format_detection_empty()
{
  PwaMeta meta;

  assert(meta.render_format_detection().empty());
}

static void test_render_all_tags()
{
  PwaMeta meta;

  meta.set_manifest_href("/app.webmanifest")
      .set_theme_color("#111111")
      .set_application_name("Vix App")
      .set_apple_mobile_web_app_capable(true)
      .set_apple_mobile_web_app_title("Vix Mobile")
      .set_apple_status_bar_style(PwaAppleStatusBarStyle::Black)
      .set_color_scheme("light dark")
      .set_format_detection("telephone=no");

  assert(
      meta.render() ==
      R"(<meta content="width=device-width, initial-scale=1, viewport-fit=cover" name="viewport">)"
      "\n"
      R"(<link href="/app.webmanifest" rel="manifest">)"
      "\n"
      R"(<meta content="#111111" name="theme-color">)"
      "\n"
      R"(<meta content="Vix App" name="application-name">)"
      "\n"
      R"(<meta content="yes" name="apple-mobile-web-app-capable">)"
      "\n"
      R"(<meta content="Vix Mobile" name="apple-mobile-web-app-title">)"
      "\n"
      R"(<meta content="black" name="apple-mobile-web-app-status-bar-style">)"
      "\n"
      R"(<meta content="light dark" name="color-scheme">)"
      "\n"
      R"(<meta content="telephone=no" name="format-detection">)");
}

static void test_render_without_viewport_and_manifest()
{
  PwaMeta meta;

  meta.set_include_viewport(false)
      .set_include_manifest(false)
      .set_theme_color("#111111");

  assert(
      meta.render() ==
      R"(<meta content="#111111" name="theme-color">)");
}

static void test_render_empty_when_all_disabled_or_empty()
{
  PwaMeta meta;

  meta.set_include_viewport(false)
      .set_include_manifest(false);

  assert(meta.render().empty());
}

static void test_render_escapes_values()
{
  PwaMeta meta;

  meta.set_include_viewport(false)
      .set_manifest_href(R"(/manifest "main".webmanifest)")
      .set_theme_color(R"(#11"11)")
      .set_application_name(R"(Vix "App")")
      .set_manifest_attr("data-note", R"(<manifest & ready>)");

  assert(
      meta.render() ==
      R"(<link data-note="&lt;manifest &amp; ready&gt;" href="/manifest &quot;main&quot;.webmanifest" rel="manifest">)"
      "\n"
      R"(<meta content="#11&quot;11" name="theme-color">)"
      "\n"
      R"(<meta content="Vix &quot;App&quot;" name="application-name">)");
}

int main()
{
  test_apple_status_bar_style_to_string();

  test_default_pwa_meta();
  test_make_factory();
  test_mobile_app_factory();

  test_setters();
  test_mutable_viewport_access();

  test_render_viewport();
  test_render_viewport_disabled();

  test_render_manifest_link();
  test_render_manifest_link_custom_href();
  test_render_manifest_link_disabled();
  test_render_manifest_link_empty_href();

  test_manifest_attrs();
  test_manifest_bool_attr_can_be_removed();

  test_render_theme_color();
  test_render_theme_color_empty();

  test_render_application_name();
  test_render_application_name_empty();

  test_render_apple_mobile_web_app_capable_yes();
  test_render_apple_mobile_web_app_capable_no();
  test_render_apple_mobile_web_app_capable_unset();

  test_render_apple_mobile_web_app_title();
  test_render_apple_mobile_web_app_title_empty();

  test_render_apple_status_bar_style();
  test_render_apple_status_bar_style_unset();

  test_render_color_scheme();
  test_render_color_scheme_empty();

  test_render_format_detection();
  test_render_format_detection_empty();

  test_render_all_tags();
  test_render_without_viewport_and_manifest();
  test_render_empty_when_all_disabled_or_empty();
  test_render_escapes_values();

  std::cout << "[OK] ui pwa meta tests passed\n";
  return 0;
}
