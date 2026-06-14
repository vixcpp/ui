/**
 *
 *  @file view_context_test.cpp
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

#include <vix/ui/core/ViewContext.hpp>

using namespace vix::ui;

static void test_empty_context()
{
  ViewContext ctx;

  assert(ctx.empty());
  assert(ctx.size() == 0);
  assert(!ctx.has("name"));
}

static void test_set_and_get_value()
{
  ViewContext ctx;

  ctx.set("name", "Gaspard");

  assert(!ctx.empty());
  assert(ctx.size() == 1);
  assert(ctx.has("name"));

  const auto *value = ctx.get("name");
  assert(value != nullptr);
  assert(value->is_string());
  assert(value->as_string() == "Gaspard");
}

static void test_set_replaces_existing_value()
{
  ViewContext ctx;

  ctx.set("name", "Alice");
  ctx.set("name", "Bob");

  assert(ctx.size() == 1);

  const auto *value = ctx.get("name");
  assert(value != nullptr);
  assert(value->as_string() == "Bob");
}

static void test_get_missing_value()
{
  ViewContext ctx;

  assert(ctx.get("missing") == nullptr);
}

static void test_get_or()
{
  ViewContext ctx;

  assert(ctx.get_or("name", "fallback").as_string() == "fallback");

  ctx.set("name", "Gaspard");

  assert(ctx.get_or("name", "fallback").as_string() == "Gaspard");
}

static void test_erase()
{
  ViewContext ctx;

  ctx.set("name", "Gaspard");
  assert(ctx.has("name"));

  const bool removed = ctx.erase("name");

  assert(removed);
  assert(!ctx.has("name"));
  assert(ctx.empty());
}

static void test_erase_missing_value()
{
  ViewContext ctx;

  const bool removed = ctx.erase("missing");

  assert(!removed);
}

static void test_clear()
{
  ViewContext ctx;

  ctx.set("name", "Gaspard");
  ctx.set("role", "builder");

  assert(ctx.size() == 2);

  ctx.clear();

  assert(ctx.empty());
  assert(ctx.size() == 0);
}

static void test_values_access()
{
  ViewContext ctx;

  ctx.set("name", "Gaspard");

  const auto &values = ctx.values();

  assert(values.size() == 1);
  assert(values.at("name").as_string() == "Gaspard");
}

static void test_mutable_values_access()
{
  ViewContext ctx;

  ctx.values()["name"] = "Gaspard";

  assert(ctx.has("name"));
  assert(ctx.get("name")->as_string() == "Gaspard");
}

static void test_to_template_context()
{
  ViewContext ctx;

  ctx.set("name", "Gaspard");
  ctx.set("active", true);

  const auto template_ctx = ctx.to_template_context();

  const auto *name = template_ctx.get("name");
  const auto *active = template_ctx.get("active");

  assert(name != nullptr);
  assert(active != nullptr);

  assert(name->as_string() == "Gaspard");
  assert(active->as_bool());
}

int main()
{
  test_empty_context();
  test_set_and_get_value();
  test_set_replaces_existing_value();
  test_get_missing_value();
  test_get_or();
  test_erase();
  test_erase_missing_value();
  test_clear();
  test_values_access();
  test_mutable_values_access();
  test_to_template_context();

  std::cout << "[OK] ui view context tests passed\n";
  return 0;
}
