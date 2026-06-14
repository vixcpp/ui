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

#include <cstdint>
#include <string>

#include <vix/ui/platform/Platform.hpp>

namespace vix::ui
{
  /**
   * @brief Configuration for a UI app shell.
   *
   * ShellConfig stores the minimal settings needed to describe how a
   * Vix UI application should be presented by an app shell.
   *
   * The first UI module version is web-first. Desktop and mobile shells
   * can later use this configuration to open a WebView, connect to a
   * local Vix server, and present the rendered application.
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
     * @param name Application name.
     * @return This configuration.
     */
    ShellConfig &set_name(std::string name);

    /**
     * @brief Set the shell application title.
     *
     * @param title Window or page title.
     * @return This configuration.
     */
    ShellConfig &set_title(std::string title);

    /**
     * @brief Set the target URL loaded by the shell.
     *
     * @param url Target URL.
     * @return This configuration.
     */
    ShellConfig &set_url(std::string url);

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
     * @brief Get the target URL.
     *
     * @return Target URL.
     */
    [[nodiscard]] const std::string &url() const noexcept;

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
     * @brief Check whether the configuration has a URL.
     *
     * @return True if URL is not empty.
     */
    [[nodiscard]] bool has_url() const noexcept;

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
     * @brief Explicit target URL.
     */
    std::string url_;

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
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_SHELL_CONFIG_HPP
