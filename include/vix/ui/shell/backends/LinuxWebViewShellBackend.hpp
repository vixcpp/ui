/**
 *
 *  @file LinuxWebViewShellBackend.hpp
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
#ifndef VIX_UI_SHELL_BACKENDS_LINUX_WEBVIEW_SHELL_BACKEND_HPP
#define VIX_UI_SHELL_BACKENDS_LINUX_WEBVIEW_SHELL_BACKEND_HPP

#include <memory>
#include <string>
#include <string_view>

#include <vix/ui/shell/backends/ShellBackend.hpp>

namespace vix::ui
{
  /**
   * @brief Linux desktop WebView shell backend.
   *
   * LinuxWebViewShellBackend is the native Linux implementation used by
   * AppShell when the UI module is built with Linux WebView support.
   *
   * The backend is responsible for:
   * - creating a native desktop window
   * - creating a WebView inside that window
   * - loading the effective shell URL
   * - applying window options from ShellConfig
   * - running the desktop event loop
   *
   * This backend is optional. It is only compiled with real WebView support
   * when the build system defines:
   *
   * @code
   * VIX_UI_ENABLE_LINUX_WEBVIEW
   * @endcode
   *
   * Without that build flag, the class still exists but start() returns a
   * clear failure result instead of trying to use unavailable native APIs.
   */
  class LinuxWebViewShellBackend final : public ShellBackend
  {
  public:
    /**
     * @brief Construct a stopped Linux WebView backend.
     */
    LinuxWebViewShellBackend() = default;

    /**
     * @brief Copy-construct a backend.
     *
     * Runtime native window handles are not copied. The copied backend is
     * created in a stopped state.
     *
     * @param other Backend to copy.
     */
    LinuxWebViewShellBackend(const LinuxWebViewShellBackend &other);

    /**
     * @brief Copy-assign a backend.
     *
     * Runtime native window handles are not copied. The assigned backend is
     * reset to a stopped state.
     *
     * @param other Backend to copy from.
     * @return This backend.
     */
    LinuxWebViewShellBackend &operator=(
        const LinuxWebViewShellBackend &other);

    /**
     * @brief Destroy the backend.
     */
    ~LinuxWebViewShellBackend() override;

    /**
     * @brief Clone this backend.
     *
     * @return Backend copy.
     */
    [[nodiscard]] std::unique_ptr<ShellBackend> clone() const override;

    /**
     * @brief Get the stable backend name.
     *
     * @return Backend name.
     */
    [[nodiscard]] std::string_view name() const noexcept override;

    /**
     * @brief Start the Linux WebView shell.
     *
     * When WebView support is enabled, this creates the native window,
     * loads the configured target URL, and enters the GTK event loop.
     *
     * @param config Shell configuration.
     * @return Successful result if the shell ran successfully.
     */
    Result<void> start(const ShellConfig &config) override;

    /**
     * @brief Stop the Linux WebView shell.
     *
     * @return Successful result if the shell stopped or was already stopped.
     */
    Result<void> stop() override;

    /**
     * @brief Check whether the backend is running.
     *
     * @return True if the backend is currently running.
     */
    [[nodiscard]] bool running() const noexcept override;

    /**
     * @brief Get a human-readable backend status.
     *
     * @return Backend status string.
     */
    [[nodiscard]] std::string status() const override;

  private:
    /**
     * @brief Reset native runtime handles.
     */
    void reset_runtime_state() noexcept;

  private:
    /**
     * @brief Whether the backend event loop is running.
     */
    bool running_{false};

    /**
     * @brief Native window handle.
     *
     * Stored as an opaque pointer so this internal header does not require
     * GTK/WebKit headers.
     */
    void *window_{nullptr};

    /**
     * @brief Native WebView handle.
     *
     * Stored as an opaque pointer so this internal header does not require
     * GTK/WebKit headers.
     */
    void *webview_{nullptr};
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_BACKENDS_LINUX_WEBVIEW_SHELL_BACKEND_HPP
