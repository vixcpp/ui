/**
 *
 *  @file ServerProcess.hpp
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
#ifndef VIX_UI_SHELL_SERVER_PROCESS_HPP
#define VIX_UI_SHELL_SERVER_PROCESS_HPP

#include <cstdint>
#include <string>

#include <vix/ui/support/Result.hpp>

namespace vix::ui
{
  /**
   * @brief Local server process launcher used by desktop shells.
   *
   * ServerProcess starts an external command before the shell opens the
   * WebView URL.
   *
   * It is intended for the desktop shell MVP:
   *
   * @code
   * Vix app
   *   -> local HTTP server
   *   -> http://127.0.0.1:<port>
   *   -> desktop WebView window
   * @endcode
   *
   * The command is provided by ShellConfig::server_command().
   *
   * This class is move-only because it owns a running process handle.
   */
  class ServerProcess
  {
  public:
    /**
     * @brief Construct an empty stopped server process.
     */
    ServerProcess() = default;

    /**
     * @brief Construct a server process from a command.
     *
     * @param command Command used to start the server.
     */
    explicit ServerProcess(std::string command);

    /**
     * @brief Construct a server process from a command and working directory.
     *
     * @param command Command used to start the server.
     * @param working_directory Working directory used by the command.
     */
    ServerProcess(std::string command, std::string working_directory);

    /**
     * @brief Destroy the server process.
     *
     * If the process is still running, stop() is called.
     */
    ~ServerProcess();

    ServerProcess(const ServerProcess &) = delete;

    ServerProcess &operator=(const ServerProcess &) = delete;

    /**
     * @brief Move-construct a server process.
     *
     * @param other Process to move from.
     */
    ServerProcess(ServerProcess &&other) noexcept;

    /**
     * @brief Move-assign a server process.
     *
     * If this process is running, it is stopped before taking ownership of
     * the other process.
     *
     * @param other Process to move from.
     * @return This process.
     */
    ServerProcess &operator=(ServerProcess &&other) noexcept;

    /**
     * @brief Set the command used to start the server.
     *
     * @param command Server command.
     * @return This process.
     */
    ServerProcess &set_command(std::string command);

    /**
     * @brief Set the working directory used by the server command.
     *
     * If empty, the current process working directory is used.
     *
     * @param working_directory Working directory.
     * @return This process.
     */
    ServerProcess &set_working_directory(std::string working_directory);

    /**
     * @brief Get the configured server command.
     *
     * @return Server command.
     */
    [[nodiscard]] const std::string &command() const noexcept;

    /**
     * @brief Get the configured working directory.
     *
     * @return Working directory.
     */
    [[nodiscard]] const std::string &working_directory() const noexcept;

    /**
     * @brief Start the server process.
     *
     * On Linux and POSIX systems, this forks and executes the command with:
     *
     * @code
     * /bin/sh -c "<command>"
     * @endcode
     *
     * @return Successful result if the process was started.
     */
    Result<void> start();

    /**
     * @brief Stop the server process.
     *
     * @return Successful result if the process stopped or was already stopped.
     */
    Result<void> stop();

    /**
     * @brief Check whether the process is running.
     *
     * @return True if the process is considered running.
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * @brief Check whether the process is stopped.
     *
     * @return True if the process is stopped.
     */
    [[nodiscard]] bool stopped() const noexcept;

    /**
     * @brief Check whether a command is configured.
     *
     * @return True if the command is not empty.
     */
    [[nodiscard]] bool has_command() const noexcept;

    /**
     * @brief Check whether a working directory is configured.
     *
     * @return True if the working directory is not empty.
     */
    [[nodiscard]] bool has_working_directory() const noexcept;

    /**
     * @brief Get the native process id.
     *
     * @return Process id, or -1 when no process is owned.
     */
    [[nodiscard]] std::int64_t pid() const noexcept;

  private:
    /**
     * @brief Reset this object to a stopped state.
     */
    void reset() noexcept;

  private:
    /**
     * @brief Server command.
     */
    std::string command_;

    /**
     * @brief Optional working directory.
     */
    std::string working_directory_;

    /**
     * @brief Native process id.
     */
    std::int64_t pid_{-1};

    /**
     * @brief Whether the process is considered running.
     */
    bool running_{false};
  };

} // namespace vix::ui

#endif // VIX_UI_SHELL_SERVER_PROCESS_HPP
