/**
 *
 *  @file ServerProcess.cpp
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
#include <vix/ui/shell/ServerProcess.hpp>

#include <utility>
#include <thread>
#include <chrono>

#if defined(__unix__) || defined(__APPLE__)
#include <cerrno>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace vix::ui
{
  ServerProcess::ServerProcess(std::string command)
      : command_(std::move(command))
  {
  }

  ServerProcess::ServerProcess(
      std::string command,
      std::string working_directory)
      : command_(std::move(command)),
        working_directory_(std::move(working_directory))
  {
  }

  ServerProcess::~ServerProcess()
  {
    (void)stop();
  }

  ServerProcess::ServerProcess(ServerProcess &&other) noexcept
      : command_(std::move(other.command_)),
        working_directory_(std::move(other.working_directory_)),
        pid_(other.pid_),
        running_(other.running_)
  {
    other.reset();
  }

  ServerProcess &ServerProcess::operator=(ServerProcess &&other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }

    (void)stop();

    command_ = std::move(other.command_);
    working_directory_ = std::move(other.working_directory_);
    pid_ = other.pid_;
    running_ = other.running_;

    other.reset();

    return *this;
  }

  ServerProcess &ServerProcess::set_command(std::string command)
  {
    command_ = std::move(command);
    return *this;
  }

  ServerProcess &ServerProcess::set_working_directory(
      std::string working_directory)
  {
    working_directory_ = std::move(working_directory);
    return *this;
  }

  const std::string &ServerProcess::command() const noexcept
  {
    return command_;
  }

  const std::string &ServerProcess::working_directory() const noexcept
  {
    return working_directory_;
  }

  Result<void> ServerProcess::start()
  {
    if (running_)
    {
      return Result<void>::ok();
    }

    if (command_.empty())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "server process requires a command");
    }

#if defined(__unix__) || defined(__APPLE__)
    const pid_t child = fork();

    if (child < 0)
    {
      return Result<void>::fail(
          ErrorCode::RuntimeError,
          std::string("failed to fork server process: ") + std::strerror(errno));
    }

    if (child == 0)
    {
      (void)setpgid(0, 0);
      const int null_fd = open("/dev/null", O_RDONLY);
      if (null_fd >= 0)
      {
        (void)dup2(null_fd, STDIN_FILENO);
        close(null_fd);
      }

      if (!working_directory_.empty())
      {
        if (chdir(working_directory_.c_str()) != 0)
        {
          _exit(127);
        }
      }

      execl("/bin/sh", "sh", "-c", command_.c_str(), static_cast<char *>(nullptr));

      _exit(127);
    }

    pid_ = static_cast<std::int64_t>(child);

    if (setpgid(child, child) != 0 && errno != EACCES && errno != ESRCH)
    {
      return Result<void>::fail(
          ErrorCode::RuntimeError,
          std::string("failed to create server process group: ") + std::strerror(errno));
    }

    running_ = true;

    return Result<void>::ok();
#else
    return Result<void>::fail(
        ErrorCode::RuntimeError,
        "server process launcher is not supported on this platform yet");
#endif
  }

  Result<void> ServerProcess::stop()
  {
    if (!running_)
    {
      reset();
      return Result<void>::ok();
    }

#if defined(__unix__) || defined(__APPLE__)
    const pid_t process_id = static_cast<pid_t>(pid_);

    if (process_id > 0)
    {
      auto terminate_group = [&](int signal) -> bool
      {
        if (kill(-process_id, signal) == 0)
        {
          return true;
        }

        if (errno == ESRCH)
        {
          return true;
        }

        if (kill(process_id, signal) == 0)
        {
          return true;
        }

        return errno == ESRCH;
      };

      if (!terminate_group(SIGTERM))
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            std::string("failed to stop server process: ") + std::strerror(errno));
      }

      int status = 0;
      bool exited = false;

      for (int i = 0; i < 30; ++i)
      {
        const pid_t waited =
            waitpid(process_id, &status, WNOHANG);

        if (waited == process_id || waited == -1)
        {
          exited = true;
          break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      if (!exited)
      {
        (void)terminate_group(SIGKILL);

        for (int i = 0; i < 20; ++i)
        {
          const pid_t waited =
              waitpid(process_id, &status, WNOHANG);

          if (waited == process_id || waited == -1)
          {
            exited = true;
            break;
          }

          std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
      }
    }

    reset();
    return Result<void>::ok();
#else
    reset();
    return Result<void>::ok();
#endif
  }

  bool ServerProcess::running() const noexcept
  {
    return running_;
  }

  bool ServerProcess::stopped() const noexcept
  {
    return !running_;
  }

  bool ServerProcess::has_command() const noexcept
  {
    return !command_.empty();
  }

  bool ServerProcess::has_working_directory() const noexcept
  {
    return !working_directory_.empty();
  }

  std::int64_t ServerProcess::pid() const noexcept
  {
    return pid_;
  }

  void ServerProcess::reset() noexcept
  {
    pid_ = -1;
    running_ = false;
  }

} // namespace vix::ui
