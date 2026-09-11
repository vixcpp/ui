/**
 *
 *  @file IOSProject.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */
#ifndef VIX_UI_MOBILE_IOS_PROJECT_HPP
#define VIX_UI_MOBILE_IOS_PROJECT_HPP

#include <filesystem>
#include <string>

#include <vix/ui/mobile/MobileProject.hpp>

namespace vix::ui
{
  /**
   * @brief iOS mobile project generator.
   *
   * IOSProject extends the common MobileProject value with the minimum iOS
   * deployment setting needed to generate an Xcode project.
   */
  class IOSProject : public MobileProject
  {
  public:
    IOSProject() = default;
    explicit IOSProject(MobileProject project);

    /**
     * @brief Set the minimum supported iOS version.
     *
     * The value uses Xcode's dotted numeric format, for example "15.0".
     */
    IOSProject &set_deployment_target(std::string target);

    /** @brief Get the minimum supported iOS version. */
    [[nodiscard]] const std::string &deployment_target() const noexcept;

    /**
     * @brief Generate or update an Xcode project in a directory.
     *
     * Configuration is validated before writing. The generated project
     * includes a Swift WKWebView application, Info.plist and asset catalog.
     *
     * @param directory Output directory for the iOS project.
     * @return Failure for invalid configuration or a filesystem error.
     */
    [[nodiscard]] Result<void> generate(
        const std::filesystem::path &directory) const;

    /**
     * @brief Validate the shared mobile and iOS-specific configuration.
     *
     * @return Failure for invalid bundle identifiers, URL schemes,
     *         deployment targets, or icon inputs.
     */
    [[nodiscard]] Result<void> validate() const;

  private:
    std::string deployment_target_{"15.0"};
  };
} // namespace vix::ui

#endif // VIX_UI_MOBILE_IOS_PROJECT_HPP
