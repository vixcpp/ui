/**
 *
 *  @file ServerReadiness.hpp
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
#ifndef VIX_UI_SHELL_SERVER_READINESS_HPP
#define VIX_UI_SHELL_SERVER_READINESS_HPP

#include <chrono>
#include <cstdint>
#include <string>

#include <vix/ui/support/Result.hpp>

namespace vix::ui
{
  /**
   * @brief Parsed local server endpoint.
   *
   * ServerEndpoint stores the host and port extracted from a HTTP URL.
   * It is used by ServerReadiness to check whether a local server is
   * accepting TCP connections before the desktop shell opens a WebView.
   */
  struct ServerEndpoint
  {
    /**
     * @brief Endpoint host.
     */
    std::string host;

    /**
     * @brief Endpoint port.
     */
    std::uint16_t port{0};

    /**
     * @brief Check whether the endpoint contains a host and port.
     *
     * @return True if the endpoint is usable.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return !host.empty() && port != 0;
    }
  };

  /**
   * @brief Local server readiness helper.
   *
   * ServerReadiness waits until a local HTTP server is reachable before
   * the desktop shell opens the configured WebView URL.
   *
   * For v0.4.0 this helper intentionally checks TCP readiness only. It does
   * not require curl, an HTTP client, or any extra dependency.
   */
  class ServerReadiness
  {
  public:
    /**
     * @brief Parse a HTTP URL into a host and port.
     *
     * Supported forms:
     *
     * @code
     * http://127.0.0.1:8080
     * http://127.0.0.1:8080/health
     * http://localhost:3000
     * @endcode
     *
     * @param url URL to parse.
     * @return Parsed endpoint or failed result.
     */
    [[nodiscard]] static Result<ServerEndpoint> parse_url(
        const std::string &url);

    /**
     * @brief Check whether a TCP endpoint accepts connections.
     *
     * @param endpoint Server endpoint.
     * @return Successful result if the endpoint is reachable.
     */
    [[nodiscard]] static Result<void> check_once(
        const ServerEndpoint &endpoint);

    /**
     * @brief Wait until a local server is reachable.
     *
     * The function retries until the server accepts a TCP connection or the
     * timeout expires.
     *
     * @param url URL used to extract host and port.
     * @param timeout Maximum time to wait.
     * @return Successful result if the server becomes reachable.
     */
    [[nodiscard]] static Result<void> wait(
        const std::string &url,
        std::chrono::milliseconds timeout);
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_SERVER_READINESS_HPP
