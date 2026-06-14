/**
 *
 *  @file HtmlResponse.cpp
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
#include <vix/ui/html/HtmlResponse.hpp>
#include <vix/ui/support/Error.hpp>

#include <utility>

namespace vix::ui
{
  HtmlResponse::HtmlResponse(
      std::string body,
      int status_code)
      : body_(std::move(body))
  {
    set_status(status_code);
  }

  HtmlResponse HtmlResponse::html(
      std::string body,
      int status_code)
  {
    return HtmlResponse(std::move(body), status_code);
  }

  HtmlResponse HtmlResponse::from_view_result(
      const ViewResult &result,
      int status_code)
  {
    HtmlResponse response(result.output, status_code);
    response.set_from_cache(result.from_cache);
    response.set_escaped(result.escaped);
    return response;
  }

  HtmlResponse &HtmlResponse::set_body(std::string body)
  {
    body_ = std::move(body);
    return *this;
  }

  HtmlResponse &HtmlResponse::append(std::string_view chunk)
  {
    body_.append(chunk.data(), chunk.size());
    return *this;
  }

  HtmlResponse &HtmlResponse::set_status(int status_code)
  {
    if (!is_valid_status(status_code))
    {
      throw HtmlError("invalid HTTP status code for HTML response");
    }

    status_code_ = status_code;
    return *this;
  }

  HtmlResponse &HtmlResponse::set_content_type(std::string content_type)
  {
    content_type_ = std::move(content_type);
    return *this;
  }

  HtmlResponse &HtmlResponse::set_charset(std::string charset)
  {
    charset_ = std::move(charset);
    return *this;
  }

  HtmlResponse &HtmlResponse::set_from_cache(bool value) noexcept
  {
    from_cache_ = value;
    return *this;
  }

  HtmlResponse &HtmlResponse::set_escaped(bool value) noexcept
  {
    escaped_ = value;
    return *this;
  }

  const std::string &HtmlResponse::body() const noexcept
  {
    return body_;
  }

  int HtmlResponse::status_code() const noexcept
  {
    return status_code_;
  }

  const std::string &HtmlResponse::content_type() const noexcept
  {
    return content_type_;
  }

  const std::string &HtmlResponse::charset() const noexcept
  {
    return charset_;
  }

  std::string HtmlResponse::header_content_type() const
  {
    if (charset_.empty())
    {
      return content_type_;
    }

    return content_type_ + "; charset=" + charset_;
  }

  bool HtmlResponse::from_cache() const noexcept
  {
    return from_cache_;
  }

  bool HtmlResponse::escaped() const noexcept
  {
    return escaped_;
  }

  bool HtmlResponse::empty() const noexcept
  {
    return body_.empty();
  }

  std::size_t HtmlResponse::size() const noexcept
  {
    return body_.size();
  }

  bool HtmlResponse::ok() const noexcept
  {
    return status_code_ >= 200 && status_code_ < 300;
  }

  bool HtmlResponse::is_valid_status(int status_code) noexcept
  {
    return status_code >= 100 && status_code <= 599;
  }

} // namespace vix::ui
