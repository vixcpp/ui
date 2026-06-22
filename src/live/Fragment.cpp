/**
 *
 *  @file Fragment.cpp
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
#include <vix/ui/live/Fragment.hpp>
#include <vix/ui/html/Html.hpp>

#include <utility>

namespace vix::ui
{
  Fragment::Fragment(std::string name)
      : name_(std::move(name))
  {
  }

  Fragment::Fragment(std::string name, std::string html)
      : name_(std::move(name)),
        html_(std::move(html))
  {
  }

  Fragment Fragment::make(std::string name)
  {
    return Fragment(std::move(name));
  }

  Fragment Fragment::html(std::string html)
  {
    Fragment fragment;
    fragment.set_html(std::move(html));
    return fragment;
  }

  Fragment &Fragment::set_name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  Fragment &Fragment::set_target(std::string target)
  {
    target_ = std::move(target);
    return *this;
  }

  Fragment &Fragment::set_html(std::string html)
  {
    html_ = std::move(html);
    return *this;
  }

  Fragment &Fragment::set_attr(std::string name, std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  Fragment &Fragment::set_bool_attr(std::string name, bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  const std::string &Fragment::name() const noexcept
  {
    return name_;
  }

  const std::string &Fragment::target() const noexcept
  {
    return target_;
  }

  const std::string &Fragment::html() const noexcept
  {
    return html_;
  }

  const HtmlAttrs &Fragment::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &Fragment::attrs() noexcept
  {
    return attrs_;
  }

  bool Fragment::has_name() const noexcept
  {
    return !name_.empty();
  }

  bool Fragment::has_target() const noexcept
  {
    return !target_.empty();
  }

  bool Fragment::has_html() const noexcept
  {
    return !html_.empty();
  }

  bool Fragment::empty() const noexcept
  {
    return html_.empty();
  }

  std::size_t Fragment::size() const noexcept
  {
    return html_.size();
  }

  std::string Fragment::render() const
  {
    return html_;
  }

  std::string Fragment::render_wrapped(std::string tag) const
  {
    HtmlAttrs attrs = attrs_;

    if (has_name())
    {
      attrs.set("data-fragment", name_);
    }

    if (has_target())
    {
      attrs.set("data-target", target_);
    }

    return Html::tag(tag, html_, attrs);
  }

} // namespace vix::ui
