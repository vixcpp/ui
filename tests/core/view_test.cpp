/**
 *
 *  @file view_test.cpp
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
#include <memory>
#include <string>

#include <vix/template/Engine.hpp>
#include <vix/template/StringLoader.hpp>
#include <vix/ui/core/View.hpp>
#include <vix/ui/support/Error.hpp>

using namespace vix::ui;

static vix::template_::Engine make_engine()
{
  auto loader = std::make_shared<vix::template_::StringLoader>();

  loader->set("home.html", "Hello {{ name }}");
  loader->set("title.html", "{{ page_title }}");
  loader->set("dashboard.html", "{{ title }}: {{ users }} users");

  return vix::template_::Engine(loader);
}

static void test_empty_view()
{
  View view;

  assert(!view.has_template());
  assert(!view.has_layout());
  assert(!view.has_title());
  assert(view.template_name().empty());
  assert(view.layout_name().empty());
  assert(view.title().empty());
  assert(view.values().empty());
}

static void test_construct_with_template_name()
{
  View view("home.html");

  assert(view.has_template());
  assert(view.template_name() == "home.html");
}

static void test_make_view()
{
  View view = View::make("home.html");

  assert(view.has_template());
  assert(view.template_name() == "home.html");
}

static void test_set_template()
{
  View view;

  view.set_template("home.html");

  assert(view.has_template());
  assert(view.template_name() == "home.html");
}

static void test_set_layout()
{
  View view("home.html");

  view.set_layout("base.html");

  assert(view.has_layout());
  assert(view.layout_name() == "base.html");
}

static void test_set_title()
{
  View view("title.html");

  view.set_title("Dashboard");

  assert(view.has_title());
  assert(view.title() == "Dashboard");
  assert(view.has("page_title"));

  const auto *value = view.get("page_title");
  assert(value != nullptr);
  assert(value->as_string() == "Dashboard");
}

static void test_set_and_get_value()
{
  View view("home.html");

  view.set("name", "Gaspard");

  assert(view.has("name"));

  const auto *value = view.get("name");
  assert(value != nullptr);
  assert(value->is_string());
  assert(value->as_string() == "Gaspard");
}

static void test_set_replaces_value()
{
  View view("home.html");

  view.set("name", "Alice");
  view.set("name", "Bob");

  assert(view.values().size() == 1);
  assert(view.get("name")->as_string() == "Bob");
}

static void test_merge_values()
{
  View view("dashboard.html");

  vix::template_::Object values;
  values["title"] = "Dashboard";
  values["users"] = 42;

  view.merge(values);

  assert(view.has("title"));
  assert(view.has("users"));
  assert(view.get("title")->as_string() == "Dashboard");
  assert(view.get("users")->as_int() == 42);
}

static void test_erase_value()
{
  View view("home.html");

  view.set("name", "Gaspard");
  assert(view.has("name"));

  const bool removed = view.erase("name");

  assert(removed);
  assert(!view.has("name"));
}

static void test_clear_values()
{
  View view("home.html");

  view.set("name", "Gaspard");
  view.set("role", "builder");

  assert(view.values().size() == 2);

  view.clear_values();

  assert(view.values().empty());
}

static void test_context()
{
  View view("home.html");

  view.set("name", "Gaspard");

  ViewContext ctx = view.context();

  assert(ctx.has("name"));
  assert(ctx.get("name")->as_string() == "Gaspard");
}

static void test_to_template_context()
{
  View view("home.html");

  view.set("name", "Gaspard");

  const auto ctx = view.to_template_context();

  const auto *value = ctx.get("name");

  assert(value != nullptr);
  assert(value->as_string() == "Gaspard");
}

static void test_render()
{
  auto engine = make_engine();

  View view("home.html");
  view.set("name", "Gaspard");

  const ViewResult result = view.render(engine);

  assert(result.success);
  assert(result.template_name == "home.html");
  assert(result.output == "Hello Gaspard");
  assert(result.escaped);
}

static void test_render_with_title()
{
  auto engine = make_engine();

  View view("title.html");
  view.set_title("Dashboard");

  const ViewResult result = view.render(engine);

  assert(result.success);
  assert(result.output == "Dashboard");
}

static void test_render_with_merged_values()
{
  auto engine = make_engine();

  View view("dashboard.html");

  vix::template_::Object values;
  values["title"] = "Stats";
  values["users"] = 128;

  view.merge(values);

  const ViewResult result = view.render(engine);

  assert(result.success);
  assert(result.output == "Stats: 128 users");
}

static void test_render_without_template_throws()
{
  auto engine = make_engine();

  View view;

  bool thrown = false;

  try
  {
    (void)view.render(engine);
  }
  catch (const ViewError &)
  {
    thrown = true;
  }

  assert(thrown);
}

int main()
{
  test_empty_view();
  test_construct_with_template_name();
  test_make_view();
  test_set_template();
  test_set_layout();
  test_set_title();
  test_set_and_get_value();
  test_set_replaces_value();
  test_merge_values();
  test_erase_value();
  test_clear_values();
  test_context();
  test_to_template_context();
  test_render();
  test_render_with_title();
  test_render_with_merged_values();
  test_render_without_template_throws();

  std::cout << "[OK] ui view tests passed\n";
  return 0;
}
