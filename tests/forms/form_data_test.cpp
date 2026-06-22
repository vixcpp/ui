/**
 *
 *  @file form_data_test.cpp
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
#include <vector>

#include <vix/ui/forms/FormData.hpp>

using namespace vix::ui;

static void test_empty_form_data()
{
  FormData data;

  assert(data.empty());
  assert(data.size() == 0);
  assert(!data.has("email"));
  assert(data.get("email") == nullptr);
  assert(data.get_all("email") == nullptr);
  assert(data.value_count("email") == 0);
}

static void test_make_form_data()
{
  FormData data = FormData::make();

  assert(data.empty());
  assert(data.size() == 0);
}

static void test_construct_from_map()
{
  std::map<std::string, std::string> values;
  values["email"] = "hello@example.com";
  values["name"] = "Gaspard";

  FormData data(values);

  assert(!data.empty());
  assert(data.size() == 2);

  assert(data.has("email"));
  assert(data.has("name"));

  assert(data.get("email") != nullptr);
  assert(*data.get("email") == "hello@example.com");

  assert(data.get("name") != nullptr);
  assert(*data.get("name") == "Gaspard");
}

static void test_from_factory()
{
  std::map<std::string, std::string> values;
  values["country"] = "ug";
  values["city"] = "kampala";

  FormData data = FormData::from(values);

  assert(data.size() == 2);
  assert(data.get_or("country") == "ug");
  assert(data.get_or("city") == "kampala");
}

static void test_set_single_value()
{
  FormData data;

  data.set("email", "hello@example.com");

  assert(!data.empty());
  assert(data.size() == 1);
  assert(data.has("email"));
  assert(data.value_count("email") == 1);

  const std::string *value = data.get("email");

  assert(value != nullptr);
  assert(*value == "hello@example.com");
}

static void test_set_replaces_existing_values()
{
  FormData data;

  data.add("tags", "cpp");
  data.add("tags", "vix");

  assert(data.value_count("tags") == 2);

  data.set("tags", "ui");

  assert(data.value_count("tags") == 1);
  assert(data.get_or("tags") == "ui");
  assert(data.contains("tags", "ui"));
  assert(!data.contains("tags", "cpp"));
}

static void test_add_multiple_values()
{
  FormData data;

  data.add("roles", "admin");
  data.add("roles", "editor");
  data.add("roles", "viewer");

  assert(data.has("roles"));
  assert(data.value_count("roles") == 3);

  const std::vector<std::string> *roles = data.get_all("roles");

  assert(roles != nullptr);
  assert(roles->size() == 3);
  assert((*roles)[0] == "admin");
  assert((*roles)[1] == "editor");
  assert((*roles)[2] == "viewer");

  assert(data.get_or("roles") == "admin");
}

static void test_set_all_values()
{
  FormData data;

  data.set_all("colors", {"red", "green", "blue"});

  assert(data.has("colors"));
  assert(data.value_count("colors") == 3);

  assert(data.contains("colors", "red"));
  assert(data.contains("colors", "green"));
  assert(data.contains("colors", "blue"));
  assert(!data.contains("colors", "black"));
}

static void test_set_all_replaces_existing_values()
{
  FormData data;

  data.set_all("colors", {"red", "green"});
  assert(data.value_count("colors") == 2);

  data.set_all("colors", {"blue"});

  assert(data.value_count("colors") == 1);
  assert(data.contains("colors", "blue"));
  assert(!data.contains("colors", "red"));
}

static void test_set_all_empty_values()
{
  FormData data;

  data.set_all("items", {});

  assert(data.has("items"));
  assert(data.value_count("items") == 0);
  assert(data.get("items") == nullptr);

  const std::vector<std::string> *items = data.get_all("items");

  assert(items != nullptr);
  assert(items->empty());
}

static void test_contains()
{
  FormData data;

  data.add("features", "live");
  data.add("features", "forms");
  data.add("features", "assets");

  assert(data.contains("features", "live"));
  assert(data.contains("features", "forms"));
  assert(data.contains("features", "assets"));
  assert(!data.contains("features", "shell"));
  assert(!data.contains("missing", "live"));
}

static void test_get_missing_value()
{
  FormData data;

  assert(data.get("missing") == nullptr);
  assert(data.get_all("missing") == nullptr);
}

static void test_get_or()
{
  FormData data;

  assert(data.get_or("email", "fallback@example.com") == "fallback@example.com");

  data.set("email", "hello@example.com");

  assert(data.get_or("email", "fallback@example.com") == "hello@example.com");
}

static void test_get_or_empty_fallback()
{
  FormData data;

  assert(data.get_or("missing").empty());

  data.set("name", "Gaspard");

  assert(data.get_or("name") == "Gaspard");
}

static void test_get_all_or_empty()
{
  FormData data;

  std::vector<std::string> missing = data.get_all_or_empty("missing");

  assert(missing.empty());

  data.add("roles", "admin");
  data.add("roles", "editor");

  std::vector<std::string> roles = data.get_all_or_empty("roles");

  assert(roles.size() == 2);
  assert(roles[0] == "admin");
  assert(roles[1] == "editor");
}

static void test_erase()
{
  FormData data;

  data.set("email", "hello@example.com");

  assert(data.has("email"));

  const bool removed = data.erase("email");

  assert(removed);
  assert(!data.has("email"));
  assert(data.empty());
}

static void test_erase_missing()
{
  FormData data;

  const bool removed = data.erase("missing");

  assert(!removed);
}

static void test_clear()
{
  FormData data;

  data.set("email", "hello@example.com");
  data.set("name", "Gaspard");
  data.add("roles", "admin");

  assert(data.size() == 3);

  data.clear();

  assert(data.empty());
  assert(data.size() == 0);
  assert(!data.has("email"));
  assert(!data.has("name"));
  assert(!data.has("roles"));
}

static void test_values_access()
{
  FormData data;

  data.set("email", "hello@example.com");
  data.add("roles", "admin");
  data.add("roles", "editor");

  const auto &values = data.values();

  assert(values.size() == 2);
  assert(values.at("email").size() == 1);
  assert(values.at("email")[0] == "hello@example.com");
  assert(values.at("roles").size() == 2);
}

static void test_mutable_values_access()
{
  FormData data;

  data.values()["name"] = {"Gaspard"};
  data.values()["roles"] = {"admin", "editor"};

  assert(data.has("name"));
  assert(data.has("roles"));

  assert(data.get_or("name") == "Gaspard");
  assert(data.value_count("roles") == 2);
  assert(data.contains("roles", "admin"));
  assert(data.contains("roles", "editor"));
}

static void test_empty_string_value()
{
  FormData data;

  data.set("description", "");

  assert(data.has("description"));
  assert(data.value_count("description") == 1);

  const std::string *value = data.get("description");

  assert(value != nullptr);
  assert(value->empty());
  assert(data.get_or("description", "fallback").empty());
}

static void test_empty_field_name()
{
  FormData data;

  data.set("", "empty-name");

  assert(data.has(""));
  assert(data.get_or("") == "empty-name");
  assert(data.value_count("") == 1);
}

int main()
{
  test_empty_form_data();
  test_make_form_data();
  test_construct_from_map();
  test_from_factory();

  test_set_single_value();
  test_set_replaces_existing_values();
  test_add_multiple_values();
  test_set_all_values();
  test_set_all_replaces_existing_values();
  test_set_all_empty_values();

  test_contains();
  test_get_missing_value();
  test_get_or();
  test_get_or_empty_fallback();
  test_get_all_or_empty();

  test_erase();
  test_erase_missing();
  test_clear();

  test_values_access();
  test_mutable_values_access();

  test_empty_string_value();
  test_empty_field_name();

  std::cout << "[OK] ui form data tests passed\n";
  return 0;
}
