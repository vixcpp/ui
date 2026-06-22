/**
 *
 *  @file fragment_test.cpp
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

#include <vix/ui/live/Fragment.hpp>

using namespace vix::ui;

static void test_empty_fragment()
{
  Fragment fragment;

  assert(fragment.name().empty());
  assert(fragment.target().empty());
  assert(fragment.html().empty());
  assert(fragment.attrs().empty());

  assert(!fragment.has_name());
  assert(!fragment.has_target());
  assert(!fragment.has_html());
  assert(fragment.empty());
  assert(fragment.size() == 0);

  assert(fragment.render().empty());
}

static void test_fragment_with_name()
{
  Fragment fragment("stats-card");

  assert(fragment.name() == "stats-card");
  assert(fragment.has_name());
  assert(!fragment.has_html());
  assert(fragment.empty());
}

static void test_fragment_with_name_and_html()
{
  Fragment fragment("user-row", "<tr><td>Gaspard</td></tr>");

  assert(fragment.name() == "user-row");
  assert(fragment.html() == "<tr><td>Gaspard</td></tr>");

  assert(fragment.has_name());
  assert(fragment.has_html());
  assert(!fragment.empty());
  assert(fragment.size() == 25);
}

static void test_make_fragment()
{
  Fragment fragment = Fragment::make("dashboard-stats");

  assert(fragment.name() == "dashboard-stats");
  assert(fragment.has_name());
  assert(!fragment.has_html());
}

static void test_html_factory()
{
  Fragment fragment = Fragment::html("<p>Saved</p>");

  assert(fragment.name().empty());
  assert(fragment.html() == "<p>Saved</p>");

  assert(!fragment.has_name());
  assert(fragment.has_html());
  assert(!fragment.empty());
}

static void test_setters()
{
  Fragment fragment;

  fragment.set_name("notification")
      .set_target("#notifications")
      .set_html("<div>New message</div>");

  assert(fragment.name() == "notification");
  assert(fragment.target() == "#notifications");
  assert(fragment.html() == "<div>New message</div>");

  assert(fragment.has_name());
  assert(fragment.has_target());
  assert(fragment.has_html());
}

static void test_set_html_replaces_previous_html()
{
  Fragment fragment("status");

  fragment.set_html("<span>offline</span>");
  fragment.set_html("<span>online</span>");

  assert(fragment.html() == "<span>online</span>");
  assert(fragment.render() == "<span>online</span>");
}

static void test_attrs()
{
  Fragment fragment("card");

  fragment.set_attr("class", "card live-card");
  fragment.set_bool_attr("hidden", true);

  assert(fragment.attrs().has("class"));
  assert(fragment.attrs().has("hidden"));
  assert(fragment.attrs().size() == 2);
}

static void test_bool_attr_can_be_removed()
{
  Fragment fragment("card");

  fragment.set_bool_attr("hidden", true);
  assert(fragment.attrs().has("hidden"));

  fragment.set_bool_attr("hidden", false);
  assert(!fragment.attrs().has("hidden"));
}

static void test_render_returns_raw_html()
{
  Fragment fragment("stats");

  fragment.set_html("<strong>42</strong>");

  assert(fragment.render() == "<strong>42</strong>");
}

static void test_render_wrapped_empty_fragment()
{
  Fragment fragment;

  const std::string rendered = fragment.render_wrapped();

  assert(rendered == "<div></div>");
}

static void test_render_wrapped_with_name()
{
  Fragment fragment("stats");

  fragment.set_html("<span>42 users</span>");

  const std::string rendered = fragment.render_wrapped();

  assert(rendered == R"(<div data-fragment="stats"><span>42 users</span></div>)");
}

static void test_render_wrapped_with_target()
{
  Fragment fragment("stats");

  fragment.set_target("#stats");
  fragment.set_html("<span>42 users</span>");

  const std::string rendered = fragment.render_wrapped();

  assert(
      rendered ==
      R"(<div data-fragment="stats" data-target="#stats"><span>42 users</span></div>)");
}

static void test_render_wrapped_with_custom_tag()
{
  Fragment fragment("row");

  fragment.set_html("<td>Gaspard</td>");

  const std::string rendered = fragment.render_wrapped("tr");

  assert(rendered == R"(<tr data-fragment="row"><td>Gaspard</td></tr>)");
}

static void test_render_wrapped_with_attrs()
{
  Fragment fragment("stats");

  fragment.set_target("#stats");
  fragment.set_attr("class", "card");
  fragment.set_bool_attr("hidden", true);
  fragment.set_html("<span>42 users</span>");

  const std::string rendered = fragment.render_wrapped();

  assert(
      rendered ==
      R"(<div class="card" data-fragment="stats" data-target="#stats" hidden><span>42 users</span></div>)");
}

static void test_render_wrapped_escapes_attrs()
{
  Fragment fragment("stats");

  fragment.set_target(R"(#stats "main")");
  fragment.set_attr("title", R"(<Live & Ready>)");
  fragment.set_html("<span>42 users</span>");

  const std::string rendered = fragment.render_wrapped();

  assert(
      rendered ==
      R"(<div data-fragment="stats" data-target="#stats &quot;main&quot;" title="&lt;Live &amp; Ready&gt;"><span>42 users</span></div>)");
}

int main()
{
  test_empty_fragment();
  test_fragment_with_name();
  test_fragment_with_name_and_html();
  test_make_fragment();
  test_html_factory();

  test_setters();
  test_set_html_replaces_previous_html();

  test_attrs();
  test_bool_attr_can_be_removed();

  test_render_returns_raw_html();
  test_render_wrapped_empty_fragment();
  test_render_wrapped_with_name();
  test_render_wrapped_with_target();
  test_render_wrapped_with_custom_tag();
  test_render_wrapped_with_attrs();
  test_render_wrapped_escapes_attrs();

  std::cout << "[OK] ui fragment tests passed\n";
  return 0;
}
