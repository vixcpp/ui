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
#include <vix/ui/shell/backends/DescriptorShellBackend.hpp>
#include <vix/ui/shell/backends/ShellBackend.hpp>
#include <vix/ui/shell/backends/LinuxWebViewShellBackend.hpp>
#include <vix/ui/shell/ServerProcess.hpp>
#include <vix/ui/shell/ServerReadiness.hpp>

#include <utility>

namespace vix::ui
{
  AppShell::AppShell()
      : backend_(make_backend())
  {
  }

  AppShell::AppShell(ShellConfig config)
      : config_(std::move(config)),
        backend_(make_backend())
  {
  }

  AppShell::~AppShell() = default;

  AppShell::AppShell(const AppShell &other)
      : config_(other.config_),
        server_process_(nullptr),
        backend_(other.backend_ ? other.backend_->clone() : make_backend())
  {
  }

  AppShell &AppShell::operator=(const AppShell &other)
  {
    if (this == &other)
    {
      return *this;
    }

    (void)stop_server_if_needed();

    config_ = other.config_;
    server_process_.reset();
    backend_ = other.backend_ ? other.backend_->clone() : make_backend();

    return *this;
  }

  AppShell::AppShell(AppShell &&other) noexcept = default;

  AppShell &AppShell::operator=(AppShell &&other) noexcept = default;

  AppShell AppShell::make(ShellConfig config)
  {
    return AppShell(std::move(config));
  }

  AppShell &AppShell::set_config(ShellConfig config)
  {
    config_ = std::move(config);

    if (stopped())
    {
      backend_ = make_backend();
    }

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
    if (running())
    {
      return Result<void>::ok();
    }

    Result<void> validation = validate();
    if (validation.is_failed())
    {
      return validation;
    }

    Result<void> server_result = start_server_if_needed();
    if (server_result.is_failed())
    {
      return server_result;
    }

    Result<void> readiness_result = wait_for_server_if_needed();
    if (readiness_result.is_failed())
    {
      (void)stop_server_if_needed();
      return readiness_result;
    }

    Result<void> shell_result = start_platform_shell();
    if (shell_result.is_failed())
    {
      (void)stop_server_if_needed();
      return shell_result;
    }

    /*
     * Native desktop backends may block until the window is closed.
     * If the backend is already stopped when start() returns, stop the
     * local server too.
     */
    if (stopped())
    {
      Result<void> stop_server_result = stop_server_if_needed();
      if (stop_server_result.is_failed())
      {
        return stop_server_result;
      }
    }

    return Result<void>::ok();
  }

  Result<void> AppShell::stop()
  {
    Result<void> shell_result = Result<void>::ok();

    if (running())
    {
      shell_result = stop_platform_shell();
    }

    Result<void> server_result = stop_server_if_needed();

    if (shell_result.is_failed())
    {
      return shell_result;
    }

    if (server_result.is_failed())
    {
      return server_result;
    }

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
    return backend_ != nullptr && backend_->running();
  }

  bool AppShell::stopped() const noexcept
  {
    return !running();
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
    if (!backend_)
    {
      return "stopped";
    }

    return backend_->status();
  }

  Result<void> AppShell::start_server_if_needed()
  {
    if (!config_.start_server())
    {
      return Result<void>::ok();
    }

    if (!config_.has_server_command())
    {
      return Result<void>::ok();
    }

    if (server_process_ && server_process_->running())
    {
      return Result<void>::ok();
    }

    server_process_ = std::make_unique<ServerProcess>(
        config_.server_command(),
        config_.server_working_directory());

    return server_process_->start();
  }

  Result<void> AppShell::stop_server_if_needed()
  {
    if (!server_process_)
    {
      return Result<void>::ok();
    }

    Result<void> result = server_process_->stop();

    if (result.is_ok())
    {
      server_process_.reset();
    }

    return result;
  }

  Result<void> AppShell::wait_for_server_if_needed()
  {
    if (!config_.start_server())
    {
      return Result<void>::ok();
    }

    if (!config_.wait_for_server())
    {
      return Result<void>::ok();
    }

    if (!config_.has_server_command())
    {
      return Result<void>::ok();
    }

    return ServerReadiness::wait(
        config_.effective_readiness_url(),
        config_.startup_timeout());
  }

  std::unique_ptr<ShellBackend> AppShell::make_backend() const
  {
#if defined(VIX_UI_ENABLE_LINUX_WEBVIEW)
    if (config_.platform().is_linux())
    {
      return std::make_unique<LinuxWebViewShellBackend>();
    }
#endif

    return std::make_unique<DescriptorShellBackend>();
  }

  Result<void> AppShell::start_platform_shell()
  {
    if (!backend_)
    {
      backend_ = make_backend();
    }

    return backend_->start(config_);
  }

  Result<void> AppShell::stop_platform_shell()
  {
    if (!backend_)
    {
      return Result<void>::ok();
    }

    return backend_->stop();
  }

} // namespace vix::ui
