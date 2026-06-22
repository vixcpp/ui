/**
 *
 *  @file Viewport.cpp
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
#include <vix/ui/pwa/Viewport.hpp>
#include <vix/ui/html/Html.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    void append_part(
        std::string &output,
        std::string_view name,
        std::string_view value)
    {
      if (value.empty())
      {
        return;
      }

      if (!output.empty())
      {
        output += ", ";
      }

      output.append(name.data(), name.size());
      output.push_back('=');
      output.append(value.data(), value.size());
    }
  } // namespace

  std::string_view to_string(ViewportFit fit) noexcept
  {
    switch (fit)
    {
    case ViewportFit::Auto:
      return "auto";

    case ViewportFit::Contain:
      return "contain";

    case ViewportFit::Cover:
      return "cover";
    }

    return "auto";
  }

  Viewport Viewport::responsive()
  {
    return Viewport{};
  }

  Viewport Viewport::mobile_app()
  {
    Viewport viewport;
    viewport.set_viewport_fit(ViewportFit::Cover);
    return viewport;
  }

  Viewport &Viewport::set_width(std::string width)
  {
    width_ = std::move(width);
    return *this;
  }

  Viewport &Viewport::set_initial_scale(std::string scale)
  {
    initial_scale_ = std::move(scale);
    return *this;
  }

  Viewport &Viewport::set_minimum_scale(std::string scale)
  {
    minimum_scale_ = std::move(scale);
    return *this;
  }

  Viewport &Viewport::set_maximum_scale(std::string scale)
  {
    maximum_scale_ = std::move(scale);
    return *this;
  }

  Viewport &Viewport::set_user_scalable(bool value)
  {
    user_scalable_ = value ? "yes" : "no";
    return *this;
  }

  Viewport &Viewport::set_viewport_fit(ViewportFit fit) noexcept
  {
    viewport_fit_ = fit;
    return *this;
  }

  Viewport &Viewport::set_attr(std::string name, std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  Viewport &Viewport::set_bool_attr(
      std::string name,
      bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  const std::string &Viewport::width() const noexcept
  {
    return width_;
  }

  const std::string &Viewport::initial_scale() const noexcept
  {
    return initial_scale_;
  }

  const std::string &Viewport::minimum_scale() const noexcept
  {
    return minimum_scale_;
  }

  const std::string &Viewport::maximum_scale() const noexcept
  {
    return maximum_scale_;
  }

  const std::string &Viewport::user_scalable() const noexcept
  {
    return user_scalable_;
  }

  ViewportFit Viewport::viewport_fit() const noexcept
  {
    return viewport_fit_;
  }

  const HtmlAttrs &Viewport::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &Viewport::attrs() noexcept
  {
    return attrs_;
  }

  bool Viewport::has_width() const noexcept
  {
    return !width_.empty();
  }

  bool Viewport::has_initial_scale() const noexcept
  {
    return !initial_scale_.empty();
  }

  bool Viewport::has_minimum_scale() const noexcept
  {
    return !minimum_scale_.empty();
  }

  bool Viewport::has_maximum_scale() const noexcept
  {
    return !maximum_scale_.empty();
  }

  bool Viewport::has_user_scalable() const noexcept
  {
    return !user_scalable_.empty();
  }

  std::string Viewport::content() const
  {
    std::string output;

    append_part(output, "width", width_);
    append_part(output, "initial-scale", initial_scale_);
    append_part(output, "minimum-scale", minimum_scale_);
    append_part(output, "maximum-scale", maximum_scale_);
    append_part(output, "user-scalable", user_scalable_);

    if (viewport_fit_ != ViewportFit::Auto)
    {
      append_part(output, "viewport-fit", to_string(viewport_fit_));
    }

    return output;
  }

  std::string Viewport::render() const
  {
    HtmlAttrs attrs = attrs_;

    attrs.set("name", "viewport");
    attrs.set("content", content());

    return Html::void_tag("meta", attrs);
  }

} // namespace vix::ui
