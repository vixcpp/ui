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
   * @brief Android build variant.
   *
   * Release selects Gradle's release variant only; signing remains an
   * external concern.
   */
  enum class AndroidBuildType
  {
    Debug,
    Release
  };

  /**
   * @brief Android artifact requested from Gradle.
   *
   * AAB is supported only for release builds.
   */
  enum class AndroidArtifact
  {
    Apk,
    Aab
  };

  /**
   * @brief Android-specific mobile project generator and builder.
   *
   * AndroidProject extends the shared MobileProject value with the Android
   * metadata required to generate and build a complete Android project.
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

    /**
     * @brief Set an optional explicit Gradle executable.
     *
     * A project Gradle Wrapper still takes precedence over this command.
     *
     * @param command Gradle executable path or command name.
     * @return This Android project.
     */
    AndroidProject &set_gradle_command(std::filesystem::path command);

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
     * @brief Get the optional explicit Gradle executable.
     *
     * @return Gradle executable path or command name.
     */
    [[nodiscard]] const std::filesystem::path &gradle_command() const noexcept;

    /**
     * @brief Check whether an explicit Gradle executable is configured.
     *
     * @return True when a Gradle command is configured.
     */
    [[nodiscard]] bool has_gradle_command() const noexcept;

    /**
     * @brief Generate or update an Android project in a directory.
     *
     * Configuration is validated before writing. On success, the directory
     * contains the Gradle project, manifest, Java activity and required
     * resources. Existing generated files may be updated.
     *
     * @param directory Output directory for the Android project.
     * @return Failure for invalid configuration or a filesystem error.
     */
    [[nodiscard]] Result<void> generate(
        const std::filesystem::path &directory) const;

    /**
     * @brief Build an APK or AAB from an existing generated Android project.
     *
     * The project wrapper is preferred, followed by an explicit Gradle
     * command and then a Gradle executable found on the system PATH.
     *
     * @param directory Directory containing the generated Gradle project.
     * @param type Gradle build variant.
     * @param artifact Requested artifact kind.
     * @return The resolved produced artifact path, or failure when the
     *         project, Gradle command, Gradle task, or artifact is missing.
     */
    [[nodiscard]] Result<std::filesystem::path> build(
        const std::filesystem::path &directory,
        AndroidBuildType type = AndroidBuildType::Debug,
        AndroidArtifact artifact = AndroidArtifact::Apk) const;

    /**
     * @brief Validate the common and Android-specific configuration.
     *
     * @return Failure when shared mobile values, Android SDK/version values,
     *         Android application id, URL scheme, or icon input are invalid.
     */
    [[nodiscard]] Result<void> validate() const;

  private:
    int min_sdk_{23};
    int target_sdk_{36};
    int compile_sdk_{36};
    int version_code_{1};
    std::string android_gradle_plugin_version_{"8.13.2"};
    std::filesystem::path gradle_command_;
  };

} // namespace vix::ui

#endif // VIX_UI_MOBILE_ANDROID_PROJECT_HPP
