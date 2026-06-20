/**
 *
 *  @file ShellBackend.hpp
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
#ifndef VIX_UI_SHELL_BACKENDS_SHELL_BACKEND_HPP
#define VIX_UI_SHELL_BACKENDS_SHELL_BACKEND_HPP

#include <memory>
#include <string>
#include <string_view>

#include <vix/ui/shell/ShellConfig.hpp>
#include <vix/ui/support/Result.hpp>

namespace vix::ui
{
  /**
   * @brief Internal interface for app shell backends.
   *
   * ShellBackend is not the public API used by applications. It is the
   * internal backend contract used by AppShell to support multiple shell
   * implementations without changing the public facade.
   */
  class ShellBackend
  {
  public:
    virtual ~ShellBackend() = default;

    /**
     * @brief Clone this backend.
     *
     * @return Backend copy.
     */
    [[nodiscard]] virtual std::unique_ptr<ShellBackend> clone() const = 0;

    /**
     * @brief Get the stable backend name.
     *
     * @return Backend name.
     */
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;

    /**
     * @brief Start the backend.
     *
     * @param config Shell configuration.
     * @return Successful result if started.
     */
    virtual Result<void> start(const ShellConfig &config) = 0;

    /**
     * @brief Stop the backend.
     *
     * @return Successful result if stopped.
     */
    virtual Result<void> stop() = 0;

    /**
     * @brief Check whether the backend is running.
     *
     * @return True if running.
     */
    [[nodiscard]] virtual bool running() const noexcept = 0;

    /**
     * @brief Get a human-readable backend status.
     *
     * @return Backend status.
     */
    [[nodiscard]] virtual std::string status() const = 0;
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_BACKENDS_SHELL_BACKEND_HPP
