/**
 *
 *  @file FlashMessage.cpp
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
#include <vix/ui/live/FlashMessage.hpp>
#include <vix/ui/html/Html.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] std::string level_class(FlashLevel level)
    {
      return "flash flash-" + std::string(to_string(level));
    }
  } // namespace

  std::string_view to_string(FlashLevel level) noexcept
  {
    switch (level)
    {
    case FlashLevel::Info:
      return "info";

    case FlashLevel::Success:
      return "success";

    case FlashLevel::Warning:
      return "warning";

    case FlashLevel::Error:
      return "error";

    case FlashLevel::Neutral:
      return "neutral";
    }

    return "info";
  }

  FlashMessage::FlashMessage(
      FlashLevel level,
      std::string message)
      : level_(level),
        message_(std::move(message))
  {
  }

  FlashMessage FlashMessage::info(std::string message)
  {
    return FlashMessage(FlashLevel::Info, std::move(message));
  }

  FlashMessage FlashMessage::success(std::string message)
  {
    return FlashMessage(FlashLevel::Success, std::move(message));
  }

  FlashMessage FlashMessage::warning(std::string message)
  {
    return FlashMessage(FlashLevel::Warning, std::move(message));
  }

  FlashMessage FlashMessage::error(std::string message)
  {
    return FlashMessage(FlashLevel::Error, std::move(message));
  }

  FlashMessage FlashMessage::neutral(std::string message)
  {
    return FlashMessage(FlashLevel::Neutral, std::move(message));
  }

  FlashMessage &FlashMessage::set_level(FlashLevel level) noexcept
  {
    level_ = level;
    return *this;
  }

  FlashMessage &FlashMessage::set_message(std::string message)
  {
    message_ = std::move(message);
    return *this;
  }

  FlashMessage &FlashMessage::set_title(std::string title)
  {
    title_ = std::move(title);
    return *this;
  }

  FlashMessage &FlashMessage::set_dismissible(bool value) noexcept
  {
    dismissible_ = value;
    return *this;
  }

  FlashMessage &FlashMessage::set_attr(
      std::string name,
      std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  FlashMessage &FlashMessage::set_bool_attr(
      std::string name,
      bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  FlashLevel FlashMessage::level() const noexcept
  {
    return level_;
  }

  const std::string &FlashMessage::message() const noexcept
  {
    return message_;
  }

  const std::string &FlashMessage::title() const noexcept
  {
    return title_;
  }

  bool FlashMessage::dismissible() const noexcept
  {
    return dismissible_;
  }

  const HtmlAttrs &FlashMessage::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &FlashMessage::attrs() noexcept
  {
    return attrs_;
  }

  bool FlashMessage::has_message() const noexcept
  {
    return !message_.empty();
  }

  bool FlashMessage::has_title() const noexcept
  {
    return !title_.empty();
  }

  bool FlashMessage::empty() const noexcept
  {
    return message_.empty();
  }

  std::string FlashMessage::render() const
  {
    HtmlAttrs attrs = attrs_;

    if (!attrs.has("class"))
    {
      attrs.set("class", level_class(level_));
    }

    attrs.set("role", level_ == FlashLevel::Error ? "alert" : "status");
    attrs.set("data-flash-level", std::string(to_string(level_)));
    attrs.boolean("data-dismissible", dismissible_);

    std::string content;

    if (has_title())
    {
      HtmlAttrs title_attrs;
      title_attrs.set("class", "flash-title");

      content += Html::tag(
          "strong",
          Html::text(title_),
          title_attrs);
    }

    if (has_message())
    {
      HtmlAttrs message_attrs;
      message_attrs.set("class", "flash-message");

      content += Html::tag(
          "span",
          Html::text(message_),
          message_attrs);
    }

    return Html::tag("div", content, attrs);
  }

} // namespace vix::ui
