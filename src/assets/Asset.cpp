/**
 *
 *  @file Asset.cpp
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
#include <vix/ui/assets/Asset.hpp>
#include <vix/ui/html/Html.hpp>
#include <vix/ui/support/Error.hpp>

#include <utility>

namespace vix::ui
{
  std::string_view to_string(AssetType type) noexcept
  {
    switch (type)
    {
    case AssetType::Unknown:
      return "unknown";

    case AssetType::Stylesheet:
      return "stylesheet";

    case AssetType::Script:
      return "script";

    case AssetType::Image:
      return "image";

    case AssetType::Font:
      return "font";

    case AssetType::Other:
      return "other";
    }

    return "unknown";
  }

  std::string_view to_string(AssetLoading loading) noexcept
  {
    switch (loading)
    {
    case AssetLoading::Default:
      return "default";

    case AssetLoading::Deferred:
      return "deferred";

    case AssetLoading::Async:
      return "async";
    }

    return "default";
  }

  Asset::Asset(AssetType type, std::string url)
      : type_(type),
        url_(std::move(url))
  {
  }

  Asset Asset::stylesheet(
      std::string href,
      std::string media)
  {
    Asset asset(AssetType::Stylesheet, std::move(href));
    asset.set_media(std::move(media));
    return asset;
  }

  Asset Asset::script(
      std::string src,
      AssetLoading loading)
  {
    Asset asset(AssetType::Script, std::move(src));
    asset.set_loading(loading);
    return asset;
  }

  Asset Asset::image(
      std::string src,
      std::string alt)
  {
    Asset asset(AssetType::Image, std::move(src));

    if (!alt.empty())
    {
      asset.set_attr("alt", std::move(alt));
    }

    return asset;
  }

  Asset Asset::font(std::string href)
  {
    Asset asset(AssetType::Font, std::move(href));
    asset.set_crossorigin("anonymous");
    return asset;
  }

  Asset &Asset::set_type(AssetType type) noexcept
  {
    type_ = type;
    return *this;
  }

  Asset &Asset::set_url(std::string url)
  {
    url_ = std::move(url);
    return *this;
  }

  Asset &Asset::set_integrity(std::string integrity)
  {
    integrity_ = std::move(integrity);
    return *this;
  }

  Asset &Asset::set_crossorigin(std::string crossorigin)
  {
    crossorigin_ = std::move(crossorigin);
    return *this;
  }

  Asset &Asset::set_media(std::string media)
  {
    media_ = std::move(media);
    return *this;
  }

  Asset &Asset::set_loading(AssetLoading loading) noexcept
  {
    loading_ = loading;
    return *this;
  }

  Asset &Asset::set_attr(std::string name, std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  Asset &Asset::set_bool_attr(std::string name, bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  AssetType Asset::type() const noexcept
  {
    return type_;
  }

  const std::string &Asset::url() const noexcept
  {
    return url_;
  }

  const std::string &Asset::integrity() const noexcept
  {
    return integrity_;
  }

  const std::string &Asset::crossorigin() const noexcept
  {
    return crossorigin_;
  }

  const std::string &Asset::media() const noexcept
  {
    return media_;
  }

  AssetLoading Asset::loading() const noexcept
  {
    return loading_;
  }

  const HtmlAttrs &Asset::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &Asset::attrs() noexcept
  {
    return attrs_;
  }

  bool Asset::has_url() const noexcept
  {
    return !url_.empty();
  }

  bool Asset::has_integrity() const noexcept
  {
    return !integrity_.empty();
  }

  bool Asset::has_crossorigin() const noexcept
  {
    return !crossorigin_.empty();
  }

  bool Asset::has_media() const noexcept
  {
    return !media_.empty();
  }

  std::string Asset::to_html() const
  {
    if (url_.empty())
    {
      throw AssetError("cannot render UI asset without a URL");
    }

    HtmlAttrs attrs = attrs_;

    switch (type_)
    {
    case AssetType::Stylesheet:
      attrs.set("rel", "stylesheet");
      attrs.set("href", url_);

      if (has_media())
      {
        attrs.set("media", media_);
      }

      if (has_integrity())
      {
        attrs.set("integrity", integrity_);
      }

      if (has_crossorigin())
      {
        attrs.set("crossorigin", crossorigin_);
      }

      return Html::void_tag("link", attrs);

    case AssetType::Script:
      attrs.set("src", url_);

      if (loading_ == AssetLoading::Deferred)
      {
        attrs.boolean("defer", true);
      }
      else if (loading_ == AssetLoading::Async)
      {
        attrs.boolean("async", true);
      }

      if (has_integrity())
      {
        attrs.set("integrity", integrity_);
      }

      if (has_crossorigin())
      {
        attrs.set("crossorigin", crossorigin_);
      }

      return Html::tag("script", "", attrs);

    case AssetType::Image:
      attrs.set("src", url_);
      return Html::void_tag("img", attrs);

    case AssetType::Font:
      attrs.set("rel", "preload");
      attrs.set("href", url_);
      attrs.set("as", "font");

      if (has_crossorigin())
      {
        attrs.set("crossorigin", crossorigin_);
      }

      return Html::void_tag("link", attrs);

    case AssetType::Other:
      attrs.set("href", url_);
      return Html::void_tag("link", attrs);

    case AssetType::Unknown:
      break;
    }

    throw AssetError("cannot render UI asset with unknown type");
  }

} // namespace vix::ui
