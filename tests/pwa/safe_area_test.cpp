/**
 *
 *  @file safe_area_test.cpp
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

#include <vix/ui/pwa/SafeArea.hpp>

using namespace vix::ui;

static void test_safe_area_edge_to_string()
{
  assert(to_string(SafeAreaEdge::Top) == "top");
  assert(to_string(SafeAreaEdge::Right) == "right");
  assert(to_string(SafeAreaEdge::Bottom) == "bottom");
  assert(to_string(SafeAreaEdge::Left) == "left");
}

static void test_default_safe_area()
{
  SafeArea safe_area;

  assert(safe_area.root_selector() == ":root");
  assert(safe_area.class_name() == "vix-safe-area");
  assert(safe_area.variable_prefix() == "--vix-safe-area");

  assert(safe_area.top() == "env(safe-area-inset-top)");
  assert(safe_area.right() == "env(safe-area-inset-right)");
  assert(safe_area.bottom() == "env(safe-area-inset-bottom)");
  assert(safe_area.left() == "env(safe-area-inset-left)");

  assert(safe_area.has_root_selector());
  assert(safe_area.has_class_name());
  assert(safe_area.has_variable_prefix());

  assert(safe_area.has_top());
  assert(safe_area.has_right());
  assert(safe_area.has_bottom());
  assert(safe_area.has_left());
}

static void test_make_factory()
{
  SafeArea safe_area = SafeArea::make();

  assert(safe_area.root_selector() == ":root");
  assert(safe_area.class_name() == "vix-safe-area");
  assert(safe_area.variable_prefix() == "--vix-safe-area");
}

static void test_all_factory()
{
  SafeArea safe_area = SafeArea::all();

  assert(safe_area.has_top());
  assert(safe_area.has_right());
  assert(safe_area.has_bottom());
  assert(safe_area.has_left());
}

static void test_vertical_factory()
{
  SafeArea safe_area = SafeArea::vertical();

  assert(safe_area.has_top());
  assert(!safe_area.has_right());
  assert(safe_area.has_bottom());
  assert(!safe_area.has_left());

  assert(safe_area.right().empty());
  assert(safe_area.left().empty());
}

static void test_setters()
{
  SafeArea safe_area;

  safe_area.set_root_selector("html")
      .set_class_name("mobile-safe")
      .set_variable_prefix("--app-safe")
      .set_top("10px")
      .set_right("20px")
      .set_bottom("30px")
      .set_left("40px");

  assert(safe_area.root_selector() == "html");
  assert(safe_area.class_name() == "mobile-safe");
  assert(safe_area.variable_prefix() == "--app-safe");

  assert(safe_area.top() == "10px");
  assert(safe_area.right() == "20px");
  assert(safe_area.bottom() == "30px");
  assert(safe_area.left() == "40px");
}

static void test_set_by_edge()
{
  SafeArea safe_area;

  safe_area.set(SafeAreaEdge::Top, "1px");
  safe_area.set(SafeAreaEdge::Right, "2px");
  safe_area.set(SafeAreaEdge::Bottom, "3px");
  safe_area.set(SafeAreaEdge::Left, "4px");

  assert(safe_area.get(SafeAreaEdge::Top) == "1px");
  assert(safe_area.get(SafeAreaEdge::Right) == "2px");
  assert(safe_area.get(SafeAreaEdge::Bottom) == "3px");
  assert(safe_area.get(SafeAreaEdge::Left) == "4px");
}

static void test_empty_values()
{
  SafeArea safe_area;

  safe_area.set_root_selector("")
      .set_class_name("")
      .set_variable_prefix("")
      .set_top("")
      .set_right("")
      .set_bottom("")
      .set_left("");

  assert(!safe_area.has_root_selector());
  assert(!safe_area.has_class_name());
  assert(!safe_area.has_variable_prefix());

  assert(!safe_area.has_top());
  assert(!safe_area.has_right());
  assert(!safe_area.has_bottom());
  assert(!safe_area.has_left());

  assert(safe_area.variable_name(SafeAreaEdge::Top).empty());
  assert(safe_area.variable_ref(SafeAreaEdge::Top).empty());

  assert(safe_area.render_variables().empty());
  assert(safe_area.render_class().empty());
  assert(safe_area.render().empty());
}

static void test_variable_name()
{
  SafeArea safe_area;

  assert(safe_area.variable_name(SafeAreaEdge::Top) == "--vix-safe-area-top");
  assert(safe_area.variable_name(SafeAreaEdge::Right) == "--vix-safe-area-right");
  assert(safe_area.variable_name(SafeAreaEdge::Bottom) == "--vix-safe-area-bottom");
  assert(safe_area.variable_name(SafeAreaEdge::Left) == "--vix-safe-area-left");
}

static void test_variable_ref()
{
  SafeArea safe_area;

  assert(safe_area.variable_ref(SafeAreaEdge::Top) == "var(--vix-safe-area-top)");
  assert(safe_area.variable_ref(SafeAreaEdge::Right) == "var(--vix-safe-area-right)");
  assert(safe_area.variable_ref(SafeAreaEdge::Bottom) == "var(--vix-safe-area-bottom)");
  assert(safe_area.variable_ref(SafeAreaEdge::Left) == "var(--vix-safe-area-left)");
}

static void test_render_variables()
{
  SafeArea safe_area;

  const std::string rendered = safe_area.render_variables();

  assert(
      rendered ==
      ":root {\n"
      "  --vix-safe-area-top: env(safe-area-inset-top);\n"
      "  --vix-safe-area-right: env(safe-area-inset-right);\n"
      "  --vix-safe-area-bottom: env(safe-area-inset-bottom);\n"
      "  --vix-safe-area-left: env(safe-area-inset-left);\n"
      "}");
}

static void test_render_class()
{
  SafeArea safe_area;

  const std::string rendered = safe_area.render_class();

  assert(
      rendered ==
      ".vix-safe-area {\n"
      "  padding-top: var(--vix-safe-area-top);\n"
      "  padding-right: var(--vix-safe-area-right);\n"
      "  padding-bottom: var(--vix-safe-area-bottom);\n"
      "  padding-left: var(--vix-safe-area-left);\n"
      "}");
}

static void test_render()
{
  SafeArea safe_area;

  const std::string rendered = safe_area.render();

  assert(
      rendered ==
      ":root {\n"
      "  --vix-safe-area-top: env(safe-area-inset-top);\n"
      "  --vix-safe-area-right: env(safe-area-inset-right);\n"
      "  --vix-safe-area-bottom: env(safe-area-inset-bottom);\n"
      "  --vix-safe-area-left: env(safe-area-inset-left);\n"
      "}\n"
      ".vix-safe-area {\n"
      "  padding-top: var(--vix-safe-area-top);\n"
      "  padding-right: var(--vix-safe-area-right);\n"
      "  padding-bottom: var(--vix-safe-area-bottom);\n"
      "  padding-left: var(--vix-safe-area-left);\n"
      "}");
}

static void test_render_vertical()
{
  SafeArea safe_area = SafeArea::vertical();

  assert(
      safe_area.render_variables() ==
      ":root {\n"
      "  --vix-safe-area-top: env(safe-area-inset-top);\n"
      "  --vix-safe-area-bottom: env(safe-area-inset-bottom);\n"
      "}");

  assert(
      safe_area.render_class() ==
      ".vix-safe-area {\n"
      "  padding-top: var(--vix-safe-area-top);\n"
      "  padding-bottom: var(--vix-safe-area-bottom);\n"
      "}");
}

static void test_render_custom_values()
{
  SafeArea safe_area;

  safe_area.set_root_selector("html")
      .set_class_name("mobile-safe")
      .set_variable_prefix("--app-safe")
      .set_top("10px")
      .set_right("20px")
      .set_bottom("30px")
      .set_left("40px");

  assert(
      safe_area.render() ==
      "html {\n"
      "  --app-safe-top: 10px;\n"
      "  --app-safe-right: 20px;\n"
      "  --app-safe-bottom: 30px;\n"
      "  --app-safe-left: 40px;\n"
      "}\n"
      ".mobile-safe {\n"
      "  padding-top: var(--app-safe-top);\n"
      "  padding-right: var(--app-safe-right);\n"
      "  padding-bottom: var(--app-safe-bottom);\n"
      "  padding-left: var(--app-safe-left);\n"
      "}");
}

static void test_render_variables_without_root_selector()
{
  SafeArea safe_area;

  safe_area.set_root_selector("");

  assert(safe_area.render_variables().empty());
  assert(!safe_area.render_class().empty());
  assert(safe_area.render() == safe_area.render_class());
}

static void test_render_class_without_class_name()
{
  SafeArea safe_area;

  safe_area.set_class_name("");

  assert(!safe_area.render_variables().empty());
  assert(safe_area.render_class().empty());
  assert(safe_area.render() == safe_area.render_variables());
}

int main()
{
  test_safe_area_edge_to_string();

  test_default_safe_area();
  test_make_factory();
  test_all_factory();
  test_vertical_factory();

  test_setters();
  test_set_by_edge();
  test_empty_values();

  test_variable_name();
  test_variable_ref();

  test_render_variables();
  test_render_class();
  test_render();
  test_render_vertical();
  test_render_custom_values();
  test_render_variables_without_root_selector();
  test_render_class_without_class_name();

  std::cout << "[OK] ui safe area tests passed\n";
  return 0;
}
