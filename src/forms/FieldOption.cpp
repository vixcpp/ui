/**
 *
 *  @file FieldOption.cpp
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
#include <vix/ui/forms/FieldOption.hpp>
#include <vix/ui/html/Html.hpp>

#include <utility>

namespace vix::ui
{
  FieldOption::FieldOption(std::string value)
      : value_(std::move(value))
  {
  }

  FieldOption::FieldOption(
      std::string value,
      std::string label)
      : value_(std::move(value)),
        label_(std::move(label))
  {
  }

  FieldOption FieldOption::make(
      std::string value,
      std::string label)
  {
    return FieldOption(std::move(value), std::move(label));
  }

  FieldOption &FieldOption::set_value(std::string value)
  {
    value_ = std::move(value);
    return *this;
  }

  FieldOption &FieldOption::set_label(std::string label)
  {
    label_ = std::move(label);
    return *this;
  }

  FieldOption &FieldOption::set_selected(bool selected) noexcept
  {
    selected_ = selected;
    return *this;
  }

  FieldOption &FieldOption::set_disabled(bool disabled) noexcept
  {
    disabled_ = disabled;
    return *this;
  }

  FieldOption &FieldOption::set_attr(
      std::string name,
      std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  FieldOption &FieldOption::set_bool_attr(
      std::string name,
      bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  const std::string &FieldOption::value() const noexcept
  {
    return value_;
  }

  const std::string &FieldOption::label() const noexcept
  {
    return label_;
  }

  bool FieldOption::selected() const noexcept
  {
    return selected_;
  }

  bool FieldOption::disabled() const noexcept
  {
    return disabled_;
  }

  const HtmlAttrs &FieldOption::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &FieldOption::attrs() noexcept
  {
    return attrs_;
  }

  bool FieldOption::has_value() const noexcept
  {
    return !value_.empty();
  }

  bool FieldOption::has_label() const noexcept
  {
    return !label_.empty();
  }

  std::string FieldOption::display_label() const
  {
    if (!label_.empty())
    {
      return label_;
    }

    return value_;
  }

  std::string FieldOption::render() const
  {
    HtmlAttrs attrs = attrs_;

    attrs.set("value", value_);
    attrs.boolean("selected", selected_);
    attrs.boolean("disabled", disabled_);

    return Html::tag(
        "option",
        Html::text(display_label()),
        attrs);
  }

} // namespace vix::ui
