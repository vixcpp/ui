/**
 *
 *  @file html_test.cpp
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

#include <vix/ui/html/Html.hpp>
#include <vix/ui/html/HtmlAttrs.hpp>
#include <vix/ui/html/HtmlEscape.hpp>
#include <vix/ui/support/Error.hpp>

using namespace vix::ui;

static void test_html_escape_text()
{
  const std::string result =
      HtmlEscape::text(R"(<div class="x">Tom & 'Jerry'</div>)");

  assert(
      result ==
      "&lt;div class=&quot;x&quot;&gt;Tom &amp; &#39;Jerry&#39;&lt;/div&gt;");
}

static void test_html_escape_attr()
{
  const std::string result = HtmlEscape::attr(R"(A&B"C'D)");

  assert(result == "A&amp;B&quot;C&#39;D");
}

static void test_html_escape_plain_text()
{
  const std::string result = HtmlEscape::text("hello world");

  assert(result == "hello world");
}

static void test_html_escape_empty()
{
  const std::string result = HtmlEscape::text("");

  assert(result.empty());
}

static void test_needs_escape()
{
  assert(HtmlEscape::needs_escape('&'));
  assert(HtmlEscape::needs_escape('<'));
  assert(HtmlEscape::needs_escape('>'));
  assert(HtmlEscape::needs_escape('"'));
  assert(HtmlEscape::needs_escape('\''));

  assert(!HtmlEscape::needs_escape('a'));
  assert(!HtmlEscape::needs_escape('1'));
  assert(!HtmlEscape::needs_escape(' '));
}

static void test_attrs_empty()
{
  HtmlAttrs attrs;

  assert(attrs.empty());
  assert(attrs.size() == 0);
  assert(attrs.render().empty());
  assert(attrs.render_with_leading_space().empty());
}

static void test_attrs_set_and_render()
{
  HtmlAttrs attrs;

  attrs.set("class", "card");
  attrs.set("id", "main");

  assert(!attrs.empty());
  assert(attrs.size() == 2);
  assert(attrs.has("class"));
  assert(attrs.has("id"));

  assert(attrs.render() == R"(class="card" id="main")");
  assert(attrs.render_with_leading_space() == R"( class="card" id="main")");
}

static void test_attrs_escape_values()
{
  HtmlAttrs attrs;

  attrs.set("title", R"(<hello & "world">)");

  assert(attrs.render() == R"(title="&lt;hello &amp; &quot;world&quot;&gt;")");
}

static void test_attrs_boolean()
{
  HtmlAttrs attrs;

  attrs.boolean("disabled", true);

  assert(attrs.has("disabled"));
  assert(attrs.render() == "disabled");

  attrs.boolean("disabled", false);

  assert(!attrs.has("disabled"));
  assert(attrs.empty());
}

static void test_attrs_set_if()
{
  HtmlAttrs attrs;

  attrs.set_if(false, "class", "hidden");
  assert(attrs.empty());

  attrs.set_if(true, "class", "visible");
  assert(attrs.has("class"));
  assert(attrs.render() == R"(class="visible")");
}

static void test_attrs_invalid_name_throws()
{
  HtmlAttrs attrs;

  bool thrown = false;

  try
  {
    attrs.set("1bad", "value");
  }
  catch (const HtmlError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_attrs_erase()
{
  HtmlAttrs attrs;

  attrs.set("class", "card");
  assert(attrs.has("class"));

  const bool removed = attrs.erase("class");

  assert(removed);
  assert(!attrs.has("class"));
}

static void test_attrs_clear()
{
  HtmlAttrs attrs;

  attrs.set("class", "card");
  attrs.set("id", "main");

  assert(attrs.size() == 2);

  attrs.clear();

  assert(attrs.empty());
}

static void test_valid_attr_names()
{
  assert(HtmlAttrs::is_valid_name("class"));
  assert(HtmlAttrs::is_valid_name("data-id"));
  assert(HtmlAttrs::is_valid_name("aria-label"));
  assert(HtmlAttrs::is_valid_name("_private"));
  assert(HtmlAttrs::is_valid_name(":name"));
  assert(HtmlAttrs::is_valid_name("x.y"));

  assert(!HtmlAttrs::is_valid_name(""));
  assert(!HtmlAttrs::is_valid_name("1bad"));
  assert(!HtmlAttrs::is_valid_name("bad name"));
  assert(!HtmlAttrs::is_valid_name("bad/name"));
}

static void test_html_text()
{
  assert(Html::text("<b>hello</b>") == "&lt;b&gt;hello&lt;/b&gt;");
}

static void test_html_raw()
{
  assert(Html::raw("<strong>ok</strong>") == "<strong>ok</strong>");
}

static void test_html_doctype()
{
  assert(Html::doctype() == "<!doctype html>");
}

static void test_html_open_tag()
{
  HtmlAttrs attrs;
  attrs.set("class", "card");

  const std::string result = Html::open_tag("div", attrs);

  assert(result == R"(<div class="card">)");
}

static void test_html_close_tag()
{
  assert(Html::close_tag("div") == "</div>");
}

static void test_html_tag()
{
  HtmlAttrs attrs;
  attrs.set("class", "title");

  const std::string result =
      Html::tag("h1", Html::text("Hello <Vix>"), attrs);

  assert(result == R"(<h1 class="title">Hello &lt;Vix&gt;</h1>)");
}

static void test_html_void_tag()
{
  HtmlAttrs attrs;
  attrs.set("charset", "utf-8");

  const std::string result = Html::void_tag("meta", attrs);

  assert(result == R"(<meta charset="utf-8">)");
}

static void test_html_invalid_tag_throws()
{
  bool thrown = false;

  try
  {
    (void)Html::tag("1bad", "content");
  }
  catch (const HtmlError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_valid_tag_names()
{
  assert(Html::is_valid_tag_name("div"));
  assert(Html::is_valid_tag_name("custom-element"));
  assert(Html::is_valid_tag_name("svg:path"));

  assert(!Html::is_valid_tag_name(""));
  assert(!Html::is_valid_tag_name("1div"));
  assert(!Html::is_valid_tag_name("bad name"));
  assert(!Html::is_valid_tag_name("bad/name"));
}

int main()
{
  test_html_escape_text();
  test_html_escape_attr();
  test_html_escape_plain_text();
  test_html_escape_empty();
  test_needs_escape();

  test_attrs_empty();
  test_attrs_set_and_render();
  test_attrs_escape_values();
  test_attrs_boolean();
  test_attrs_set_if();
  test_attrs_invalid_name_throws();
  test_attrs_erase();
  test_attrs_clear();
  test_valid_attr_names();

  test_html_text();
  test_html_raw();
  test_html_doctype();
  test_html_open_tag();
  test_html_close_tag();
  test_html_tag();
  test_html_void_tag();
  test_html_invalid_tag_throws();
  test_valid_tag_names();

  std::cout << "[OK] ui html tests passed\n";
  return 0;
}
