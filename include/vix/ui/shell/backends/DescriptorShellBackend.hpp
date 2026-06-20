/**
 *
 *  @file DescriptorShellBackend.hpp
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
#ifndef VIX_UI_SHELL_BACKENDS_DESCRIPTOR_SHELL_BACKEND_HPP
#define VIX_UI_SHELL_BACKENDS_DESCRIPTOR_SHELL_BACKEND_HPP

#include <memory>
#include <string>
#include <string_view>

#include <vix/ui/shell/backends/ShellBackend.hpp>

namespace vix::ui
{
  /**
   * @brief Descriptor-only shell backend.
   *
   * This backend keeps the current AppShell behavior while native desktop
   * WebView backends are added incrementally.
   *
   * It validates that the shell has an effective URL and tracks lifecycle
   * state, but it does not create a native window.
   */
  class DescriptorShellBackend final : public ShellBackend
  {
  public:
    DescriptorShellBackend() = default;

    [[nodiscard]] std::unique_ptr<ShellBackend> clone() const override;

    [[nodiscard]] std::string_view name() const noexcept override;

    Result<void> start(const ShellConfig &config) override;

    Result<void> stop() override;

    [[nodiscard]] bool running() const noexcept override;

    [[nodiscard]] std::string status() const override;

  private:
    bool running_{false};
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_BACKENDS_DESCRIPTOR_SHELL_BACKEND_HPP
