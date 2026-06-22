/**
 *
 *  @file WebAppManifest.cpp
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
#include <vix/ui/pwa/WebAppManifest.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    void append_json_string(
        std::string &output,
        std::string_view value)
    {
      output.push_back('"');

      for (char ch : value)
      {
        switch (ch)
        {
        case '"':
          output += "\\\"";
          break;

        case '\\':
          output += "\\\\";
          break;

        case '\b':
          output += "\\b";
          break;

        case '\f':
          output += "\\f";
          break;

        case '\n':
          output += "\\n";
          break;

        case '\r':
          output += "\\r";
          break;

        case '\t':
          output += "\\t";
          break;

        default:
          output.push_back(ch);
          break;
        }
      }

      output.push_back('"');
    }

    void append_json_field(
        std::string &output,
        std::string_view name,
        std::string_view value,
        bool &first)
    {
      if (value.empty())
      {
        return;
      }

      if (!first)
      {
        output.push_back(',');
      }

      first = false;

      append_json_string(output, name);
      output.push_back(':');
      append_json_string(output, value);
    }
  } // namespace

  std::string_view to_string(
      WebAppDisplayMode mode) noexcept
  {
    switch (mode)
    {
    case WebAppDisplayMode::Browser:
      return "browser";

    case WebAppDisplayMode::MinimalUi:
      return "minimal-ui";

    case WebAppDisplayMode::Standalone:
      return "standalone";

    case WebAppDisplayMode::Fullscreen:
      return "fullscreen";
    }

    return "standalone";
  }

  std::string_view to_string(
      WebAppOrientation orientation) noexcept
  {
    switch (orientation)
    {
    case WebAppOrientation::Any:
      return "any";

    case WebAppOrientation::Natural:
      return "natural";

    case WebAppOrientation::Portrait:
      return "portrait";

    case WebAppOrientation::Landscape:
      return "landscape";

    case WebAppOrientation::PortraitPrimary:
      return "portrait-primary";

    case WebAppOrientation::PortraitSecondary:
      return "portrait-secondary";

    case WebAppOrientation::LandscapePrimary:
      return "landscape-primary";

    case WebAppOrientation::LandscapeSecondary:
      return "landscape-secondary";
    }

    return "any";
  }

  std::string_view to_string(
      WebAppDirection direction) noexcept
  {
    switch (direction)
    {
    case WebAppDirection::Auto:
      return "auto";

    case WebAppDirection::Ltr:
      return "ltr";

    case WebAppDirection::Rtl:
      return "rtl";
    }

    return "auto";
  }

  WebAppManifestIcon::WebAppManifestIcon(std::string src)
      : src_(std::move(src))
  {
  }

  WebAppManifestIcon::WebAppManifestIcon(
      std::string src,
      std::string sizes,
      std::string type)
      : src_(std::move(src)),
        sizes_(std::move(sizes)),
        type_(std::move(type))
  {
  }

  WebAppManifestIcon WebAppManifestIcon::make(
      std::string src,
      std::string sizes,
      std::string type)
  {
    return WebAppManifestIcon(
        std::move(src),
        std::move(sizes),
        std::move(type));
  }

  WebAppManifestIcon &WebAppManifestIcon::set_src(std::string src)
  {
    src_ = std::move(src);
    return *this;
  }

  WebAppManifestIcon &WebAppManifestIcon::set_sizes(std::string sizes)
  {
    sizes_ = std::move(sizes);
    return *this;
  }

  WebAppManifestIcon &WebAppManifestIcon::set_type(std::string type)
  {
    type_ = std::move(type);
    return *this;
  }

  WebAppManifestIcon &WebAppManifestIcon::set_purpose(
      std::string purpose)
  {
    purpose_ = std::move(purpose);
    return *this;
  }

  const std::string &WebAppManifestIcon::src() const noexcept
  {
    return src_;
  }

  const std::string &WebAppManifestIcon::sizes() const noexcept
  {
    return sizes_;
  }

  const std::string &WebAppManifestIcon::type() const noexcept
  {
    return type_;
  }

  const std::string &WebAppManifestIcon::purpose() const noexcept
  {
    return purpose_;
  }

  bool WebAppManifestIcon::has_src() const noexcept
  {
    return !src_.empty();
  }

  bool WebAppManifestIcon::has_sizes() const noexcept
  {
    return !sizes_.empty();
  }

  bool WebAppManifestIcon::has_type() const noexcept
  {
    return !type_.empty();
  }

  bool WebAppManifestIcon::has_purpose() const noexcept
  {
    return !purpose_.empty();
  }

  bool WebAppManifestIcon::empty() const noexcept
  {
    return src_.empty();
  }

  std::string WebAppManifestIcon::to_json() const
  {
    std::string output;
    bool first = true;

    output.push_back('{');

    append_json_field(output, "src", src_, first);
    append_json_field(output, "sizes", sizes_, first);
    append_json_field(output, "type", type_, first);
    append_json_field(output, "purpose", purpose_, first);

    output.push_back('}');

    return output;
  }

  WebAppManifest WebAppManifest::make()
  {
    return WebAppManifest{};
  }

  WebAppManifest WebAppManifest::app(
      std::string name,
      std::string short_name)
  {
    WebAppManifest manifest;

    manifest.set_name(std::move(name));
    manifest.set_short_name(std::move(short_name));
    manifest.set_display(WebAppDisplayMode::Standalone);

    return manifest;
  }

  WebAppManifest &WebAppManifest::set_name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_short_name(std::string short_name)
  {
    short_name_ = std::move(short_name);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_description(
      std::string description)
  {
    description_ = std::move(description);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_start_url(std::string start_url)
  {
    start_url_ = std::move(start_url);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_scope(std::string scope)
  {
    scope_ = std::move(scope);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_id(std::string id)
  {
    id_ = std::move(id);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_lang(std::string lang)
  {
    lang_ = std::move(lang);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_direction(
      WebAppDirection direction) noexcept
  {
    direction_ = direction;
    return *this;
  }

  WebAppManifest &WebAppManifest::set_display(
      WebAppDisplayMode display) noexcept
  {
    display_ = display;
    return *this;
  }

  WebAppManifest &WebAppManifest::set_orientation(
      WebAppOrientation orientation) noexcept
  {
    orientation_ = orientation;
    return *this;
  }

  WebAppManifest &WebAppManifest::set_background_color(
      std::string color)
  {
    background_color_ = std::move(color);
    return *this;
  }

  WebAppManifest &WebAppManifest::set_theme_color(std::string color)
  {
    theme_color_ = std::move(color);
    return *this;
  }

  WebAppManifest &WebAppManifest::add_icon(
      WebAppManifestIcon icon)
  {
    if (!icon.empty())
    {
      icons_.push_back(std::move(icon));
    }

    return *this;
  }

  WebAppManifest &WebAppManifest::add_icon(
      std::string src,
      std::string sizes,
      std::string type,
      std::string purpose)
  {
    WebAppManifestIcon icon(
        std::move(src),
        std::move(sizes),
        std::move(type));

    icon.set_purpose(std::move(purpose));

    return add_icon(std::move(icon));
  }

  void WebAppManifest::clear_icons() noexcept
  {
    icons_.clear();
  }

  const std::string &WebAppManifest::name() const noexcept
  {
    return name_;
  }

  const std::string &WebAppManifest::short_name() const noexcept
  {
    return short_name_;
  }

  const std::string &WebAppManifest::description() const noexcept
  {
    return description_;
  }

  const std::string &WebAppManifest::start_url() const noexcept
  {
    return start_url_;
  }

  const std::string &WebAppManifest::scope() const noexcept
  {
    return scope_;
  }

  const std::string &WebAppManifest::id() const noexcept
  {
    return id_;
  }

  const std::string &WebAppManifest::lang() const noexcept
  {
    return lang_;
  }

  WebAppDirection WebAppManifest::direction() const noexcept
  {
    return direction_;
  }

  WebAppDisplayMode WebAppManifest::display() const noexcept
  {
    return display_;
  }

  WebAppOrientation WebAppManifest::orientation() const noexcept
  {
    return orientation_;
  }

  const std::string &WebAppManifest::background_color() const noexcept
  {
    return background_color_;
  }

  const std::string &WebAppManifest::theme_color() const noexcept
  {
    return theme_color_;
  }

  const std::vector<WebAppManifestIcon> &
  WebAppManifest::icons() const noexcept
  {
    return icons_;
  }

  std::vector<WebAppManifestIcon> &
  WebAppManifest::icons() noexcept
  {
    return icons_;
  }

  bool WebAppManifest::has_name() const noexcept
  {
    return !name_.empty();
  }

  bool WebAppManifest::has_short_name() const noexcept
  {
    return !short_name_.empty();
  }

  bool WebAppManifest::has_description() const noexcept
  {
    return !description_.empty();
  }

  bool WebAppManifest::has_start_url() const noexcept
  {
    return !start_url_.empty();
  }

  bool WebAppManifest::has_scope() const noexcept
  {
    return !scope_.empty();
  }

  bool WebAppManifest::has_id() const noexcept
  {
    return !id_.empty();
  }

  bool WebAppManifest::has_lang() const noexcept
  {
    return !lang_.empty();
  }

  bool WebAppManifest::has_background_color() const noexcept
  {
    return !background_color_.empty();
  }

  bool WebAppManifest::has_theme_color() const noexcept
  {
    return !theme_color_.empty();
  }

  bool WebAppManifest::has_icons() const noexcept
  {
    return !icons_.empty();
  }

  std::size_t WebAppManifest::icon_count() const noexcept
  {
    return icons_.size();
  }

  std::string WebAppManifest::to_json() const
  {
    std::string output;
    bool first = true;

    output.push_back('{');

    append_json_field(output, "name", name_, first);
    append_json_field(output, "short_name", short_name_, first);
    append_json_field(output, "description", description_, first);
    append_json_field(output, "start_url", start_url_, first);
    append_json_field(output, "scope", scope_, first);
    append_json_field(output, "id", id_, first);
    append_json_field(output, "lang", lang_, first);

    if (direction_ != WebAppDirection::Auto)
    {
      append_json_field(output, "dir", to_string(direction_), first);
    }

    append_json_field(output, "display", to_string(display_), first);

    if (orientation_ != WebAppOrientation::Any)
    {
      append_json_field(output, "orientation", to_string(orientation_), first);
    }

    append_json_field(output, "background_color", background_color_, first);
    append_json_field(output, "theme_color", theme_color_, first);

    if (!icons_.empty())
    {
      if (!first)
      {
        output.push_back(',');
      }

      first = false;

      append_json_string(output, "icons");
      output.push_back(':');
      output.push_back('[');

      for (std::size_t i = 0; i < icons_.size(); ++i)
      {
        if (i > 0)
        {
          output.push_back(',');
        }

        output += icons_[i].to_json();
      }

      output.push_back(']');
    }

    output.push_back('}');

    return output;
  }

  std::string WebAppManifest::render() const
  {
    return to_json();
  }

} // namespace vix::ui
