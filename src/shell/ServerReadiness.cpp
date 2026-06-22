/**
 *
 *  @file ServerReadiness.cpp
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
#include <vix/ui/shell/ServerReadiness.hpp>

#include <chrono>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <limits>
#include <string>
#include <thread>

#if defined(__unix__) || defined(__APPLE__)
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] bool starts_with(
        const std::string &value,
        const std::string &prefix) noexcept
    {
      return value.size() >= prefix.size() &&
             value.compare(0, prefix.size(), prefix) == 0;
    }

    [[nodiscard]] bool is_digits(const std::string &value) noexcept
    {
      if (value.empty())
      {
        return false;
      }

      for (char ch : value)
      {
        const unsigned char c = static_cast<unsigned char>(ch);

        if (!std::isdigit(c))
        {
          return false;
        }
      }

      return true;
    }

    [[nodiscard]] Result<std::uint16_t> parse_port(
        const std::string &value)
    {
      if (!is_digits(value))
      {
        return Result<std::uint16_t>::fail(
            ErrorCode::ConfigError,
            "server readiness URL contains an invalid port");
      }

      unsigned long port = 0;

      try
      {
        port = std::stoul(value);
      }
      catch (...)
      {
        return Result<std::uint16_t>::fail(
            ErrorCode::ConfigError,
            "server readiness URL contains an invalid port");
      }

      if (port == 0 ||
          port > std::numeric_limits<std::uint16_t>::max())
      {
        return Result<std::uint16_t>::fail(
            ErrorCode::ConfigError,
            "server readiness URL port is out of range");
      }

      return Result<std::uint16_t>::ok(
          static_cast<std::uint16_t>(port));
    }

#if defined(__unix__) || defined(__APPLE__)
    [[nodiscard]] Result<void> connect_tcp(
        const ServerEndpoint &endpoint)
    {
      struct addrinfo hints{};

      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;

      struct addrinfo *addresses = nullptr;

      const std::string port = std::to_string(endpoint.port);

      const int lookup_status = getaddrinfo(
          endpoint.host.c_str(),
          port.c_str(),
          &hints,
          &addresses);

      if (lookup_status != 0)
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            std::string("failed to resolve server readiness host: ") +
                gai_strerror(lookup_status));
      }

      Result<void> result = Result<void>::fail(
          ErrorCode::RuntimeError,
          "server readiness endpoint is not reachable");

      for (struct addrinfo *entry = addresses;
           entry != nullptr;
           entry = entry->ai_next)
      {
        const int fd = socket(
            entry->ai_family,
            entry->ai_socktype,
            entry->ai_protocol);

        if (fd < 0)
        {
          continue;
        }

        const int connect_status = connect(
            fd,
            entry->ai_addr,
            entry->ai_addrlen);

        if (connect_status == 0)
        {
          close(fd);
          result = Result<void>::ok();
          break;
        }

        close(fd);
      }

      freeaddrinfo(addresses);
      return result;
    }
#endif
  } // namespace

  Result<ServerEndpoint> ServerReadiness::parse_url(
      const std::string &url)
  {
    if (url.empty())
    {
      return Result<ServerEndpoint>::fail(
          ErrorCode::ConfigError,
          "server readiness URL is empty");
    }

    std::string rest;
    std::uint16_t default_port = 0;

    if (starts_with(url, "http://"))
    {
      rest = url.substr(7);
      default_port = 80;
    }
    else if (starts_with(url, "https://"))
    {
      rest = url.substr(8);
      default_port = 443;
    }
    else
    {
      return Result<ServerEndpoint>::fail(
          ErrorCode::ConfigError,
          "server readiness URL must start with http:// or https://");
    }

    const std::size_t path_pos = rest.find('/');
    const std::string authority =
        path_pos == std::string::npos ? rest : rest.substr(0, path_pos);

    if (authority.empty())
    {
      return Result<ServerEndpoint>::fail(
          ErrorCode::ConfigError,
          "server readiness URL is missing a host");
    }

    std::string host;
    std::uint16_t port = default_port;

    const std::size_t colon_pos = authority.rfind(':');

    if (colon_pos == std::string::npos)
    {
      host = authority;
    }
    else
    {
      host = authority.substr(0, colon_pos);

      const std::string port_text = authority.substr(colon_pos + 1);
      Result<std::uint16_t> parsed_port = parse_port(port_text);

      if (parsed_port.is_failed())
      {
        return Result<ServerEndpoint>::fail(
            parsed_port.error_code(),
            parsed_port.error_message());
      }

      port = parsed_port.value();
    }

    if (host.empty())
    {
      return Result<ServerEndpoint>::fail(
          ErrorCode::ConfigError,
          "server readiness URL is missing a host");
    }

    ServerEndpoint endpoint;
    endpoint.host = std::move(host);
    endpoint.port = port;

    if (!endpoint.valid())
    {
      return Result<ServerEndpoint>::fail(
          ErrorCode::ConfigError,
          "server readiness URL did not produce a valid endpoint");
    }

    return Result<ServerEndpoint>::ok(std::move(endpoint));
  }

  Result<void> ServerReadiness::check_once(
      const ServerEndpoint &endpoint)
  {
    if (!endpoint.valid())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "server readiness endpoint is invalid");
    }

#if defined(__unix__) || defined(__APPLE__)
    return connect_tcp(endpoint);
#else
    return Result<void>::fail(
        ErrorCode::RuntimeError,
        "server readiness checks are not supported on this platform yet");
#endif
  }

  Result<void> ServerReadiness::wait(
      const std::string &url,
      std::chrono::milliseconds timeout)
  {
    Result<ServerEndpoint> endpoint_result = parse_url(url);

    if (endpoint_result.is_failed())
    {
      return Result<void>::fail(
          endpoint_result.error_code(),
          endpoint_result.error_message());
    }

    if (timeout.count() <= 0)
    {
      return check_once(endpoint_result.value());
    }

    const auto start = std::chrono::steady_clock::now();
    const auto deadline = start + timeout;

    Result<void> last_result = Result<void>::fail(
        ErrorCode::RuntimeError,
        "server readiness endpoint is not reachable");

    while (std::chrono::steady_clock::now() <= deadline)
    {
      last_result = check_once(endpoint_result.value());

      if (last_result.is_ok())
      {
        return Result<void>::ok();
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return Result<void>::fail(
        ErrorCode::RuntimeError,
        "server did not become ready before startup timeout");
  }

} // namespace vix::ui
