/**
 *
 *  @file Platform.hpp
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
#ifndef VIX_UI_PLATFORM_PLATFORM_HPP
#define VIX_UI_PLATFORM_PLATFORM_HPP

#include <string_view>

namespace vix::ui
{
  /**
   * @brief Supported UI platform kinds.
   *
   * PlatformKind identifies the concrete platform where a UI app or
   * app shell is expected to run.
   */
  enum class PlatformKind
  {
    Unknown = 0,
    Linux,
    Windows,
    MacOS,
    Android,
    IOS,
    Web
  };

  /**
   * @brief Supported UI platform families.
   *
   * PlatformFamily groups concrete platforms into broader runtime targets:
   * - desktop
   * - mobile
   * - web
   */
  enum class PlatformFamily
  {
    Unknown = 0,
    Desktop,
    Mobile,
    Web
  };

  /**
   * @brief Convert a platform kind to a stable string.
   *
   * @param kind Platform kind.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(PlatformKind kind) noexcept;

  /**
   * @brief Convert a platform family to a stable string.
   *
   * @param family Platform family.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(PlatformFamily family) noexcept;

  /**
   * @brief Basic UI platform descriptor.
   *
   * Platform stores the concrete platform kind and its broader platform
   * family. It is intentionally small because the first UI module version
   * is web-first, while desktop and mobile shells will be added later.
   */
  class Platform
  {
  public:
    /**
     * @brief Construct an unknown platform.
     */
    Platform() = default;

    /**
     * @brief Construct a platform from a concrete platform kind.
     *
     * @param kind Platform kind.
     */
    explicit Platform(PlatformKind kind) noexcept;

    /**
     * @brief Detect the current compile-time platform.
     *
     * @return Detected platform.
     */
    [[nodiscard]] static Platform current() noexcept;

    /**
     * @brief Create a web platform descriptor.
     *
     * @return Web platform.
     */
    [[nodiscard]] static Platform web() noexcept;

    /**
     * @brief Create a Linux platform descriptor.
     *
     * @return Linux platform.
     */
    [[nodiscard]] static Platform linux() noexcept;

    /**
     * @brief Create a Windows platform descriptor.
     *
     * @return Windows platform.
     */
    [[nodiscard]] static Platform windows() noexcept;

    /**
     * @brief Create a macOS platform descriptor.
     *
     * @return macOS platform.
     */
    [[nodiscard]] static Platform macos() noexcept;

    /**
     * @brief Create an Android platform descriptor.
     *
     * @return Android platform.
     */
    [[nodiscard]] static Platform android() noexcept;

    /**
     * @brief Create an iOS platform descriptor.
     *
     * @return iOS platform.
     */
    [[nodiscard]] static Platform ios() noexcept;

    /**
     * @brief Get the concrete platform kind.
     *
     * @return Platform kind.
     */
    [[nodiscard]] PlatformKind kind() const noexcept;

    /**
     * @brief Get the broader platform family.
     *
     * @return Platform family.
     */
    [[nodiscard]] PlatformFamily family() const noexcept;

    /**
     * @brief Check whether this platform is known.
     *
     * @return True if the platform kind is not unknown.
     */
    [[nodiscard]] bool known() const noexcept;

    /**
     * @brief Check whether this platform is unknown.
     *
     * @return True if the platform kind is unknown.
     */
    [[nodiscard]] bool unknown() const noexcept;

    /**
     * @brief Check whether this platform is a desktop platform.
     *
     * @return True if desktop.
     */
    [[nodiscard]] bool desktop() const noexcept;

    /**
     * @brief Check whether this platform is a mobile platform.
     *
     * @return True if mobile.
     */
    [[nodiscard]] bool mobile() const noexcept;

    /**
     * @brief Check whether this platform is a web platform.
     *
     * @return True if web.
     */
    [[nodiscard]] bool web_platform() const noexcept;

    /**
     * @brief Check whether this platform is Linux.
     *
     * @return True if Linux.
     */
    [[nodiscard]] bool is_linux() const noexcept;

    /**
     * @brief Check whether this platform is Windows.
     *
     * @return True if Windows.
     */
    [[nodiscard]] bool is_windows() const noexcept;

    /**
     * @brief Check whether this platform is macOS.
     *
     * @return True if macOS.
     */
    [[nodiscard]] bool is_macos() const noexcept;

    /**
     * @brief Check whether this platform is Android.
     *
     * @return True if Android.
     */
    [[nodiscard]] bool is_android() const noexcept;

    /**
     * @brief Check whether this platform is iOS.
     *
     * @return True if iOS.
     */
    [[nodiscard]] bool is_ios() const noexcept;

    /**
     * @brief Get the stable platform name.
     *
     * @return Platform name.
     */
    [[nodiscard]] std::string_view name() const noexcept;

  private:
    /**
     * @brief Concrete platform kind.
     */
    PlatformKind kind_{PlatformKind::Unknown};
  };

} // namespace vix::ui

#endif // VIX_UI_PLATFORM_PLATFORM_HPP
