/**
 *
 *  @file AssetMode.hpp
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
#ifndef VIX_UI_ASSETS_ASSET_MODE_HPP
#define VIX_UI_ASSETS_ASSET_MODE_HPP

#include <string_view>

namespace vix::ui
{
  /**
   * @brief Asset resolution mode.
   *
   * AssetMode controls how UI assets are resolved before being rendered.
   *
   * Development mode keeps asset paths stable and predictable.
   * Production mode enables production-oriented resolution such as
   * versioned URLs and manifest-based hashed paths.
   */
  enum class AssetMode
  {
    Development = 0,
    Production
  };

  /**
   * @brief Convert an asset mode to a stable string.
   *
   * @param mode Asset mode.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(AssetMode mode) noexcept;

} // namespace vix::ui

#endif // VIX_UI_ASSETS_ASSET_MODE_HPP
