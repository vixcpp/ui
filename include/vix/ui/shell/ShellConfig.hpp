/**
 *
 *  @file ShellConfig.hpp
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
#ifndef VIX_UI_SHELL_SHELL_CONFIG_HPP
#define VIX_UI_SHELL_SHELL_CONFIG_HPP

#include <chrono>
#include <cstdint>
#include <string>

#include <vix/ui/platform/Platform.hpp>

namespace vix::ui
{
  /**
   * @brief Configuration for a UI app shell.
   *
   * ShellConfig stores the settings needed to describe how a Vix UI
   * application should be presented by an app shell.
   *
   * It keeps the shell web-first while providing enough metadata and
   * lifecycle configuration for desktop WebView shells.
   */
  class ShellConfig
  {
  public:
    /**
     * @brief Construct a default shell configuration.
     */
    ShellConfig() = default;

    /**
     * @brief Create a default shell configuration.
     *
     * @return Shell configuration.
     */
    [[nodiscard]] static ShellConfig make();

    /**
     * @brief Set the shell application name.
     *
     * The name is the human-readable application name.
     *
     * @param name Application name.
     * @return This configuration.
     */
    ShellConfig &set_name(std::string name);

    /**
     * @brief Set the shell application title.
     *
     * The title is usually used as the desktop window title.
     *
     * @param title Window or page title.
     * @return This configuration.
     */
    ShellConfig &set_title(std::string title);

    /**
     * @brief Set the stable desktop application id.
     *
     * The app id is useful for future packaging, desktop entries,
     * notifications, settings storage and platform integration.
     *
     * Example:
     *
     * @code
     * com.softadastra.vix.demo
     * @endcode
     *
     * @param app_id Stable application id.
     * @return This configuration.
     */
    ShellConfig &set_app_id(std::string app_id);

    /**
     * @brief Set the application version metadata.
     *
     * This value describes the app using the shell, not the Vix UI module
     * version itself.
     *
     * @param version Application version.
     * @return This configuration.
     */
    ShellConfig &set_app_version(std::string version);

    /**
     * @brief Set the application vendor metadata.
     *
     * @param vendor Vendor or organization name.
     * @return This configuration.
     */
    ShellConfig &set_vendor(std::string vendor);

    /**
     * @brief Set the icon path used by desktop shells.
     *
     * The path can point to a PNG, SVG, ICO or any format supported by the
     * active backend/platform.
     *
     * @param path Icon file path.
     * @return This configuration.
     */
    ShellConfig &set_icon_path(std::string path);

    /**
     * @brief Set the target URL loaded by the shell.
     *
     * If this value is empty, the shell uses local_url().
     *
     * @param url Target URL.
     * @return This configuration.
     */
    ShellConfig &set_url(std::string url);

    /**
     * @brief Set the readiness URL checked before opening the shell.
     *
     * If this value is empty, effective_readiness_url() falls back to
     * effective_url().
     *
     * @param url Readiness URL.
     * @return This configuration.
     */
    ShellConfig &set_readiness_url(std::string url);

    /**
     * @brief Set the host used by a local UI shell.
     *
     * @param host Host address.
     * @return This configuration.
     */
    ShellConfig &set_host(std::string host);

    /**
     * @brief Set the port used by a local UI shell.
     *
     * @param port Port number.
     * @return This configuration.
     */
    ShellConfig &set_port(std::uint16_t port) noexcept;

    /**
     * @brief Set the target platform.
     *
     * @param platform Platform descriptor.
     * @return This configuration.
     */
    ShellConfig &set_platform(Platform platform) noexcept;

    /**
     * @brief Set the initial shell width.
     *
     * @param width Width in pixels.
     * @return This configuration.
     */
    ShellConfig &set_width(int width) noexcept;

    /**
     * @brief Set the initial shell height.
     *
     * @param height Height in pixels.
     * @return This configuration.
     */
    ShellConfig &set_height(int height) noexcept;

    /**
     * @brief Set whether the shell should be resizable.
     *
     * @param value Resizable flag.
     * @return This configuration.
     */
    ShellConfig &set_resizable(bool value) noexcept;

    /**
     * @brief Set whether the shell should start fullscreen.
     *
     * @param value Fullscreen flag.
     * @return This configuration.
     */
    ShellConfig &set_fullscreen(bool value) noexcept;

    /**
     * @brief Set whether developer tools should be enabled.
     *
     * @param value Developer tools flag.
     * @return This configuration.
     */
    ShellConfig &set_devtools(bool value) noexcept;

    /**
     * @brief Set whether the local server should be started by the shell.
     *
     * @param value Start-server flag.
     * @return This configuration.
     */
    ShellConfig &set_start_server(bool value) noexcept;

    /**
     * @brief Set whether the shell should wait for the local server.
     *
     * When enabled, AppShell can wait until the readiness URL is reachable
     * before opening the desktop WebView.
     *
     * @param value Wait-for-server flag.
     * @return This configuration.
     */
    ShellConfig &set_wait_for_server(bool value) noexcept;

    /**
     * @brief Set the startup timeout used by server readiness checks.
     *
     * @param timeout Maximum startup wait duration.
     * @return This configuration.
     */
    ShellConfig &set_startup_timeout(std::chrono::milliseconds timeout) noexcept;

    /**
     * @brief Set the local server command started by the shell.
     *
     * This command is optional. If it is empty, the shell only opens the
     * configured URL and does not start an external server process.
     *
     * Example:
     *
     * @code
     * vix run examples/desktop_app.cpp --port 8080
     * @endcode
     *
     * @param command Server command.
     * @return This configuration.
     */
    ShellConfig &set_server_command(std::string command);

    /**
     * @brief Set the working directory used by the local server command.
     *
     * If empty, the current process working directory is used.
     *
     * @param directory Working directory.
     * @return This configuration.
     */
    ShellConfig &set_server_working_directory(std::string directory);

    /**
     * @brief Get the shell application name.
     *
     * @return Application name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Get the shell application title.
     *
     * @return Window or page title.
     */
    [[nodiscard]] const std::string &title() const noexcept;

    /**
     * @brief Get the stable desktop application id.
     *
     * @return Application id.
     */
    [[nodiscard]] const std::string &app_id() const noexcept;

    /**
     * @brief Get the application version metadata.
     *
     * @return Application version.
     */
    [[nodiscard]] const std::string &app_version() const noexcept;

    /**
     * @brief Get the application vendor metadata.
     *
     * @return Vendor name.
     */
    [[nodiscard]] const std::string &vendor() const noexcept;

    /**
     * @brief Get the desktop icon path.
     *
     * @return Icon path.
     */
    [[nodiscard]] const std::string &icon_path() const noexcept;

    /**
     * @brief Get the target URL.
     *
     * @return Target URL.
     */
    [[nodiscard]] const std::string &url() const noexcept;

    /**
     * @brief Get the readiness URL.
     *
     * @return Readiness URL.
     */
    [[nodiscard]] const std::string &readiness_url() const noexcept;

    /**
     * @brief Get the local shell host.
     *
     * @return Host address.
     */
    [[nodiscard]] const std::string &host() const noexcept;

    /**
     * @brief Get the local shell port.
     *
     * @return Port number.
     */
    [[nodiscard]] std::uint16_t port() const noexcept;

    /**
     * @brief Get the target platform.
     *
     * @return Platform descriptor.
     */
    [[nodiscard]] const Platform &platform() const noexcept;

    /**
     * @brief Get the initial shell width.
     *
     * @return Width in pixels.
     */
    [[nodiscard]] int width() const noexcept;

    /**
     * @brief Get the initial shell height.
     *
     * @return Height in pixels.
     */
    [[nodiscard]] int height() const noexcept;

    /**
     * @brief Check whether the shell is resizable.
     *
     * @return True if resizable.
     */
    [[nodiscard]] bool resizable() const noexcept;

    /**
     * @brief Check whether the shell starts fullscreen.
     *
     * @return True if fullscreen.
     */
    [[nodiscard]] bool fullscreen() const noexcept;

    /**
     * @brief Check whether developer tools are enabled.
     *
     * @return True if developer tools are enabled.
     */
    [[nodiscard]] bool devtools() const noexcept;

    /**
     * @brief Check whether the shell should start the local server.
     *
     * @return True if the shell should start the server.
     */
    [[nodiscard]] bool start_server() const noexcept;

    /**
     * @brief Check whether the shell should wait for the local server.
     *
     * @return True if readiness waiting is enabled.
     */
    [[nodiscard]] bool wait_for_server() const noexcept;

    /**
     * @brief Get the startup timeout used by readiness checks.
     *
     * @return Startup timeout.
     */
    [[nodiscard]] std::chrono::milliseconds startup_timeout() const noexcept;

    /**
     * @brief Get the local server command.
     *
     * @return Server command.
     */
    [[nodiscard]] const std::string &server_command() const noexcept;

    /**
     * @brief Get the local server working directory.
     *
     * @return Working directory.
     */
    [[nodiscard]] const std::string &server_working_directory() const noexcept;

    /**
     * @brief Check whether the configuration has an application name.
     *
     * @return True if name is not empty.
     */
    [[nodiscard]] bool has_name() const noexcept;

    /**
     * @brief Check whether the configuration has a title.
     *
     * @return True if title is not empty.
     */
    [[nodiscard]] bool has_title() const noexcept;

    /**
     * @brief Check whether the configuration has an application id.
     *
     * @return True if app id is not empty.
     */
    [[nodiscard]] bool has_app_id() const noexcept;

    /**
     * @brief Check whether the configuration has an app version.
     *
     * @return True if app version is not empty.
     */
    [[nodiscard]] bool has_app_version() const noexcept;

    /**
     * @brief Check whether the configuration has vendor metadata.
     *
     * @return True if vendor is not empty.
     */
    [[nodiscard]] bool has_vendor() const noexcept;

    /**
     * @brief Check whether the configuration has an icon path.
     *
     * @return True if icon path is not empty.
     */
    [[nodiscard]] bool has_icon_path() const noexcept;

    /**
     * @brief Check whether the configuration has a URL.
     *
     * @return True if URL is not empty.
     */
    [[nodiscard]] bool has_url() const noexcept;

    /**
     * @brief Check whether the configuration has a readiness URL.
     *
     * @return True if readiness URL is not empty.
     */
    [[nodiscard]] bool has_readiness_url() const noexcept;

    /**
     * @brief Check whether a local server command is configured.
     *
     * @return True if a server command is set.
     */
    [[nodiscard]] bool has_server_command() const noexcept;

    /**
     * @brief Check whether a local server working directory is configured.
     *
     * @return True if a server working directory is set.
     */
    [[nodiscard]] bool has_server_working_directory() const noexcept;

    /**
     * @brief Build a local HTTP URL from the host and port.
     *
     * Example:
     * `http://127.0.0.1:8080`
     *
     * @return Local URL.
     */
    [[nodiscard]] std::string local_url() const;

    /**
     * @brief Resolve the effective URL for the shell.
     *
     * If an explicit URL is set, it is returned.
     * Otherwise the local URL is returned.
     *
     * @return Effective shell URL.
     */
    [[nodiscard]] std::string effective_url() const;

    /**
     * @brief Resolve the effective readiness URL for the shell.
     *
     * If an explicit readiness URL is set, it is returned.
     * Otherwise the effective target URL is returned.
     *
     * @return Effective readiness URL.
     */
    [[nodiscard]] std::string effective_readiness_url() const;

  private:
    /**
     * @brief Application name.
     */
    std::string name_{"Vix UI App"};

    /**
     * @brief Window or page title.
     */
    std::string title_{"Vix UI App"};

    /**
     * @brief Stable desktop application id.
     */
    std::string app_id_;

    /**
     * @brief Application version metadata.
     */
    std::string app_version_;

    /**
     * @brief Application vendor metadata.
     */
    std::string vendor_;

    /**
     * @brief Optional desktop icon path.
     */
    std::string icon_path_;

    /**
     * @brief Explicit target URL.
     */
    std::string url_;

    /**
     * @brief Optional readiness URL.
     */
    std::string readiness_url_;

    /**
     * @brief Local host address.
     */
    std::string host_{"127.0.0.1"};

    /**
     * @brief Local HTTP port.
     */
    std::uint16_t port_{8080};

    /**
     * @brief Target platform.
     */
    Platform platform_{Platform::current()};

    /**
     * @brief Initial shell width.
     */
    int width_{1024};

    /**
     * @brief Initial shell height.
     */
    int height_{768};

    /**
     * @brief Whether the shell is resizable.
     */
    bool resizable_{true};

    /**
     * @brief Whether the shell starts fullscreen.
     */
    bool fullscreen_{false};

    /**
     * @brief Whether developer tools are enabled.
     */
    bool devtools_{false};

    /**
     * @brief Whether the shell should start the local server.
     */
    bool start_server_{true};

    /**
     * @brief Whether the shell should wait for local server readiness.
     */
    bool wait_for_server_{true};

    /**
     * @brief Maximum time to wait for local server startup.
     */
    std::chrono::milliseconds startup_timeout_{5000};

    /**
     * @brief Optional command used to start the local Vix server.
     */
    std::string server_command_;

    /**
     * @brief Optional working directory for the local server command.
     */
    std::string server_working_directory_;
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_SHELL_CONFIG_HPP
