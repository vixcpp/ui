/**
 *
 *  @file live_update_test.cpp
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
#include <vix/ui/live/LiveUpdate.hpp>

using namespace vix::ui;

static void test_live_update_action_to_string()
{
  assert(to_string(LiveUpdateAction::Replace) == "replace");
  assert(to_string(LiveUpdateAction::Append) == "append");
  assert(to_string(LiveUpdateAction::Prepend) == "prepend");
  assert(to_string(LiveUpdateAction::Before) == "before");
  assert(to_string(LiveUpdateAction::After) == "after");
  assert(to_string(LiveUpdateAction::Remove) == "remove");
  assert(to_string(LiveUpdateAction::None) == "none");
}

static void test_empty_live_update()
{
  LiveUpdate update;

  assert(update.action() == LiveUpdateAction::None);
  assert(update.target().empty());
  assert(update.event().empty());
  assert(update.id().empty());

  assert(!update.has_target());
  assert(!update.has_fragment());
  assert(!update.has_event());
  assert(!update.has_id());

  assert(update.none_action());
  assert(!update.removes_target());
  assert(update.render().empty());

  assert(update.to_json() == R"({"type":"ui.update","action":"none"})");
}

static void test_construct_live_update()
{
  Fragment fragment("stats", "<span>42</span>");

  LiveUpdate update(
      LiveUpdateAction::Replace,
      "#stats",
      fragment);

  assert(update.action() == LiveUpdateAction::Replace);
  assert(update.target() == "#stats");
  assert(update.fragment().name() == "stats");
  assert(update.fragment().html() == "<span>42</span>");

  assert(update.has_target());
  assert(update.has_fragment());
  assert(!update.none_action());
  assert(!update.removes_target());

  assert(update.render() == "<span>42</span>");
}

static void test_replace_update()
{
  Fragment fragment("stats", "<span>42 users</span>");

  LiveUpdate update = LiveUpdate::replace("#stats", fragment);

  assert(update.action() == LiveUpdateAction::Replace);
  assert(update.target() == "#stats");
  assert(update.has_target());
  assert(update.has_fragment());
  assert(update.render() == "<span>42 users</span>");
}

static void test_append_update()
{
  Fragment fragment("message", "<li>Hello</li>");

  LiveUpdate update = LiveUpdate::append("#messages", fragment);

  assert(update.action() == LiveUpdateAction::Append);
  assert(update.target() == "#messages");
  assert(update.render() == "<li>Hello</li>");
}

static void test_prepend_update()
{
  Fragment fragment("message", "<li>First</li>");

  LiveUpdate update = LiveUpdate::prepend("#messages", fragment);

  assert(update.action() == LiveUpdateAction::Prepend);
  assert(update.target() == "#messages");
  assert(update.render() == "<li>First</li>");
}

static void test_before_update()
{
  Fragment fragment("banner", "<div>Before</div>");

  LiveUpdate update = LiveUpdate::before("#content", fragment);

  assert(update.action() == LiveUpdateAction::Before);
  assert(update.target() == "#content");
  assert(update.render() == "<div>Before</div>");
}

static void test_after_update()
{
  Fragment fragment("banner", "<div>After</div>");

  LiveUpdate update = LiveUpdate::after("#content", fragment);

  assert(update.action() == LiveUpdateAction::After);
  assert(update.target() == "#content");
  assert(update.render() == "<div>After</div>");
}

static void test_remove_update()
{
  LiveUpdate update = LiveUpdate::remove("#toast");

  assert(update.action() == LiveUpdateAction::Remove);
  assert(update.target() == "#toast");

  assert(update.has_target());
  assert(!update.has_fragment());
  assert(update.removes_target());
  assert(!update.none_action());

  assert(update.render().empty());
}

static void test_none_update()
{
  LiveUpdate update = LiveUpdate::none();

  assert(update.action() == LiveUpdateAction::None);
  assert(update.none_action());
  assert(!update.removes_target());
  assert(update.render().empty());
}

static void test_setters()
{
  Fragment fragment("row", "<tr><td>Gaspard</td></tr>");

  LiveUpdate update;

  update.set_action(LiveUpdateAction::Replace)
      .set_target("#row-1")
      .set_fragment(fragment)
      .set_event("users.updated")
      .set_id("req-123");

  assert(update.action() == LiveUpdateAction::Replace);
  assert(update.target() == "#row-1");
  assert(update.fragment().name() == "row");
  assert(update.fragment().html() == "<tr><td>Gaspard</td></tr>");
  assert(update.event() == "users.updated");
  assert(update.id() == "req-123");

  assert(update.has_target());
  assert(update.has_fragment());
  assert(update.has_event());
  assert(update.has_id());
}

static void test_mutable_fragment_access()
{
  LiveUpdate update = LiveUpdate::replace("#stats", Fragment::make("stats"));

  update.fragment().set_html("<span>100</span>");

  assert(update.has_fragment());
  assert(update.render() == "<span>100</span>");
}

static void test_to_json_replace()
{
  Fragment fragment("stats", "<span>42</span>");

  LiveUpdate update = LiveUpdate::replace("#stats", fragment);

  assert(
      update.to_json() ==
      R"({"type":"ui.update","action":"replace","target":"#stats","fragment":"stats","html":"<span>42</span>"})");
}

static void test_to_json_append()
{
  Fragment fragment("message", "<li>Hello</li>");

  LiveUpdate update = LiveUpdate::append("#messages", fragment);

  assert(
      update.to_json() ==
      R"({"type":"ui.update","action":"append","target":"#messages","fragment":"message","html":"<li>Hello</li>"})");
}

static void test_to_json_remove()
{
  LiveUpdate update = LiveUpdate::remove("#toast");

  assert(
      update.to_json() ==
      R"({"type":"ui.update","action":"remove","target":"#toast"})");
}

static void test_to_json_with_event_and_id()
{
  Fragment fragment("stats", "<span>42</span>");

  LiveUpdate update = LiveUpdate::replace("#stats", fragment);

  update.set_event("dashboard.stats")
      .set_id("update-42");

  assert(
      update.to_json() ==
      R"({"type":"ui.update","action":"replace","target":"#stats","event":"dashboard.stats","id":"update-42","fragment":"stats","html":"<span>42</span>"})");
}

static void test_to_json_escapes_strings()
{
  Fragment fragment(
      R"(frag "main")",
      "<span>Line 1\nLine 2</span>");

  LiveUpdate update = LiveUpdate::replace(R"(#stats "main")", fragment);

  update.set_event(R"(event\name)")
      .set_id("id\t42");

  assert(
      update.to_json() ==
      "{\"type\":\"ui.update\",\"action\":\"replace\",\"target\":\"#stats \\\"main\\\"\",\"event\":\"event\\\\name\",\"id\":\"id\\t42\",\"fragment\":\"frag \\\"main\\\"\",\"html\":\"<span>Line 1\\nLine 2</span>\"}");
}

static void test_to_json_without_fragment_name()
{
  Fragment fragment = Fragment::html("<span>OK</span>");

  LiveUpdate update = LiveUpdate::replace("#status", fragment);

  assert(
      update.to_json() ==
      R"({"type":"ui.update","action":"replace","target":"#status","html":"<span>OK</span>"})");
}

static void test_render_remove_and_none_are_empty()
{
  Fragment fragment("unused", "<div>unused</div>");

  LiveUpdate remove = LiveUpdate::remove("#old");
  LiveUpdate none = LiveUpdate::none();

  remove.set_fragment(fragment);
  none.set_fragment(fragment);

  assert(remove.render().empty());
  assert(none.render().empty());
}

int main()
{
  test_live_update_action_to_string();

  test_empty_live_update();
  test_construct_live_update();

  test_replace_update();
  test_append_update();
  test_prepend_update();
  test_before_update();
  test_after_update();
  test_remove_update();
  test_none_update();

  test_setters();
  test_mutable_fragment_access();

  test_to_json_replace();
  test_to_json_append();
  test_to_json_remove();
  test_to_json_with_event_and_id();
  test_to_json_escapes_strings();
  test_to_json_without_fragment_name();

  test_render_remove_and_none_are_empty();

  std::cout << "[OK] ui live update tests passed\n";
  return 0;
}
