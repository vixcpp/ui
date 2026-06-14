/**
 *
 *  @file HtmlResponse.hpp
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
#ifndef VIX_UI_HTML_HTML_RESPONSE_HPP
#define VIX_UI_HTML_HTML_RESPONSE_HPP

#include <cstddef>
#include <string>
#include <string_view>

#include <vix/ui/core/ViewResult.hpp>

namespace vix::ui
{
  /**
   * @brief HTML response data produced by the UI module.
   *
   * HtmlResponse is a small transport-neutral container for rendered HTML.
   * It stores:
   * - the response body
   * - the HTTP status code
   * - the content type
   * - the charset
   * - rendering metadata from the view/template layer
   *
   * This class intentionally does not depend on a concrete HTTP response
   * implementation. Higher-level integrations can copy its data into
   * `vix::Response`, another HTTP response object, or tests.
   */
  class HtmlResponse
  {
  public:
    /**
     * @brief Construct an empty HTML response.
     */
    HtmlResponse() = default;

    /**
     * @brief Construct an HTML response from a body and status code.
     *
     * @param body HTML response body.
     * @param status_code HTTP status code.
     *
     * @throws HtmlError if the status code is invalid.
     */
    explicit HtmlResponse(
        std::string body,
        int status_code = 200);

    /**
     * @brief Create an HTML response from a body and status code.
     *
     * @param body HTML response body.
     * @param status_code HTTP status code.
     * @return HTML response.
     *
     * @throws HtmlError if the status code is invalid.
     */
    [[nodiscard]] static HtmlResponse html(
        std::string body,
        int status_code = 200);

    /**
     * @brief Create an HTML response from a rendered view result.
     *
     * @param result Rendered view result.
     * @param status_code HTTP status code.
     * @return HTML response.
     *
     * @throws HtmlError if the status code is invalid.
     */
    [[nodiscard]] static HtmlResponse from_view_result(
        const ViewResult &result,
        int status_code = 200);

    /**
     * @brief Replace the response body.
     *
     * @param body HTML response body.
     * @return This response.
     */
    HtmlResponse &set_body(std::string body);

    /**
     * @brief Append raw HTML to the response body.
     *
     * The appended content is not escaped.
     *
     * @param chunk HTML chunk to append.
     * @return This response.
     */
    HtmlResponse &append(std::string_view chunk);

    /**
     * @brief Set the HTTP status code.
     *
     * @param status_code HTTP status code.
     * @return This response.
     *
     * @throws HtmlError if the status code is invalid.
     */
    HtmlResponse &set_status(int status_code);

    /**
     * @brief Set the content type.
     *
     * @param content_type MIME content type.
     * @return This response.
     */
    HtmlResponse &set_content_type(std::string content_type);

    /**
     * @brief Set the response charset.
     *
     * @param charset Response charset.
     * @return This response.
     */
    HtmlResponse &set_charset(std::string charset);

    /**
     * @brief Set whether the response came from a cached template.
     *
     * @param value Cache flag.
     * @return This response.
     */
    HtmlResponse &set_from_cache(bool value) noexcept;

    /**
     * @brief Set whether HTML escaping was applied.
     *
     * @param value Escaping flag.
     * @return This response.
     */
    HtmlResponse &set_escaped(bool value) noexcept;

    /**
     * @brief Get the response body.
     *
     * @return HTML response body.
     */
    [[nodiscard]] const std::string &body() const noexcept;

    /**
     * @brief Get the HTTP status code.
     *
     * @return HTTP status code.
     */
    [[nodiscard]] int status_code() const noexcept;

    /**
     * @brief Get the content type.
     *
     * @return MIME content type.
     */
    [[nodiscard]] const std::string &content_type() const noexcept;

    /**
     * @brief Get the response charset.
     *
     * @return Charset.
     */
    [[nodiscard]] const std::string &charset() const noexcept;

    /**
     * @brief Build the full Content-Type header value.
     *
     * Example:
     * `text/html; charset=utf-8`
     *
     * @return Full Content-Type header value.
     */
    [[nodiscard]] std::string header_content_type() const;

    /**
     * @brief Check whether the response came from a cached template.
     *
     * @return True if from cache.
     */
    [[nodiscard]] bool from_cache() const noexcept;

    /**
     * @brief Check whether HTML escaping was applied.
     *
     * @return True if escaped.
     */
    [[nodiscard]] bool escaped() const noexcept;

    /**
     * @brief Check whether the body is empty.
     *
     * @return True if empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the response body size.
     *
     * @return Body size in bytes.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Check whether the status code is a success status.
     *
     * @return True for 2xx status codes.
     */
    [[nodiscard]] bool ok() const noexcept;

    /**
     * @brief Check whether an HTTP status code is valid.
     *
     * @param status_code HTTP status code.
     * @return True if the status code is in the valid HTTP range.
     */
    [[nodiscard]] static bool is_valid_status(int status_code) noexcept;

  private:
    /**
     * @brief HTML response body.
     */
    std::string body_;

    /**
     * @brief HTTP status code.
     */
    int status_code_{200};

    /**
     * @brief MIME content type.
     */
    std::string content_type_{"text/html"};

    /**
     * @brief Response charset.
     */
    std::string charset_{"utf-8"};

    /**
     * @brief Whether the source template was loaded from cache.
     */
    bool from_cache_{false};

    /**
     * @brief Whether HTML escaping was applied while rendering.
     */
    bool escaped_{false};
  };

} // namespace vix::ui

#endif // VIX_UI_HTML_HTML_RESPONSE_HPP
