/**
 *
 *  @file CsrfToken.cpp
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
#include <vix/ui/forms/CsrfToken.hpp>
#include <vix/ui/html/Html.hpp>

#include <utility>

namespace vix::ui
{
  CsrfToken::CsrfToken(std::string value)
      : value_(std::move(value))
  {
  }

  CsrfToken::CsrfToken(
      std::string name,
      std::string value)
      : name_(std::move(name)),
        value_(std::move(value))
  {
  }

  CsrfToken CsrfToken::make(std::string value)
  {
    return CsrfToken(std::move(value));
  }

  CsrfToken CsrfToken::named(
      std::string name,
      std::string value)
  {
    return CsrfToken(std::move(name), std::move(value));
  }

  CsrfToken &CsrfToken::set_name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  CsrfToken &CsrfToken::set_value(std::string value)
  {
    value_ = std::move(value);
    return *this;
  }

  CsrfToken &CsrfToken::set_header_name(std::string header_name)
  {
    header_name_ = std::move(header_name);
    return *this;
  }

  CsrfToken &CsrfToken::set_attr(
      std::string name,
      std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  CsrfToken &CsrfToken::set_bool_attr(
      std::string name,
      bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  const std::string &CsrfToken::name() const noexcept
  {
    return name_;
  }

  const std::string &CsrfToken::value() const noexcept
  {
    return value_;
  }

  const std::string &CsrfToken::header_name() const noexcept
  {
    return header_name_;
  }

  const HtmlAttrs &CsrfToken::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &CsrfToken::attrs() noexcept
  {
    return attrs_;
  }

  bool CsrfToken::has_name() const noexcept
  {
    return !name_.empty();
  }

  bool CsrfToken::has_value() const noexcept
  {
    return !value_.empty();
  }

  bool CsrfToken::has_header_name() const noexcept
  {
    return !header_name_.empty();
  }

  bool CsrfToken::empty() const noexcept
  {
    return value_.empty();
  }

  std::string CsrfToken::render() const
  {
    HtmlAttrs attrs = attrs_;

    attrs.set("type", "hidden");

    if (has_name())
    {
      attrs.set("name", name_);
      attrs.set("id", name_);
    }

    attrs.set("value", value_);

    return Html::void_tag("input", attrs);
  }

  std::string CsrfToken::render_meta(std::string meta_name) const
  {
    HtmlAttrs attrs;

    attrs.set("name", std::move(meta_name));
    attrs.set("content", value_);

    if (has_header_name())
    {
      attrs.set("data-header", header_name_);
    }

    return Html::void_tag("meta", attrs);
  }

} // namespace vix::ui
