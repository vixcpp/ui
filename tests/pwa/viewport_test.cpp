/**
 *
 *  @file viewport_test.cpp
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

#include <vix/ui/pwa/Viewport.hpp>

using namespace vix::ui;

static void test_viewport_fit_to_string()
{
  assert(to_string(ViewportFit::Auto) == "auto");
  assert(to_string(ViewportFit::Contain) == "contain");
  assert(to_string(ViewportFit::Cover) == "cover");
}

static void test_default_viewport()
{
  Viewport viewport;

  assert(viewport.width() == "device-width");
  assert(viewport.initial_scale() == "1");
  assert(viewport.minimum_scale().empty());
  assert(viewport.maximum_scale().empty());
  assert(viewport.user_scalable().empty());
  assert(viewport.viewport_fit() == ViewportFit::Auto);
  assert(viewport.attrs().empty());

  assert(viewport.has_width());
  assert(viewport.has_initial_scale());
  assert(!viewport.has_minimum_scale());
  assert(!viewport.has_maximum_scale());
  assert(!viewport.has_user_scalable());

  assert(viewport.content() == "width=device-width, initial-scale=1");

  assert(
      viewport.render() ==
      R"(<meta content="width=device-width, initial-scale=1" name="viewport">)");
}

static void test_responsive_factory()
{
  Viewport viewport = Viewport::responsive();

  assert(viewport.width() == "device-width");
  assert(viewport.initial_scale() == "1");
  assert(viewport.viewport_fit() == ViewportFit::Auto);

  assert(
      viewport.render() ==
      R"(<meta content="width=device-width, initial-scale=1" name="viewport">)");
}

static void test_mobile_app_factory()
{
  Viewport viewport = Viewport::mobile_app();

  assert(viewport.width() == "device-width");
  assert(viewport.initial_scale() == "1");
  assert(viewport.viewport_fit() == ViewportFit::Cover);

  assert(
      viewport.content() ==
      "width=device-width, initial-scale=1, viewport-fit=cover");

  assert(
      viewport.render() ==
      R"(<meta content="width=device-width, initial-scale=1, viewport-fit=cover" name="viewport">)");
}

static void test_setters()
{
  Viewport viewport;

  viewport.set_width("480")
      .set_initial_scale("1.0")
      .set_minimum_scale("1")
      .set_maximum_scale("5")
      .set_user_scalable(true)
      .set_viewport_fit(ViewportFit::Contain);

  assert(viewport.width() == "480");
  assert(viewport.initial_scale() == "1.0");
  assert(viewport.minimum_scale() == "1");
  assert(viewport.maximum_scale() == "5");
  assert(viewport.user_scalable() == "yes");
  assert(viewport.viewport_fit() == ViewportFit::Contain);

  assert(viewport.has_width());
  assert(viewport.has_initial_scale());
  assert(viewport.has_minimum_scale());
  assert(viewport.has_maximum_scale());
  assert(viewport.has_user_scalable());

  assert(
      viewport.content() ==
      "width=480, initial-scale=1.0, minimum-scale=1, maximum-scale=5, user-scalable=yes, viewport-fit=contain");
}

static void test_user_scalable_false()
{
  Viewport viewport;

  viewport.set_user_scalable(false);

  assert(viewport.user_scalable() == "no");
  assert(viewport.has_user_scalable());

  assert(
      viewport.content() ==
      "width=device-width, initial-scale=1, user-scalable=no");
}

static void test_empty_values_are_skipped()
{
  Viewport viewport;

  viewport.set_width("");
  viewport.set_initial_scale("");
  viewport.set_minimum_scale("");
  viewport.set_maximum_scale("");

  assert(!viewport.has_width());
  assert(!viewport.has_initial_scale());
  assert(!viewport.has_minimum_scale());
  assert(!viewport.has_maximum_scale());

  assert(viewport.content().empty());

  assert(
      viewport.render() ==
      R"(<meta content="" name="viewport">)");
}

static void test_viewport_fit_auto_is_skipped()
{
  Viewport viewport;

  viewport.set_viewport_fit(ViewportFit::Auto);

  assert(viewport.content() == "width=device-width, initial-scale=1");

  assert(
      viewport.render() ==
      R"(<meta content="width=device-width, initial-scale=1" name="viewport">)");
}

static void test_viewport_fit_cover()
{
  Viewport viewport;

  viewport.set_viewport_fit(ViewportFit::Cover);

  assert(
      viewport.content() ==
      "width=device-width, initial-scale=1, viewport-fit=cover");

  assert(
      viewport.render() ==
      R"(<meta content="width=device-width, initial-scale=1, viewport-fit=cover" name="viewport">)");
}

static void test_attrs()
{
  Viewport viewport;

  viewport.set_attr("id", "main-viewport");
  viewport.set_bool_attr("hidden", true);

  assert(viewport.attrs().has("id"));
  assert(viewport.attrs().has("hidden"));
  assert(viewport.attrs().size() == 2);
}

static void test_bool_attr_can_be_removed()
{
  Viewport viewport;

  viewport.set_bool_attr("hidden", true);
  assert(viewport.attrs().has("hidden"));

  viewport.set_bool_attr("hidden", false);
  assert(!viewport.attrs().has("hidden"));
}

static void test_render_with_custom_attrs()
{
  Viewport viewport;

  viewport.set_attr("id", "main-viewport");
  viewport.set_attr("data-source", "vix-ui");

  assert(
      viewport.render() ==
      R"(<meta content="width=device-width, initial-scale=1" data-source="vix-ui" id="main-viewport" name="viewport">)");
}

static void test_render_escapes_values()
{
  Viewport viewport;

  viewport.set_width(R"(device-width "main")");
  viewport.set_attr("data-note", R"(<mobile & ready>)");

  assert(
      viewport.render() ==
      R"(<meta content="width=device-width &quot;main&quot;, initial-scale=1" data-note="&lt;mobile &amp; ready&gt;" name="viewport">)");
}

int main()
{
  test_viewport_fit_to_string();

  test_default_viewport();
  test_responsive_factory();
  test_mobile_app_factory();

  test_setters();
  test_user_scalable_false();
  test_empty_values_are_skipped();
  test_viewport_fit_auto_is_skipped();
  test_viewport_fit_cover();

  test_attrs();
  test_bool_attr_can_be_removed();
  test_render_with_custom_attrs();
  test_render_escapes_values();

  std::cout << "[OK] ui viewport tests passed\n";
  return 0;
}
