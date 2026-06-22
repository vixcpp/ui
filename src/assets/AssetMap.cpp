/**
 *
 *  @file AssetMap.cpp
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
#include <vix/ui/assets/AssetMap.hpp>
#include <vix/ui/assets/AssetMode.hpp>
#include <vix/ui/support/Error.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] std::string make_key(std::string_view value)
    {
      return std::string(value);
    }

    void validate_mapping(
        const std::string &logical_path,
        const std::string &resolved_path)
    {
      if (logical_path.empty())
      {
        throw AssetError("cannot register UI asset map entry with an empty logical path");
      }

      if (resolved_path.empty())
      {
        throw AssetError("cannot register UI asset map entry with an empty resolved path");
      }
    }
  } // namespace

  std::string_view to_string(AssetMode mode) noexcept
  {
    switch (mode)
    {
    case AssetMode::Development:
      return "development";

    case AssetMode::Production:
      return "production";
    }

    return "development";
  }

  AssetMap::AssetMap(std::map<std::string, std::string> entries)
  {
    for (auto &[logical_path, resolved_path] : entries)
    {
      set(std::move(logical_path), std::move(resolved_path));
    }
  }

  AssetMap AssetMap::make()
  {
    return AssetMap{};
  }

  AssetMap AssetMap::from(
      std::map<std::string, std::string> entries)
  {
    return AssetMap(std::move(entries));
  }

  AssetMap &AssetMap::set(
      std::string logical_path,
      std::string resolved_path)
  {
    validate_mapping(logical_path, resolved_path);

    entries_[std::move(logical_path)] = std::move(resolved_path);
    return *this;
  }

  bool AssetMap::has(std::string_view logical_path) const noexcept
  {
    return entries_.find(make_key(logical_path)) != entries_.end();
  }

  const std::string *AssetMap::get(
      std::string_view logical_path) const noexcept
  {
    const auto it = entries_.find(make_key(logical_path));

    if (it == entries_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  std::string AssetMap::get_or(
      std::string_view logical_path,
      std::string fallback) const
  {
    const std::string *resolved = get(logical_path);

    if (resolved == nullptr)
    {
      return fallback;
    }

    return *resolved;
  }

  std::string AssetMap::resolve(
      std::string_view logical_path) const
  {
    const std::string *resolved = get(logical_path);

    if (resolved == nullptr)
    {
      return std::string(logical_path);
    }

    return *resolved;
  }

  bool AssetMap::remove(std::string_view logical_path)
  {
    return entries_.erase(make_key(logical_path)) > 0;
  }

  void AssetMap::clear() noexcept
  {
    entries_.clear();
  }

  bool AssetMap::empty() const noexcept
  {
    return entries_.empty();
  }

  std::size_t AssetMap::size() const noexcept
  {
    return entries_.size();
  }

  const std::map<std::string, std::string> &
  AssetMap::entries() const noexcept
  {
    return entries_;
  }

  std::map<std::string, std::string> &
  AssetMap::entries() noexcept
  {
    return entries_;
  }

} // namespace vix::ui
