/**
 *
 *  @file AppShell.cpp
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
#include <vix/ui/shell/AppShell.hpp>

#include <utility>

namespace vix::ui
{
  AppShell::AppShell(ShellConfig config)
      : config_(std::move(config))
  {
  }

  AppShell AppShell::make(ShellConfig config)
  {
    return AppShell(std::move(config));
  }

  AppShell &AppShell::set_config(ShellConfig config)
  {
    config_ = std::move(config);
    return *this;
  }

  const ShellConfig &AppShell::config() const noexcept
  {
    return config_;
  }

  ShellConfig &AppShell::config() noexcept
  {
    return config_;
  }

  Result<void> AppShell::validate() const
  {
    if (!config_.has_name())
    {
      return Result<void>::fail(
          ErrorCode::ShellError,
          "UI app shell requires an application name");
    }

    if (!config_.has_title())
    {
      return Result<void>::fail(
          ErrorCode::ShellError,
          "UI app shell requires a title");
    }

    if (config_.width() <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ShellError,
          "UI app shell width must be greater than zero");
    }

    if (config_.height() <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ShellError,
          "UI app shell height must be greater than zero");
    }

    if (config_.effective_url().empty())
    {
      return Result<void>::fail(
          ErrorCode::ShellError,
          "UI app shell requires a target URL");
    }

    return Result<void>::ok();
  }

  Result<void> AppShell::start()
  {
    Result<void> validation = validate();
    if (validation.is_failed())
    {
      return validation;
    }

    running_ = true;
    return Result<void>::ok();
  }

  Result<void> AppShell::stop()
  {
    running_ = false;
    return Result<void>::ok();
  }

  Result<void> AppShell::restart()
  {
    Result<void> stopped_result = stop();
    if (stopped_result.is_failed())
    {
      return stopped_result;
    }

    return start();
  }

  bool AppShell::running() const noexcept
  {
    return running_;
  }

  bool AppShell::stopped() const noexcept
  {
    return !running_;
  }

  std::string AppShell::target_url() const
  {
    return config_.effective_url();
  }

  const Platform &AppShell::platform() const noexcept
  {
    return config_.platform();
  }

  std::string AppShell::status() const
  {
    return running_ ? "running" : "stopped";
  }

} // namespace vix::ui
