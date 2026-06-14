/**
 *
 *  @file AssetManager.cpp
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
#include <vix/ui/assets/AssetManager.hpp>
#include <vix/ui/support/Error.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    bool is_absolute_url(std::string_view value) noexcept
    {
      return value.starts_with("http://") ||
             value.starts_with("https://") ||
             value.starts_with("//") ||
             value.starts_with("data:");
    }

    std::string join_url(
        std::string_view base_url,
        std::string_view path)
    {
      if (base_url.empty() || path.empty() || is_absolute_url(path))
      {
        return std::string(path);
      }

      const bool base_has_slash = base_url.back() == '/';
      const bool path_has_slash = path.front() == '/';

      if (base_has_slash && path_has_slash)
      {
        std::string output(base_url);
        output.append(path.substr(1).data(), path.size() - 1);
        return output;
      }

      if (!base_has_slash && !path_has_slash)
      {
        std::string output(base_url);
        output.push_back('/');
        output.append(path.data(), path.size());
        return output;
      }

      std::string output(base_url);
      output.append(path.data(), path.size());
      return output;
    }

    void validate_asset_name(const std::string &name)
    {
      if (name.empty())
      {
        throw AssetError("cannot register UI asset with an empty name");
      }
    }

    std::vector<Asset> collect_assets(
        const std::map<std::string, Asset> &assets,
        AssetType type)
    {
      std::vector<Asset> output;

      for (const auto &[name, asset] : assets)
      {
        (void)name;

        if (asset.type() == type)
        {
          output.push_back(asset);
        }
      }

      return output;
    }
  } // namespace

  AssetManager::AssetManager(std::string base_url)
      : base_url_(std::move(base_url))
  {
  }

  AssetManager AssetManager::make(std::string base_url)
  {
    return AssetManager(std::move(base_url));
  }

  AssetManager &AssetManager::set_base_url(std::string base_url)
  {
    base_url_ = std::move(base_url);
    return *this;
  }

  const std::string &AssetManager::base_url() const noexcept
  {
    return base_url_;
  }

  std::string AssetManager::resolve(std::string_view path) const
  {
    return join_url(base_url_, path);
  }

  AssetManager &AssetManager::add(std::string name, Asset asset)
  {
    validate_asset_name(name);

    if (!asset.has_url())
    {
      throw AssetError("cannot register UI asset without a URL");
    }

    assets_[std::move(name)] = std::move(asset);
    return *this;
  }

  AssetManager &AssetManager::add_stylesheet(
      std::string name,
      std::string href,
      std::string media)
  {
    return add(
        std::move(name),
        Asset::stylesheet(resolve(href), std::move(media)));
  }

  AssetManager &AssetManager::add_script(
      std::string name,
      std::string src,
      AssetLoading loading)
  {
    return add(
        std::move(name),
        Asset::script(resolve(src), loading));
  }

  AssetManager &AssetManager::add_image(
      std::string name,
      std::string src,
      std::string alt)
  {
    return add(
        std::move(name),
        Asset::image(resolve(src), std::move(alt)));
  }

  AssetManager &AssetManager::add_font(
      std::string name,
      std::string href)
  {
    return add(
        std::move(name),
        Asset::font(resolve(href)));
  }

  bool AssetManager::has(const std::string &name) const
  {
    return assets_.find(name) != assets_.end();
  }

  const Asset *AssetManager::get(const std::string &name) const noexcept
  {
    const auto it = assets_.find(name);
    if (it == assets_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  Asset *AssetManager::get(const std::string &name) noexcept
  {
    const auto it = assets_.find(name);
    if (it == assets_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  bool AssetManager::remove(const std::string &name)
  {
    return assets_.erase(name) > 0;
  }

  void AssetManager::clear() noexcept
  {
    assets_.clear();
  }

  bool AssetManager::empty() const noexcept
  {
    return assets_.empty();
  }

  std::size_t AssetManager::size() const noexcept
  {
    return assets_.size();
  }

  const std::map<std::string, Asset> &AssetManager::assets() const noexcept
  {
    return assets_;
  }

  std::map<std::string, Asset> &AssetManager::assets() noexcept
  {
    return assets_;
  }

  AssetManifest AssetManager::manifest() const
  {
    AssetManifest manifest;

    for (const auto &[name, asset] : assets_)
    {
      (void)name;
      manifest.add(asset);
    }

    return manifest;
  }

  std::string AssetManager::render() const
  {
    return manifest().render();
  }

  std::string AssetManager::render_type(AssetType type) const
  {
    return AssetManifest(collect_assets(assets_, type)).render();
  }

  std::string AssetManager::render_stylesheets() const
  {
    return render_type(AssetType::Stylesheet);
  }

  std::string AssetManager::render_scripts() const
  {
    return render_type(AssetType::Script);
  }

  std::string AssetManager::render_fonts() const
  {
    return render_type(AssetType::Font);
  }

} // namespace vix::ui
