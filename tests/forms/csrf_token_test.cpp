/**
 *
 *  @file csrf_token_test.cpp
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

#include <vix/ui/forms/CsrfToken.hpp>

using namespace vix::ui;

static void test_default_csrf_token()
{
  CsrfToken token;

  assert(token.name() == "_token");
  assert(token.value().empty());
  assert(token.header_name() == "X-CSRF-Token");
  assert(token.attrs().empty());

  assert(token.has_name());
  assert(!token.has_value());
  assert(token.has_header_name());
  assert(token.empty());
}

static void test_construct_with_value()
{
  CsrfToken token("abc123");

  assert(token.name() == "_token");
  assert(token.value() == "abc123");
  assert(token.header_name() == "X-CSRF-Token");

  assert(token.has_name());
  assert(token.has_value());
  assert(token.has_header_name());
  assert(!token.empty());
}

static void test_construct_with_name_and_value()
{
  CsrfToken token("csrf", "abc123");

  assert(token.name() == "csrf");
  assert(token.value() == "abc123");
  assert(token.header_name() == "X-CSRF-Token");

  assert(token.has_name());
  assert(token.has_value());
  assert(!token.empty());
}

static void test_make_token()
{
  CsrfToken token = CsrfToken::make("abc123");

  assert(token.name() == "_token");
  assert(token.value() == "abc123");
  assert(token.has_value());
}

static void test_make_empty_token()
{
  CsrfToken token = CsrfToken::make();

  assert(token.name() == "_token");
  assert(token.value().empty());
  assert(token.empty());
}

static void test_named_token()
{
  CsrfToken token = CsrfToken::named("csrf_token", "abc123");

  assert(token.name() == "csrf_token");
  assert(token.value() == "abc123");
  assert(token.has_name());
  assert(token.has_value());
}

static void test_setters()
{
  CsrfToken token;

  token.set_name("csrf")
      .set_value("secure-token")
      .set_header_name("X-Vix-CSRF");

  assert(token.name() == "csrf");
  assert(token.value() == "secure-token");
  assert(token.header_name() == "X-Vix-CSRF");

  assert(token.has_name());
  assert(token.has_value());
  assert(token.has_header_name());
  assert(!token.empty());
}

static void test_empty_name()
{
  CsrfToken token("abc123");

  token.set_name("");

  assert(token.name().empty());
  assert(!token.has_name());
  assert(token.has_value());
}

static void test_empty_header_name()
{
  CsrfToken token("abc123");

  token.set_header_name("");

  assert(token.header_name().empty());
  assert(!token.has_header_name());
}

static void test_attrs()
{
  CsrfToken token("abc123");

  token.set_attr("class", "csrf-input");
  token.set_bool_attr("hidden", true);

  assert(token.attrs().has("class"));
  assert(token.attrs().has("hidden"));
  assert(token.attrs().size() == 2);
}

static void test_bool_attr_can_be_removed()
{
  CsrfToken token("abc123");

  token.set_bool_attr("hidden", true);
  assert(token.attrs().has("hidden"));

  token.set_bool_attr("hidden", false);
  assert(!token.attrs().has("hidden"));
}

static void test_render_default_hidden_input()
{
  CsrfToken token("abc123");

  const std::string rendered = token.render();

  assert(
      rendered ==
      R"(<input id="_token" name="_token" type="hidden" value="abc123">)");
}

static void test_render_named_hidden_input()
{
  CsrfToken token = CsrfToken::named("csrf", "abc123");

  const std::string rendered = token.render();

  assert(
      rendered ==
      R"(<input id="csrf" name="csrf" type="hidden" value="abc123">)");
}

static void test_render_without_name()
{
  CsrfToken token("abc123");

  token.set_name("");

  const std::string rendered = token.render();

  assert(rendered == R"(<input type="hidden" value="abc123">)");
}

static void test_render_empty_value()
{
  CsrfToken token;

  const std::string rendered = token.render();

  assert(
      rendered ==
      R"(<input id="_token" name="_token" type="hidden" value="">)");
}

static void test_render_with_custom_attrs()
{
  CsrfToken token("abc123");

  token.set_attr("class", "csrf-input");
  token.set_bool_attr("hidden", true);

  const std::string rendered = token.render();

  assert(
      rendered ==
      R"(<input class="csrf-input" hidden id="_token" name="_token" type="hidden" value="abc123">)");
}

static void test_render_escapes_name_and_value()
{
  CsrfToken token(R"(csrf "main")", R"(<secure & token>)");

  const std::string rendered = token.render();

  assert(
      rendered ==
      R"(<input id="csrf &quot;main&quot;" name="csrf &quot;main&quot;" type="hidden" value="&lt;secure &amp; token&gt;">)");
}

static void test_render_escapes_custom_attrs()
{
  CsrfToken token("abc123");

  token.set_attr("data-note", R"(token "main" & secure)");

  const std::string rendered = token.render();

  assert(
      rendered ==
      R"(<input data-note="token &quot;main&quot; &amp; secure" id="_token" name="_token" type="hidden" value="abc123">)");
}

static void test_render_meta_default()
{
  CsrfToken token("abc123");

  const std::string rendered = token.render_meta();

  assert(
      rendered ==
      R"(<meta content="abc123" data-header="X-CSRF-Token" name="csrf-token">)");
}

static void test_render_meta_custom_name()
{
  CsrfToken token("abc123");

  const std::string rendered = token.render_meta("vix-csrf");

  assert(
      rendered ==
      R"(<meta content="abc123" data-header="X-CSRF-Token" name="vix-csrf">)");
}

static void test_render_meta_without_header()
{
  CsrfToken token("abc123");

  token.set_header_name("");

  const std::string rendered = token.render_meta();

  assert(rendered == R"(<meta content="abc123" name="csrf-token">)");
}

static void test_render_meta_empty_value()
{
  CsrfToken token;

  const std::string rendered = token.render_meta();

  assert(
      rendered ==
      R"(<meta content="" data-header="X-CSRF-Token" name="csrf-token">)");
}

static void test_render_meta_escapes_values()
{
  CsrfToken token(R"(<secure & "token">)");

  token.set_header_name(R"(X-CSRF "Main")");

  const std::string rendered = token.render_meta(R"(csrf "meta")");

  assert(
      rendered ==
      R"(<meta content="&lt;secure &amp; &quot;token&quot;&gt;" data-header="X-CSRF &quot;Main&quot;" name="csrf &quot;meta&quot;">)");
}

int main()
{
  test_default_csrf_token();
  test_construct_with_value();
  test_construct_with_name_and_value();

  test_make_token();
  test_make_empty_token();
  test_named_token();

  test_setters();
  test_empty_name();
  test_empty_header_name();

  test_attrs();
  test_bool_attr_can_be_removed();

  test_render_default_hidden_input();
  test_render_named_hidden_input();
  test_render_without_name();
  test_render_empty_value();
  test_render_with_custom_attrs();
  test_render_escapes_name_and_value();
  test_render_escapes_custom_attrs();

  test_render_meta_default();
  test_render_meta_custom_name();
  test_render_meta_without_header();
  test_render_meta_empty_value();
  test_render_meta_escapes_values();

  std::cout << "[OK] ui csrf token tests passed\n";
  return 0;
}
