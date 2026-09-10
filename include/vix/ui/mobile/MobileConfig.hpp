/**
 *
 *  @file MobileConfig.hpp
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
#ifndef VIX_UI_MOBILE_MOBILE_CONFIG_HPP
#define VIX_UI_MOBILE_MOBILE_CONFIG_HPP

#include <string>

#include <vix/ui/support/Result.hpp>

namespace vix::ui
{
  /**
   * @brief Common identity and configuration for a mobile application.
   *
   * This value type holds only cross-platform identity and Web application
   * inputs. Platform build settings and platform-specific icon processing
   * belong to their respective backends.
   */
  class MobileConfig
  {
  public:
    /**
     * @brief Construct a default mobile configuration.
     */
    MobileConfig() = default;

    /**
     * @brief Create a default mobile configuration.
     *
     * @return Mobile configuration.
     */
    [[nodiscard]] static MobileConfig make();

    /** @brief Set the human-readable application name. */
    MobileConfig &set_name(std::string name);

    /**
     * @brief Set the stable application identifier.
     *
     * Backends may impose additional identifier rules. For example, Android
     * requires a valid Java package name.
     */
    MobileConfig &set_app_id(std::string app_id);

    /** @brief Set the application version. */
    MobileConfig &set_version(std::string version);

    /** @brief Set the initial URL loaded by the mobile application. */
    MobileConfig &set_url(std::string url);

    /**
     * @brief Set the optional source path for the application icon.
     *
     * Supported file formats are backend-specific.
     */
    MobileConfig &set_icon_path(std::string path);

    /** @brief Get the human-readable application name. */
    [[nodiscard]] const std::string &name() const noexcept;

    /** @brief Get the stable application id. */
    [[nodiscard]] const std::string &app_id() const noexcept;

    /** @brief Get the application version. */
    [[nodiscard]] const std::string &version() const noexcept;

    /** @brief Get the URL loaded by the mobile application. */
    [[nodiscard]] const std::string &url() const noexcept;

    /** @brief Get the optional application icon path. */
    [[nodiscard]] const std::string &icon_path() const noexcept;

    /** @brief Check whether an application name is set. */
    [[nodiscard]] bool has_name() const noexcept;

    /** @brief Check whether an application id is set. */
    [[nodiscard]] bool has_app_id() const noexcept;

    /** @brief Check whether an application version is set. */
    [[nodiscard]] bool has_version() const noexcept;

    /** @brief Check whether a target URL is set. */
    [[nodiscard]] bool has_url() const noexcept;

    /** @brief Check whether an icon path is set. */
    [[nodiscard]] bool has_icon_path() const noexcept;

    /**
     * @brief Validate the required mobile application configuration.
     *
     * This checks only that the required common values are present.
     * Backend-specific format and compatibility checks are deliberately left
     * to mobile backends.
     *
     * @return Successful result when the required fields are set.
     */
    [[nodiscard]] Result<void> validate() const;

  private:
    std::string name_{"Vix Mobile App"};
    std::string app_id_;
    std::string version_{"0.1.0"};
    std::string url_{"https://example.com"};
    std::string icon_path_;
  };

} // namespace vix::ui

#endif // VIX_UI_MOBILE_MOBILE_CONFIG_HPP
