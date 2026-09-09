/**
 *
 *  @file AndroidProject.hpp
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
#ifndef VIX_UI_MOBILE_ANDROID_PROJECT_HPP
#define VIX_UI_MOBILE_ANDROID_PROJECT_HPP

#include <filesystem>
#include <string>

#include <vix/ui/mobile/MobileProject.hpp>

namespace vix::ui
{
  /**
   * @brief Android-specific configuration for a mobile project.
   *
   * AndroidProject extends the common mobile project boundary with the
   * Android version metadata required by a future project generator.
   */
  class AndroidProject : public MobileProject
  {
  public:
    /**
     * @brief Construct an Android project with default configuration.
     */
    AndroidProject() = default;

    /**
     * @brief Construct an Android project from a common mobile project.
     *
     * @param project Common mobile project configuration.
     */
    explicit AndroidProject(MobileProject project);

    /** @brief Set the minimum supported Android SDK level. */
    AndroidProject &set_min_sdk(int min_sdk) noexcept;

    /** @brief Set the target Android SDK level. */
    AndroidProject &set_target_sdk(int target_sdk) noexcept;

    /** @brief Set the Android SDK level used for compilation. */
    AndroidProject &set_compile_sdk(int compile_sdk) noexcept;

    /** @brief Set the Android application version code. */
    AndroidProject &set_version_code(int version_code) noexcept;

    /** @brief Set the Android Gradle Plugin version. */
    AndroidProject &set_android_gradle_plugin_version(std::string version);

    /** @brief Get the minimum supported Android SDK level. */
    [[nodiscard]] int min_sdk() const noexcept;

    /** @brief Get the target Android SDK level. */
    [[nodiscard]] int target_sdk() const noexcept;

    /** @brief Get the Android SDK level used for compilation. */
    [[nodiscard]] int compile_sdk() const noexcept;

    /** @brief Get the Android application version code. */
    [[nodiscard]] int version_code() const noexcept;

    /** @brief Get the Android Gradle Plugin version. */
    [[nodiscard]] const std::string &android_gradle_plugin_version() const noexcept;

    /**
     * @brief Generate the base Android project files.
     *
     * The output contains only the Gradle settings and build configuration
     * required by the Android project skeleton.
     *
     * @param directory Output directory for the Android project.
     * @return Successful result when all files were generated.
     */
    [[nodiscard]] Result<void> generate(
        const std::filesystem::path &directory) const;

    /**
     * @brief Validate the common and Android-specific configuration.
     *
     * @return Successful result when the project configuration is valid.
     */
    [[nodiscard]] Result<void> validate() const;

  private:
    int min_sdk_{23};
    int target_sdk_{36};
    int compile_sdk_{36};
    int version_code_{1};
    std::string android_gradle_plugin_version_{"8.13.2"};
  };

} // namespace vix::ui

#endif // VIX_UI_MOBILE_ANDROID_PROJECT_HPP
