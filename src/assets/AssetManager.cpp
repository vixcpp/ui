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

#include <algorithm>
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

    std::string make_key(std::string_view value)
    {
      return std::string(value);
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

    std::string append_version(
        std::string url,
        const std::string &version)
    {
      if (url.empty() || version.empty() || is_absolute_url(url))
      {
        return url;
      }

      if (url.find('?') == std::string::npos)
      {
        url += "?v=";
        url += version;
        return url;
      }

      url += "&v=";
      url += version;
      return url;
    }

    void validate_asset_name(const std::string &name)
    {
      if (name.empty())
      {
        throw AssetError("cannot register UI asset with an empty name");
      }
    }

    void validate_asset_group(const std::string &group)
    {
      if (group.empty())
      {
        throw AssetError("cannot register UI asset with an empty group name");
      }
    }

    void add_unique(
        std::vector<std::string> &items,
        const std::string &value)
    {
      if (std::find(items.begin(), items.end(), value) == items.end())
      {
        items.push_back(value);
      }
    }

    void remove_from_group(
        std::vector<std::string> &items,
        const std::string &value)
    {
      items.erase(
          std::remove(items.begin(), items.end(), value),
          items.end());
    }

    std::vector<Asset> collect_assets(
        const std::map<std::string, Asset> &assets,
        const std::vector<std::string> &order,
        AssetType type)
    {
      std::vector<Asset> output;

      for (const auto &name : order)
      {
        const auto it = assets.find(name);
        if (it == assets.end())
        {
          continue;
        }

        if (it->second.type() == type)
        {
          output.push_back(it->second);
        }
      }

      return output;
    }

    std::vector<Asset> collect_group_assets(
        const std::map<std::string, Asset> &assets,
        const std::vector<std::string> &names)
    {
      std::vector<Asset> output;

      for (const auto &name : names)
      {
        const auto it = assets.find(name);
        if (it == assets.end())
        {
          continue;
        }

        output.push_back(it->second);
      }

      return output;
    }

    std::vector<Asset> collect_group_assets(
        const std::map<std::string, Asset> &assets,
        const std::vector<std::string> &names,
        AssetType type)
    {
      std::vector<Asset> output;

      for (const auto &name : names)
      {
        const auto it = assets.find(name);
        if (it == assets.end())
        {
          continue;
        }

        if (it->second.type() == type)
        {
          output.push_back(it->second);
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

  AssetManager &AssetManager::set_mode(AssetMode mode) noexcept
  {
    mode_ = mode;
    return *this;
  }

  AssetManager &AssetManager::set_version(std::string version)
  {
    version_ = std::move(version);
    return *this;
  }

  AssetManager &AssetManager::set_asset_map(AssetMap asset_map)
  {
    asset_map_ = std::move(asset_map);
    return *this;
  }

  AssetManager &AssetManager::set_manifest(AssetMap asset_map)
  {
    return set_asset_map(std::move(asset_map));
  }

  void AssetManager::clear_asset_map() noexcept
  {
    asset_map_.clear();
  }

  const std::string &AssetManager::base_url() const noexcept
  {
    return base_url_;
  }

  AssetMode AssetManager::mode() const noexcept
  {
    return mode_;
  }

  const std::string &AssetManager::version() const noexcept
  {
    return version_;
  }

  const AssetMap &AssetManager::asset_map() const noexcept
  {
    return asset_map_;
  }

  AssetMap &AssetManager::asset_map() noexcept
  {
    return asset_map_;
  }

  bool AssetManager::has_version() const noexcept
  {
    return !version_.empty();
  }

  bool AssetManager::has_asset_map() const noexcept
  {
    return !asset_map_.empty();
  }

  std::string AssetManager::resolve(std::string_view path) const
  {
    return join_url(base_url_, path);
  }

  std::string AssetManager::path(std::string_view path) const
  {
    if (mode_ == AssetMode::Development)
    {
      return resolve(path);
    }

    if (asset_map_.has(path))
    {
      return resolve(asset_map_.resolve(path));
    }

    return append_version(resolve(path), version_);
  }

  AssetManager &AssetManager::add(std::string name, Asset asset)
  {
    validate_asset_name(name);

    if (!asset.has_url())
    {
      throw AssetError("cannot register UI asset without a URL");
    }

    const bool already_exists = has(name);

    if (!already_exists)
    {
      order_.push_back(name);
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
        Asset::stylesheet(path(href), std::move(media)));
  }

  AssetManager &AssetManager::add_script(
      std::string name,
      std::string src,
      AssetLoading loading)
  {
    return add(
        std::move(name),
        Asset::script(path(src), loading));
  }

  AssetManager &AssetManager::add_module_script(
      std::string name,
      std::string src)
  {
    return add(
        std::move(name),
        Asset::module_script(path(src)));
  }

  AssetManager &AssetManager::add_image(
      std::string name,
      std::string src,
      std::string alt)
  {
    return add(
        std::move(name),
        Asset::image(path(src), std::move(alt)));
  }

  AssetManager &AssetManager::add_font(
      std::string name,
      std::string href)
  {
    return add(
        std::move(name),
        Asset::font(path(href)));
  }

  AssetManager &AssetManager::add_to_group(
      std::string group,
      std::string name,
      Asset asset)
  {
    validate_asset_group(group);

    const std::string group_name = group;
    const std::string asset_name = name;

    add(std::move(name), std::move(asset));
    add_unique(groups_[group_name], asset_name);

    return *this;
  }

  AssetManager &AssetManager::add_stylesheet_to(
      std::string group,
      std::string name,
      std::string href,
      std::string media)
  {
    return add_to_group(
        std::move(group),
        std::move(name),
        Asset::stylesheet(path(href), std::move(media)));
  }

  AssetManager &AssetManager::add_script_to(
      std::string group,
      std::string name,
      std::string src,
      AssetLoading loading)
  {
    return add_to_group(
        std::move(group),
        std::move(name),
        Asset::script(path(src), loading));
  }

  AssetManager &AssetManager::add_module_script_to(
      std::string group,
      std::string name,
      std::string src)
  {
    return add_to_group(
        std::move(group),
        std::move(name),
        Asset::module_script(path(src)));
  }

  bool AssetManager::has(const std::string &name) const
  {
    return assets_.find(name) != assets_.end();
  }

  bool AssetManager::has_group(const std::string &group) const
  {
    return groups_.find(group) != groups_.end();
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

  const std::vector<std::string> *AssetManager::group(
      const std::string &group) const noexcept
  {
    const auto it = groups_.find(group);
    if (it == groups_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  bool AssetManager::remove(const std::string &name)
  {
    const bool removed = assets_.erase(name) > 0;

    if (removed)
    {
      order_.erase(
          std::remove(order_.begin(), order_.end(), name),
          order_.end());

      for (auto it = groups_.begin(); it != groups_.end();)
      {
        remove_from_group(it->second, name);

        if (it->second.empty())
        {
          it = groups_.erase(it);
        }
        else
        {
          ++it;
        }
      }
    }

    return removed;
  }

  bool AssetManager::remove_group(const std::string &group)
  {
    return groups_.erase(group) > 0;
  }

  void AssetManager::clear() noexcept
  {
    assets_.clear();
    order_.clear();
    groups_.clear();
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

  const std::map<std::string, std::vector<std::string>> &
  AssetManager::groups() const noexcept
  {
    return groups_;
  }

  std::map<std::string, std::vector<std::string>> &
  AssetManager::groups() noexcept
  {
    return groups_;
  }

  AssetManifest AssetManager::manifest() const
  {
    AssetManifest manifest;

    for (const auto &name : order_)
    {
      const auto it = assets_.find(name);
      if (it == assets_.end())
      {
        continue;
      }

      manifest.add(it->second);
    }

    return manifest;
  }

  AssetManifest AssetManager::manifest(std::string_view group) const
  {
    const auto it = groups_.find(make_key(group));

    if (it == groups_.end())
    {
      return AssetManifest{};
    }

    return AssetManifest(collect_group_assets(assets_, it->second));
  }

  std::string AssetManager::render() const
  {
    return manifest().render();
  }

  std::string AssetManager::render_group(std::string_view group) const
  {
    return manifest(group).render();
  }

  std::string AssetManager::render_type(AssetType type) const
  {
    return AssetManifest(collect_assets(assets_, order_, type)).render();
  }

  std::string AssetManager::render_type(
      std::string_view group,
      AssetType type) const
  {
    const auto it = groups_.find(make_key(group));

    if (it == groups_.end())
    {
      return {};
    }

    return AssetManifest(
               collect_group_assets(assets_, it->second, type))
        .render();
  }

  std::string AssetManager::render_stylesheets() const
  {
    return render_type(AssetType::Stylesheet);
  }

  std::string AssetManager::render_stylesheets(
      std::string_view group) const
  {
    return render_type(group, AssetType::Stylesheet);
  }

  std::string AssetManager::render_scripts() const
  {
    return render_type(AssetType::Script);
  }

  std::string AssetManager::render_scripts(
      std::string_view group) const
  {
    return render_type(group, AssetType::Script);
  }

  std::string AssetManager::render_fonts() const
  {
    return render_type(AssetType::Font);
  }

} // namespace vix::ui
