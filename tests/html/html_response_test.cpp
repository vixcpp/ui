/**
 *
 *  @file html_response_test.cpp
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

#include <vix/ui/core/ViewResult.hpp>
#include <vix/ui/html/HtmlResponse.hpp>
#include <vix/ui/support/Error.hpp>

using namespace vix::ui;

static void test_default_response()
{
  HtmlResponse response;

  assert(response.body().empty());
  assert(response.status_code() == 200);
  assert(response.content_type() == "text/html");
  assert(response.charset() == "utf-8");
  assert(response.header_content_type() == "text/html; charset=utf-8");
  assert(!response.from_cache());
  assert(!response.escaped());
  assert(response.empty());
  assert(response.size() == 0);
  assert(response.ok());
}

static void test_construct_response()
{
  HtmlResponse response("<h1>Hello</h1>", 201);

  assert(response.body() == "<h1>Hello</h1>");
  assert(response.status_code() == 201);
  assert(response.size() == 14);
  assert(response.ok());
}

static void test_static_html_factory()
{
  HtmlResponse response = HtmlResponse::html("<p>OK</p>", 202);

  assert(response.body() == "<p>OK</p>");
  assert(response.status_code() == 202);
  assert(response.ok());
}

static void test_from_view_result()
{
  ViewResult result;
  result.template_name = "home.html";
  result.output = "<h1>Home</h1>";
  result.from_cache = true;
  result.escaped = true;
  result.success = true;

  HtmlResponse response = HtmlResponse::from_view_result(result);

  assert(response.body() == "<h1>Home</h1>");
  assert(response.status_code() == 200);
  assert(response.from_cache());
  assert(response.escaped());
  assert(response.ok());
}

static void test_set_body()
{
  HtmlResponse response;

  response.set_body("<h1>Dashboard</h1>");

  assert(response.body() == "<h1>Dashboard</h1>");
  assert(!response.empty());
}

static void test_append()
{
  HtmlResponse response;

  response.append("<h1>");
  response.append("Hello");
  response.append("</h1>");

  assert(response.body() == "<h1>Hello</h1>");
}

static void test_set_status()
{
  HtmlResponse response;

  response.set_status(404);

  assert(response.status_code() == 404);
  assert(!response.ok());
}

static void test_invalid_status_throws()
{
  HtmlResponse response;

  bool thrown = false;

  try
  {
    response.set_status(99);
  }
  catch (const HtmlError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_set_content_type()
{
  HtmlResponse response;

  response.set_content_type("application/xhtml+xml");

  assert(response.content_type() == "application/xhtml+xml");
  assert(response.header_content_type() == "application/xhtml+xml; charset=utf-8");
}

static void test_set_charset()
{
  HtmlResponse response;

  response.set_charset("utf-16");

  assert(response.charset() == "utf-16");
  assert(response.header_content_type() == "text/html; charset=utf-16");
}

static void test_empty_charset()
{
  HtmlResponse response;

  response.set_charset("");

  assert(response.charset().empty());
  assert(response.header_content_type() == "text/html");
}

static void test_set_metadata_flags()
{
  HtmlResponse response;

  response.set_from_cache(true);
  response.set_escaped(true);

  assert(response.from_cache());
  assert(response.escaped());
}

static void test_empty_and_size()
{
  HtmlResponse response;

  assert(response.empty());
  assert(response.size() == 0);

  response.set_body("abc");

  assert(!response.empty());
  assert(response.size() == 3);
}

static void test_ok_status_range()
{
  HtmlResponse response;

  response.set_status(200);
  assert(response.ok());

  response.set_status(204);
  assert(response.ok());

  response.set_status(299);
  assert(response.ok());

  response.set_status(300);
  assert(!response.ok());
}

static void test_is_valid_status()
{
  assert(HtmlResponse::is_valid_status(100));
  assert(HtmlResponse::is_valid_status(200));
  assert(HtmlResponse::is_valid_status(404));
  assert(HtmlResponse::is_valid_status(599));

  assert(!HtmlResponse::is_valid_status(99));
  assert(!HtmlResponse::is_valid_status(600));
}

int main()
{
  test_default_response();
  test_construct_response();
  test_static_html_factory();
  test_from_view_result();
  test_set_body();
  test_append();
  test_set_status();
  test_invalid_status_throws();
  test_set_content_type();
  test_set_charset();
  test_empty_charset();
  test_set_metadata_flags();
  test_empty_and_size();
  test_ok_status_range();
  test_is_valid_status();

  std::cout << "[OK] ui html response tests passed\n";
  return 0;
}
