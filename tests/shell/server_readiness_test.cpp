/**
 *
 *  @file server_readiness_test.cpp
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
#include <chrono>
#include <iostream>
#include <string>

#include <vix/ui/shell/ServerReadiness.hpp>
#include <vix/ui/support/Result.hpp>

using namespace vix::ui;

static void test_parse_empty_url_fails()
{
  Result<ServerEndpoint> result = ServerReadiness::parse_url("");

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_parse_invalid_scheme_fails()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("ftp://127.0.0.1:8080");

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_parse_missing_host_fails()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("http://:8080");

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_parse_invalid_port_fails()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("http://127.0.0.1:abc");

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_parse_out_of_range_port_fails()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("http://127.0.0.1:999999");

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_parse_http_url_with_port()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("http://127.0.0.1:8080");

  assert(result.is_ok());

  const ServerEndpoint &endpoint = result.value();

  assert(endpoint.valid());
  assert(endpoint.host == "127.0.0.1");
  assert(endpoint.port == 8080);
}

static void test_parse_http_url_with_path()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("http://localhost:3000/health");

  assert(result.is_ok());

  const ServerEndpoint &endpoint = result.value();

  assert(endpoint.valid());
  assert(endpoint.host == "localhost");
  assert(endpoint.port == 3000);
}

static void test_parse_http_url_uses_default_port()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("http://localhost");

  assert(result.is_ok());

  const ServerEndpoint &endpoint = result.value();

  assert(endpoint.valid());
  assert(endpoint.host == "localhost");
  assert(endpoint.port == 80);
}

static void test_parse_https_url_uses_default_port()
{
  Result<ServerEndpoint> result =
      ServerReadiness::parse_url("https://localhost/health");

  assert(result.is_ok());

  const ServerEndpoint &endpoint = result.value();

  assert(endpoint.valid());
  assert(endpoint.host == "localhost");
  assert(endpoint.port == 443);
}

static void test_check_once_invalid_endpoint_fails()
{
  ServerEndpoint endpoint;

  Result<void> result = ServerReadiness::check_once(endpoint);

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_wait_empty_url_fails()
{
  Result<void> result =
      ServerReadiness::wait("", std::chrono::milliseconds(1));

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_wait_invalid_url_fails()
{
  Result<void> result =
      ServerReadiness::wait("bad-url", std::chrono::milliseconds(1));

  assert(result.is_failed());
  assert(result.error_code() == ErrorCode::ConfigError);
  assert(!result.error_message().empty());
}

static void test_wait_unreachable_port_fails()
{
  Result<void> result =
      ServerReadiness::wait(
          "http://127.0.0.1:1",
          std::chrono::milliseconds(1));

  assert(result.is_failed());
  assert(!result.error_message().empty());
}

int main()
{
  test_parse_empty_url_fails();
  test_parse_invalid_scheme_fails();
  test_parse_missing_host_fails();
  test_parse_invalid_port_fails();
  test_parse_out_of_range_port_fails();

  test_parse_http_url_with_port();
  test_parse_http_url_with_path();
  test_parse_http_url_uses_default_port();
  test_parse_https_url_uses_default_port();

  test_check_once_invalid_endpoint_fails();

  test_wait_empty_url_fails();
  test_wait_invalid_url_fails();
  test_wait_unreachable_port_fails();

  std::cout << "[OK] ui server readiness tests passed\n";
  return 0;
}
