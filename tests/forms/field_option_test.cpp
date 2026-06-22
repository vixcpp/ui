/**
 *
 *  @file field_option_test.cpp
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

#include <vix/ui/forms/FieldOption.hpp>

using namespace vix::ui;

static void test_empty_option()
{
  FieldOption option;

  assert(option.value().empty());
  assert(option.label().empty());
  assert(!option.selected());
  assert(!option.disabled());
  assert(option.attrs().empty());

  assert(!option.has_value());
  assert(!option.has_label());
  assert(option.display_label().empty());
}

static void test_construct_with_value()
{
  FieldOption option("ug");

  assert(option.value() == "ug");
  assert(option.label().empty());

  assert(option.has_value());
  assert(!option.has_label());
  assert(option.display_label() == "ug");
}

static void test_construct_with_value_and_label()
{
  FieldOption option("ug", "Uganda");

  assert(option.value() == "ug");
  assert(option.label() == "Uganda");

  assert(option.has_value());
  assert(option.has_label());
  assert(option.display_label() == "Uganda");
}

static void test_make_option()
{
  FieldOption option = FieldOption::make("cd", "DRC");

  assert(option.value() == "cd");
  assert(option.label() == "DRC");
  assert(option.display_label() == "DRC");
}

static void test_make_empty_option()
{
  FieldOption option = FieldOption::make();

  assert(option.value().empty());
  assert(option.label().empty());
  assert(option.display_label().empty());
}

static void test_setters()
{
  FieldOption option;

  option.set_value("us")
      .set_label("United States")
      .set_selected(true)
      .set_disabled(true);

  assert(option.value() == "us");
  assert(option.label() == "United States");
  assert(option.selected());
  assert(option.disabled());

  assert(option.has_value());
  assert(option.has_label());
  assert(option.display_label() == "United States");
}

static void test_selected_can_be_disabled()
{
  FieldOption option("ug", "Uganda");

  option.set_selected(true);
  assert(option.selected());

  option.set_selected(false);
  assert(!option.selected());
}

static void test_disabled_can_be_disabled()
{
  FieldOption option("ug", "Uganda");

  option.set_disabled(true);
  assert(option.disabled());

  option.set_disabled(false);
  assert(!option.disabled());
}

static void test_attrs()
{
  FieldOption option("ug", "Uganda");

  option.set_attr("class", "country-option");
  option.set_bool_attr("hidden", true);

  assert(option.attrs().has("class"));
  assert(option.attrs().has("hidden"));
  assert(option.attrs().size() == 2);
}

static void test_bool_attr_can_be_removed()
{
  FieldOption option("ug", "Uganda");

  option.set_bool_attr("hidden", true);
  assert(option.attrs().has("hidden"));

  option.set_bool_attr("hidden", false);
  assert(!option.attrs().has("hidden"));
}

static void test_render_basic_option()
{
  FieldOption option("ug", "Uganda");

  const std::string rendered = option.render();

  assert(rendered == R"(<option value="ug">Uganda</option>)");
}

static void test_render_uses_value_as_label_when_label_is_empty()
{
  FieldOption option("ug");

  const std::string rendered = option.render();

  assert(rendered == R"(<option value="ug">ug</option>)");
}

static void test_render_selected_option()
{
  FieldOption option("ug", "Uganda");

  option.set_selected(true);

  const std::string rendered = option.render();

  assert(rendered == R"(<option selected value="ug">Uganda</option>)");
}

static void test_render_disabled_option()
{
  FieldOption option("ug", "Uganda");

  option.set_disabled(true);

  const std::string rendered = option.render();

  assert(rendered == R"(<option disabled value="ug">Uganda</option>)");
}

static void test_render_selected_and_disabled_option()
{
  FieldOption option("ug", "Uganda");

  option.set_selected(true)
      .set_disabled(true);

  const std::string rendered = option.render();

  assert(rendered == R"(<option disabled selected value="ug">Uganda</option>)");
}

static void test_render_with_custom_attrs()
{
  FieldOption option("ug", "Uganda");

  option.set_attr("class", "country-option");
  option.set_attr("data-code", "256");
  option.set_selected(true);

  const std::string rendered = option.render();

  assert(
      rendered ==
      R"(<option class="country-option" data-code="256" selected value="ug">Uganda</option>)");
}

static void test_render_escapes_value_and_label()
{
  FieldOption option(R"(ug "main")", R"(<Uganda & DRC>)");

  const std::string rendered = option.render();

  assert(
      rendered ==
      R"(<option value="ug &quot;main&quot;">&lt;Uganda &amp; DRC&gt;</option>)");
}

static void test_render_escapes_custom_attrs()
{
  FieldOption option("ug", "Uganda");

  option.set_attr("title", R"(Country "main" & active)");

  const std::string rendered = option.render();

  assert(
      rendered ==
      R"(<option title="Country &quot;main&quot; &amp; active" value="ug">Uganda</option>)");
}

int main()
{
  test_empty_option();
  test_construct_with_value();
  test_construct_with_value_and_label();
  test_make_option();
  test_make_empty_option();

  test_setters();
  test_selected_can_be_disabled();
  test_disabled_can_be_disabled();

  test_attrs();
  test_bool_attr_can_be_removed();

  test_render_basic_option();
  test_render_uses_value_as_label_when_label_is_empty();
  test_render_selected_option();
  test_render_disabled_option();
  test_render_selected_and_disabled_option();
  test_render_with_custom_attrs();
  test_render_escapes_value_and_label();
  test_render_escapes_custom_attrs();

  std::cout << "[OK] ui field option tests passed\n";
  return 0;
}
