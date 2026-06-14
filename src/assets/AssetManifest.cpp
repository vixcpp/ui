/**
 *
 *  @file AssetManifest.cpp
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
#include <vix/ui/assets/AssetManifest.hpp>
#include <vix/ui/support/Error.hpp>

#include <algorithm>
#include <utility>

namespace vix::ui
{
  namespace
  {
    std::string render_assets(const std::vector<Asset> &assets)
    {
      std::string output;

      for (std::size_t i = 0; i < assets.size(); ++i)
      {
        if (i > 0)
        {
          output.push_back('\n');
        }

        output += assets[i].to_html();
      }

      return output;
    }
  } // namespace

  AssetManifest::AssetManifest(std::vector<Asset> assets)
      : assets_(std::move(assets))
  {
  }

  AssetManifest AssetManifest::make()
  {
    return AssetManifest{};
  }

  AssetManifest &AssetManifest::add(Asset asset)
  {
    if (!asset.has_url())
    {
      throw AssetError("cannot add UI asset without a URL");
    }

    assets_.push_back(std::move(asset));
    return *this;
  }

  AssetManifest &AssetManifest::add_stylesheet(
      std::string href,
      std::string media)
  {
    return add(Asset::stylesheet(std::move(href), std::move(media)));
  }

  AssetManifest &AssetManifest::add_script(
      std::string src,
      AssetLoading loading)
  {
    return add(Asset::script(std::move(src), loading));
  }

  AssetManifest &AssetManifest::add_image(
      std::string src,
      std::string alt)
  {
    return add(Asset::image(std::move(src), std::move(alt)));
  }

  AssetManifest &AssetManifest::add_font(std::string href)
  {
    return add(Asset::font(std::move(href)));
  }

  bool AssetManifest::contains(std::string_view url) const noexcept
  {
    return find(url) != nullptr;
  }

  const Asset *AssetManifest::find(std::string_view url) const noexcept
  {
    for (const auto &asset : assets_)
    {
      if (asset.url() == url)
      {
        return &asset;
      }
    }

    return nullptr;
  }

  Asset *AssetManifest::find(std::string_view url) noexcept
  {
    for (auto &asset : assets_)
    {
      if (asset.url() == url)
      {
        return &asset;
      }
    }

    return nullptr;
  }

  std::size_t AssetManifest::remove(std::string_view url)
  {
    const auto before = assets_.size();

    assets_.erase(
        std::remove_if(
            assets_.begin(),
            assets_.end(),
            [url](const Asset &asset)
            {
              return asset.url() == url;
            }),
        assets_.end());

    return before - assets_.size();
  }

  void AssetManifest::clear() noexcept
  {
    assets_.clear();
  }

  bool AssetManifest::empty() const noexcept
  {
    return assets_.empty();
  }

  std::size_t AssetManifest::size() const noexcept
  {
    return assets_.size();
  }

  const std::vector<Asset> &AssetManifest::assets() const noexcept
  {
    return assets_;
  }

  std::vector<Asset> &AssetManifest::assets() noexcept
  {
    return assets_;
  }

  std::string AssetManifest::render() const
  {
    return render_assets(assets_);
  }

  std::string AssetManifest::render_type(AssetType type) const
  {
    std::vector<Asset> filtered;

    for (const auto &asset : assets_)
    {
      if (asset.type() == type)
      {
        filtered.push_back(asset);
      }
    }

    return render_assets(filtered);
  }

  std::string AssetManifest::render_stylesheets() const
  {
    return render_type(AssetType::Stylesheet);
  }

  std::string AssetManifest::render_scripts() const
  {
    return render_type(AssetType::Script);
  }

  std::string AssetManifest::render_fonts() const
  {
    return render_type(AssetType::Font);
  }

} // namespace vix::ui
