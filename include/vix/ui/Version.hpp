/**
 *
 *  @file Version.hpp
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
#ifndef VIX_UI_VERSION_HPP
#define VIX_UI_VERSION_HPP

#include <string_view>

namespace vix::ui
{
  /**
   * @brief Major version of the UI module.
   */
  inline constexpr int VERSION_MAJOR = 0;

  /**
   * @brief Minor version of the UI module.
   */
  inline constexpr int VERSION_MINOR = 1;

  /**
   * @brief Patch version of the UI module.
   */
  inline constexpr int VERSION_PATCH = 0;

  /**
   * @brief Full semantic version string of the UI module.
   */
  inline constexpr std::string_view VERSION = "1.0.0";

  /**
   * @brief Get the major version of the UI module.
   *
   * @return Major version number.
   */
  [[nodiscard]] int version_major() noexcept;

  /**
   * @brief Get the minor version of the UI module.
   *
   * @return Minor version number.
   */
  [[nodiscard]] int version_minor() noexcept;

  /**
   * @brief Get the patch version of the UI module.
   *
   * @return Patch version number.
   */
  [[nodiscard]] int version_patch() noexcept;

  /**
   * @brief Get the full semantic version string of the UI module.
   *
   * @return Version string.
   */
  [[nodiscard]] std::string_view version() noexcept;

} // namespace vix::ui

#endif // VIX_UI_VERSION_HPP
