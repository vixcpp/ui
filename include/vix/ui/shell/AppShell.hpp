/**
 *
 *  @file AppShell.hpp
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
#ifndef VIX_UI_SHELL_APP_SHELL_HPP
#define VIX_UI_SHELL_APP_SHELL_HPP

#include <string>

#include <vix/ui/platform/Platform.hpp>
#include <vix/ui/shell/ShellConfig.hpp>
#include <vix/ui/support/Result.hpp>

namespace vix::ui
{
  /**
   * @brief Lightweight app shell descriptor for Vix UI applications.
   *
   * AppShell represents the shell around a Vix UI application.
   *
   * The first implementation is intentionally small and platform-neutral:
   * - it stores shell configuration
   * - validates shell settings
   * - tracks whether the shell is running
   * - exposes the effective target URL
   *
   * Later desktop/mobile implementations can use this class as the public
   * control surface for WebView-based app shells.
   */
  class AppShell
  {
  public:
    /**
     * @brief Construct an app shell with default configuration.
     */
    AppShell() = default;

    /**
     * @brief Construct an app shell from a configuration.
     *
     * @param config Shell configuration.
     */
    explicit AppShell(ShellConfig config);

    /**
     * @brief Create an app shell from a configuration.
     *
     * @param config Shell configuration.
     * @return App shell.
     */
    [[nodiscard]] static AppShell make(ShellConfig config = {});

    /**
     * @brief Replace the shell configuration.
     *
     * @param config Shell configuration.
     * @return This app shell.
     */
    AppShell &set_config(ShellConfig config);

    /**
     * @brief Access the shell configuration.
     *
     * @return Immutable shell configuration.
     */
    [[nodiscard]] const ShellConfig &config() const noexcept;

    /**
     * @brief Access the shell configuration.
     *
     * @return Mutable shell configuration.
     */
    [[nodiscard]] ShellConfig &config() noexcept;

    /**
     * @brief Validate the current shell configuration.
     *
     * @return Successful result if valid, failed result otherwise.
     */
    [[nodiscard]] Result<void> validate() const;

    /**
     * @brief Start the app shell.
     *
     * This initial implementation validates the shell configuration and
     * marks the shell as running. Native window/WebView startup can be
     * attached here in later platform-specific implementations.
     *
     * @return Successful result if the shell started.
     */
    Result<void> start();

    /**
     * @brief Stop the app shell.
     *
     * @return Successful result.
     */
    Result<void> stop();

    /**
     * @brief Restart the app shell.
     *
     * @return Successful result if the shell restarted.
     */
    Result<void> restart();

    /**
     * @brief Check whether the shell is running.
     *
     * @return True if running.
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * @brief Check whether the shell is stopped.
     *
     * @return True if stopped.
     */
    [[nodiscard]] bool stopped() const noexcept;

    /**
     * @brief Get the effective target URL loaded by the shell.
     *
     * @return Effective target URL.
     */
    [[nodiscard]] std::string target_url() const;

    /**
     * @brief Get the target platform.
     *
     * @return Platform descriptor.
     */
    [[nodiscard]] const Platform &platform() const noexcept;

    /**
     * @brief Get a stable human-readable shell status.
     *
     * @return Shell status string.
     */
    [[nodiscard]] std::string status() const;

  private:
    /**
     * @brief Shell configuration.
     */
    ShellConfig config_{};

    /**
     * @brief Whether the shell is currently running.
     */
    bool running_{false};
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_APP_SHELL_HPP
