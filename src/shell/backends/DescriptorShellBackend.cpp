/**
 *
 *  @file DescriptorShellBackend.cpp
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
#include <vix/ui/shell/backends/DescriptorShellBackend.hpp>

namespace vix::ui
{
  std::unique_ptr<ShellBackend> DescriptorShellBackend::clone() const
  {
    return std::make_unique<DescriptorShellBackend>(*this);
  }

  std::string_view DescriptorShellBackend::name() const noexcept
  {
    return "descriptor";
  }

  Result<void> DescriptorShellBackend::start(const ShellConfig &config)
  {
    if (config.effective_url().empty())
    {
      return Result<void>::fail(
          ErrorCode::ShellError,
          "descriptor shell backend requires a target URL");
    }

    running_ = true;
    return Result<void>::ok();
  }

  Result<void> DescriptorShellBackend::stop()
  {
    running_ = false;
    return Result<void>::ok();
  }

  bool DescriptorShellBackend::running() const noexcept
  {
    return running_;
  }

  std::string DescriptorShellBackend::status() const
  {
    return running_ ? "running" : "stopped";
  }

} // namespace vix::ui
