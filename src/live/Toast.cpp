/**
 *
 *  @file Toast.cpp
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
#include <vix/ui/live/Toast.hpp>
#include <vix/ui/html/Html.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] std::string toast_class(
        FlashLevel level,
        ToastPosition position)
    {
      return "toast toast-" + std::string(to_string(level)) +
             " toast-" + std::string(to_string(position));
    }
  } // namespace

  std::string_view to_string(ToastPosition position) noexcept
  {
    switch (position)
    {
    case ToastPosition::TopRight:
      return "top-right";

    case ToastPosition::TopLeft:
      return "top-left";

    case ToastPosition::BottomRight:
      return "bottom-right";

    case ToastPosition::BottomLeft:
      return "bottom-left";

    case ToastPosition::TopCenter:
      return "top-center";

    case ToastPosition::BottomCenter:
      return "bottom-center";
    }

    return "top-right";
  }

  Toast::Toast(FlashLevel level, std::string message)
      : level_(level),
        message_(std::move(message))
  {
  }

  Toast Toast::info(std::string message)
  {
    return Toast(FlashLevel::Info, std::move(message));
  }

  Toast Toast::success(std::string message)
  {
    return Toast(FlashLevel::Success, std::move(message));
  }

  Toast Toast::warning(std::string message)
  {
    return Toast(FlashLevel::Warning, std::move(message));
  }

  Toast Toast::error(std::string message)
  {
    return Toast(FlashLevel::Error, std::move(message));
  }

  Toast Toast::neutral(std::string message)
  {
    return Toast(FlashLevel::Neutral, std::move(message));
  }

  Toast &Toast::set_level(FlashLevel level) noexcept
  {
    level_ = level;
    return *this;
  }

  Toast &Toast::set_message(std::string message)
  {
    message_ = std::move(message);
    return *this;
  }

  Toast &Toast::set_title(std::string title)
  {
    title_ = std::move(title);
    return *this;
  }

  Toast &Toast::set_position(ToastPosition position) noexcept
  {
    position_ = position;
    return *this;
  }

  Toast &Toast::set_dismissible(bool value) noexcept
  {
    dismissible_ = value;
    return *this;
  }

  Toast &Toast::set_auto_hide(bool value) noexcept
  {
    auto_hide_ = value;
    return *this;
  }

  Toast &Toast::set_timeout(std::chrono::milliseconds timeout) noexcept
  {
    timeout_ = timeout;
    return *this;
  }

  Toast &Toast::set_attr(std::string name, std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  Toast &Toast::set_bool_attr(std::string name, bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  FlashLevel Toast::level() const noexcept
  {
    return level_;
  }

  const std::string &Toast::message() const noexcept
  {
    return message_;
  }

  const std::string &Toast::title() const noexcept
  {
    return title_;
  }

  ToastPosition Toast::position() const noexcept
  {
    return position_;
  }

  bool Toast::dismissible() const noexcept
  {
    return dismissible_;
  }

  bool Toast::auto_hide() const noexcept
  {
    return auto_hide_;
  }

  std::chrono::milliseconds Toast::timeout() const noexcept
  {
    return timeout_;
  }

  const HtmlAttrs &Toast::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &Toast::attrs() noexcept
  {
    return attrs_;
  }

  bool Toast::has_message() const noexcept
  {
    return !message_.empty();
  }

  bool Toast::has_title() const noexcept
  {
    return !title_.empty();
  }

  bool Toast::empty() const noexcept
  {
    return message_.empty();
  }

  std::string Toast::render() const
  {
    HtmlAttrs attrs = attrs_;

    if (!attrs.has("class"))
    {
      attrs.set("class", toast_class(level_, position_));
    }

    attrs.set("role", level_ == FlashLevel::Error ? "alert" : "status");
    attrs.set("data-toast-level", std::string(to_string(level_)));
    attrs.set("data-toast-position", std::string(to_string(position_)));
    attrs.set("data-timeout", std::to_string(timeout_.count()));

    attrs.boolean("data-dismissible", dismissible_);
    attrs.boolean("data-auto-hide", auto_hide_);

    std::string content;

    if (has_title())
    {
      HtmlAttrs title_attrs;
      title_attrs.set("class", "toast-title");

      content += Html::tag(
          "strong",
          Html::text(title_),
          title_attrs);
    }

    if (has_message())
    {
      HtmlAttrs message_attrs;
      message_attrs.set("class", "toast-message");

      content += Html::tag(
          "span",
          Html::text(message_),
          message_attrs);
    }

    return Html::tag("div", content, attrs);
  }

} // namespace vix::ui
