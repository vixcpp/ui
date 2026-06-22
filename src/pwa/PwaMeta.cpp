/**
 *
 *  @file PwaMeta.cpp
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
#include <vix/ui/pwa/PwaMeta.hpp>
#include <vix/ui/html/Html.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    void append_line(
        std::string &output,
        const std::string &line)
    {
      if (line.empty())
      {
        return;
      }

      if (!output.empty())
      {
        output.push_back('\n');
      }

      output += line;
    }

    std::string render_meta(
        std::string name,
        std::string content)
    {
      if (name.empty() || content.empty())
      {
        return {};
      }

      HtmlAttrs attrs;

      attrs.set("name", std::move(name));
      attrs.set("content", std::move(content));

      return Html::void_tag("meta", attrs);
    }
  } // namespace

  std::string_view to_string(
      PwaAppleStatusBarStyle style) noexcept
  {
    switch (style)
    {
    case PwaAppleStatusBarStyle::Default:
      return "default";

    case PwaAppleStatusBarStyle::Black:
      return "black";

    case PwaAppleStatusBarStyle::BlackTranslucent:
      return "black-translucent";
    }

    return "default";
  }

  PwaMeta PwaMeta::make()
  {
    return PwaMeta{};
  }

  PwaMeta PwaMeta::mobile_app(
      std::string app_name,
      std::string theme_color)
  {
    PwaMeta meta;

    meta.set_viewport(Viewport::mobile_app());
    meta.set_application_name(app_name);
    meta.set_apple_mobile_web_app_title(std::move(app_name));
    meta.set_theme_color(std::move(theme_color));
    meta.set_apple_mobile_web_app_capable(true);
    meta.set_apple_status_bar_style(
        PwaAppleStatusBarStyle::BlackTranslucent);

    return meta;
  }

  PwaMeta &PwaMeta::set_include_viewport(bool value) noexcept
  {
    include_viewport_ = value;
    return *this;
  }

  PwaMeta &PwaMeta::set_include_manifest(bool value) noexcept
  {
    include_manifest_ = value;
    return *this;
  }

  PwaMeta &PwaMeta::set_viewport(Viewport viewport)
  {
    viewport_ = std::move(viewport);
    return *this;
  }

  PwaMeta &PwaMeta::set_manifest_href(std::string href)
  {
    manifest_href_ = std::move(href);
    return *this;
  }

  PwaMeta &PwaMeta::set_theme_color(std::string color)
  {
    theme_color_ = std::move(color);
    return *this;
  }

  PwaMeta &PwaMeta::set_application_name(std::string name)
  {
    application_name_ = std::move(name);
    return *this;
  }

  PwaMeta &PwaMeta::set_apple_mobile_web_app_title(
      std::string title)
  {
    apple_mobile_web_app_title_ = std::move(title);
    return *this;
  }

  PwaMeta &PwaMeta::set_apple_mobile_web_app_capable(
      bool value) noexcept
  {
    apple_mobile_web_app_capable_set_ = true;
    apple_mobile_web_app_capable_ = value;
    return *this;
  }

  PwaMeta &PwaMeta::set_apple_status_bar_style(
      PwaAppleStatusBarStyle style) noexcept
  {
    apple_status_bar_style_set_ = true;
    apple_status_bar_style_ = style;
    return *this;
  }

  PwaMeta &PwaMeta::set_color_scheme(std::string scheme)
  {
    color_scheme_ = std::move(scheme);
    return *this;
  }

  PwaMeta &PwaMeta::set_format_detection(std::string value)
  {
    format_detection_ = std::move(value);
    return *this;
  }

  PwaMeta &PwaMeta::set_manifest_attr(
      std::string name,
      std::string value)
  {
    manifest_attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  PwaMeta &PwaMeta::set_manifest_bool_attr(
      std::string name,
      bool enabled)
  {
    manifest_attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  bool PwaMeta::include_viewport() const noexcept
  {
    return include_viewport_;
  }

  bool PwaMeta::include_manifest() const noexcept
  {
    return include_manifest_;
  }

  const Viewport &PwaMeta::viewport() const noexcept
  {
    return viewport_;
  }

  Viewport &PwaMeta::viewport() noexcept
  {
    return viewport_;
  }

  const std::string &PwaMeta::manifest_href() const noexcept
  {
    return manifest_href_;
  }

  const std::string &PwaMeta::theme_color() const noexcept
  {
    return theme_color_;
  }

  const std::string &PwaMeta::application_name() const noexcept
  {
    return application_name_;
  }

  const std::string &
  PwaMeta::apple_mobile_web_app_title() const noexcept
  {
    return apple_mobile_web_app_title_;
  }

  bool PwaMeta::has_apple_mobile_web_app_capable() const noexcept
  {
    return apple_mobile_web_app_capable_set_;
  }

  bool PwaMeta::apple_mobile_web_app_capable() const noexcept
  {
    return apple_mobile_web_app_capable_;
  }

  bool PwaMeta::has_apple_status_bar_style() const noexcept
  {
    return apple_status_bar_style_set_;
  }

  PwaAppleStatusBarStyle
  PwaMeta::apple_status_bar_style() const noexcept
  {
    return apple_status_bar_style_;
  }

  const std::string &PwaMeta::color_scheme() const noexcept
  {
    return color_scheme_;
  }

  const std::string &PwaMeta::format_detection() const noexcept
  {
    return format_detection_;
  }

  const HtmlAttrs &PwaMeta::manifest_attrs() const noexcept
  {
    return manifest_attrs_;
  }

  HtmlAttrs &PwaMeta::manifest_attrs() noexcept
  {
    return manifest_attrs_;
  }

  bool PwaMeta::has_manifest_href() const noexcept
  {
    return !manifest_href_.empty();
  }

  bool PwaMeta::has_theme_color() const noexcept
  {
    return !theme_color_.empty();
  }

  bool PwaMeta::has_application_name() const noexcept
  {
    return !application_name_.empty();
  }

  bool PwaMeta::has_apple_mobile_web_app_title() const noexcept
  {
    return !apple_mobile_web_app_title_.empty();
  }

  bool PwaMeta::has_color_scheme() const noexcept
  {
    return !color_scheme_.empty();
  }

  bool PwaMeta::has_format_detection() const noexcept
  {
    return !format_detection_.empty();
  }

  std::string PwaMeta::render_viewport() const
  {
    if (!include_viewport_)
    {
      return {};
    }

    return viewport_.render();
  }

  std::string PwaMeta::render_manifest_link() const
  {
    if (!include_manifest_ || manifest_href_.empty())
    {
      return {};
    }

    HtmlAttrs attrs = manifest_attrs_;

    attrs.set("rel", "manifest");
    attrs.set("href", manifest_href_);

    return Html::void_tag("link", attrs);
  }

  std::string PwaMeta::render_theme_color() const
  {
    return render_meta("theme-color", theme_color_);
  }

  std::string PwaMeta::render_application_name() const
  {
    return render_meta("application-name", application_name_);
  }

  std::string PwaMeta::render_apple_mobile_web_app_capable() const
  {
    if (!apple_mobile_web_app_capable_set_)
    {
      return {};
    }

    return render_meta(
        "apple-mobile-web-app-capable",
        apple_mobile_web_app_capable_ ? "yes" : "no");
  }

  std::string PwaMeta::render_apple_mobile_web_app_title() const
  {
    return render_meta(
        "apple-mobile-web-app-title",
        apple_mobile_web_app_title_);
  }

  std::string PwaMeta::render_apple_status_bar_style() const
  {
    if (!apple_status_bar_style_set_)
    {
      return {};
    }

    return render_meta(
        "apple-mobile-web-app-status-bar-style",
        std::string(to_string(apple_status_bar_style_)));
  }

  std::string PwaMeta::render_color_scheme() const
  {
    return render_meta("color-scheme", color_scheme_);
  }

  std::string PwaMeta::render_format_detection() const
  {
    return render_meta("format-detection", format_detection_);
  }

  std::string PwaMeta::render() const
  {
    std::string output;

    append_line(output, render_viewport());
    append_line(output, render_manifest_link());
    append_line(output, render_theme_color());
    append_line(output, render_application_name());
    append_line(output, render_apple_mobile_web_app_capable());
    append_line(output, render_apple_mobile_web_app_title());
    append_line(output, render_apple_status_bar_style());
    append_line(output, render_color_scheme());
    append_line(output, render_format_detection());

    return output;
  }

} // namespace vix::ui
