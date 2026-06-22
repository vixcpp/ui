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

#include <memory>
#include <string>

#include <vix/ui/platform/Platform.hpp>
#include <vix/ui/shell/ShellConfig.hpp>
#include <vix/ui/support/Result.hpp>

namespace vix::ui
{
  class ServerProcess;
  class ShellBackend;

  /**
   * @brief Public app shell facade for Vix UI applications.
   *
   * AppShell is the stable public entry point used to run a Vix UI
   * application inside an application shell.
   *
   * The shell owns:
   * - the shell configuration
   * - the platform/backend selection
   * - the lifecycle state
   *
   * Platform-specific behavior is delegated internally to shell backends.
   * This allows the public API to remain stable while Linux, Windows,
   * macOS, web-only, test, and future mobile backends are added.
   *
   * For v0.3.0, AppShell is expected to become the entry point for the
   * desktop shell MVP, where a local Vix web application can be opened in
   * a desktop WebView window.
   */
  class AppShell
  {
  public:
    /**
     * @brief Construct an app shell with the default shell configuration.
     *
     * The default configuration points to the local URL built from
     * ShellConfig defaults:
     *
     * @code
     * http://127.0.0.1:8080
     * @endcode
     */
    AppShell();

    /**
     * @brief Construct an app shell from an explicit configuration.
     *
     * @param config Shell configuration.
     */
    explicit AppShell(ShellConfig config);

    /**
     * @brief Destroy the app shell.
     *
     * The destructor is defined out-of-line because AppShell owns an
     * internal backend through a std::unique_ptr to an incomplete type.
     */
    ~AppShell();

    /**
     * @brief Copy-construct an app shell.
     *
     * The internal backend is cloned so the copied shell owns its own
     * backend instance.
     *
     * @param other App shell to copy.
     */
    AppShell(const AppShell &other);

    /**
     * @brief Copy-assign an app shell.
     *
     * The internal backend is cloned so the assigned shell owns its own
     * backend instance.
     *
     * @param other App shell to copy from.
     * @return This app shell.
     */
    AppShell &operator=(const AppShell &other);

    /**
     * @brief Move-construct an app shell.
     *
     * @param other App shell to move from.
     */
    AppShell(AppShell &&other) noexcept;

    /**
     * @brief Move-assign an app shell.
     *
     * @param other App shell to move from.
     * @return This app shell.
     */
    AppShell &operator=(AppShell &&other) noexcept;

    /**
     * @brief Create an app shell from a configuration.
     *
     * This is a convenience factory for fluent application code.
     *
     * @param config Shell configuration.
     * @return App shell.
     */
    [[nodiscard]] static AppShell make(ShellConfig config = {});

    /**
     * @brief Replace the shell configuration.
     *
     * If the shell is stopped, the internal backend can be recreated from
     * the new configuration. If the shell is already running, the new
     * configuration is stored, but the running backend is not restarted
     * automatically.
     *
     * @param config New shell configuration.
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
     * Validation checks that:
     * - the application name is not empty
     * - the title is not empty
     * - the width is greater than zero
     * - the height is greater than zero
     * - the effective target URL is not empty
     *
     * @return Successful result if the configuration is valid.
     */
    [[nodiscard]] Result<void> validate() const;

    /**
     * @brief Start the app shell.
     *
     * This validates the configuration and delegates startup to the
     * selected internal shell backend.
     *
     * If the shell is already running, this returns a successful result
     * without starting a second backend.
     *
     * @return Successful result if the shell started or was already running.
     */
    Result<void> start();

    /**
     * @brief Stop the app shell.
     *
     * This delegates shutdown to the selected internal shell backend.
     *
     * If the shell is already stopped, this returns a successful result.
     *
     * @return Successful result if the shell stopped or was already stopped.
     */
    Result<void> stop();

    /**
     * @brief Restart the app shell.
     *
     * This stops the current backend and starts it again with the current
     * configuration.
     *
     * @return Successful result if the shell restarted.
     */
    Result<void> restart();

    /**
     * @brief Check whether the shell is currently running.
     *
     * @return True if the internal backend is running.
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * @brief Check whether the shell is currently stopped.
     *
     * @return True if the internal backend is not running.
     */
    [[nodiscard]] bool stopped() const noexcept;

    /**
     * @brief Get the effective target URL loaded by the shell.
     *
     * If ShellConfig contains an explicit URL, that URL is returned.
     * Otherwise, the local URL is built from the configured host and port.
     *
     * Example:
     *
     * @code
     * http://127.0.0.1:8080
     * @endcode
     *
     * @return Effective shell target URL.
     */
    [[nodiscard]] std::string target_url() const;

    /**
     * @brief Get the configured target platform.
     *
     * @return Platform descriptor.
     */
    [[nodiscard]] const Platform &platform() const noexcept;

    /**
     * @brief Get a stable human-readable shell status.
     *
     * Typical values:
     * - "running"
     * - "stopped"
     *
     * @return Shell status string.
     */
    [[nodiscard]] std::string status() const;

  private:
    /**
     * @brief Start the configured local server process if needed.
     *
     * The server is started only when:
     * - ShellConfig::start_server() is true
     * - ShellConfig::has_server_command() is true
     *
     * If no server command is configured, this returns success and does
     * nothing.
     *
     * @return Successful result if no server is needed or the server started.
     */
    Result<void> start_server_if_needed();

    /**
     * @brief Stop the local server process if one is running.
     *
     * @return Successful result if no server is running or the server stopped.
     */
    Result<void> stop_server_if_needed();

    /**
     * @brief Wait for the configured local server to become reachable.
     *
     * The readiness check runs only when:
     * - ShellConfig::start_server() is true
     * - ShellConfig::wait_for_server() is true
     * - ShellConfig::has_server_command() is true
     *
     * The readiness URL comes from
     * ShellConfig::effective_readiness_url().
     *
     * @return Successful result if no wait is needed or the server is ready.
     */
    Result<void> wait_for_server_if_needed();

    /**
     * @brief Create the internal backend for the current configuration.
     *
     * This is the backend selection point.
     *
     * Future versions can select:
     * - Linux desktop WebView backend
     * - Windows desktop WebView backend
     * - macOS desktop WebView backend
     * - descriptor-only backend
     * - test/null backend
     *
     * @return Newly created shell backend.
     */
    [[nodiscard]] std::unique_ptr<ShellBackend> make_backend() const;

    /**
     * @brief Start the selected platform shell backend.
     *
     * @return Successful result if the backend started.
     */
    Result<void> start_platform_shell();

    /**
     * @brief Stop the selected platform shell backend.
     *
     * @return Successful result if the backend stopped.
     */
    Result<void> stop_platform_shell();

  private:
    /**
     * @brief Shell configuration.
     */
    ShellConfig config_{};

    /**
     * @brief Optional local server process owned by the shell.
     *
     * This is used when ShellConfig asks the shell to start a local Vix
     * server before opening the target URL.
     */
    std::unique_ptr<ServerProcess> server_process_;

    /**
     * @brief Internal platform-specific shell backend.
     *
     * The backend is intentionally hidden behind the public AppShell facade
     * so applications do not depend on backend implementation details.
     */
    std::unique_ptr<ShellBackend> backend_;
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_APP_SHELL_HPP
