/**
 *
 *  @file ShellConfig.cpp
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
#include <vix/ui/shell/ShellConfig.hpp>

#include <utility>

namespace vix::ui
{
  ShellConfig ShellConfig::make()
  {
    return ShellConfig{};
  }

  ShellConfig &ShellConfig::set_name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  ShellConfig &ShellConfig::set_title(std::string title)
  {
    title_ = std::move(title);
    return *this;
  }

  ShellConfig &ShellConfig::set_url(std::string url)
  {
    url_ = std::move(url);
    return *this;
  }

  ShellConfig &ShellConfig::set_host(std::string host)
  {
    host_ = std::move(host);
    return *this;
  }

  ShellConfig &ShellConfig::set_port(std::uint16_t port) noexcept
  {
    port_ = port;
    return *this;
  }

  ShellConfig &ShellConfig::set_platform(Platform platform) noexcept
  {
    platform_ = platform;
    return *this;
  }

  ShellConfig &ShellConfig::set_width(int width) noexcept
  {
    width_ = width;
    return *this;
  }

  ShellConfig &ShellConfig::set_height(int height) noexcept
  {
    height_ = height;
    return *this;
  }

  ShellConfig &ShellConfig::set_resizable(bool value) noexcept
  {
    resizable_ = value;
    return *this;
  }

  ShellConfig &ShellConfig::set_fullscreen(bool value) noexcept
  {
    fullscreen_ = value;
    return *this;
  }

  ShellConfig &ShellConfig::set_devtools(bool value) noexcept
  {
    devtools_ = value;
    return *this;
  }

  ShellConfig &ShellConfig::set_start_server(bool value) noexcept
  {
    start_server_ = value;
    return *this;
  }

  ShellConfig &ShellConfig::set_server_command(std::string command)
  {
    server_command_ = std::move(command);
    return *this;
  }

  ShellConfig &ShellConfig::set_server_working_directory(std::string directory)
  {
    server_working_directory_ = std::move(directory);
    return *this;
  }

  const std::string &ShellConfig::name() const noexcept
  {
    return name_;
  }

  const std::string &ShellConfig::title() const noexcept
  {
    return title_;
  }

  const std::string &ShellConfig::url() const noexcept
  {
    return url_;
  }

  const std::string &ShellConfig::host() const noexcept
  {
    return host_;
  }

  std::uint16_t ShellConfig::port() const noexcept
  {
    return port_;
  }

  const Platform &ShellConfig::platform() const noexcept
  {
    return platform_;
  }

  int ShellConfig::width() const noexcept
  {
    return width_;
  }

  int ShellConfig::height() const noexcept
  {
    return height_;
  }

  bool ShellConfig::resizable() const noexcept
  {
    return resizable_;
  }

  bool ShellConfig::fullscreen() const noexcept
  {
    return fullscreen_;
  }

  bool ShellConfig::devtools() const noexcept
  {
    return devtools_;
  }

  bool ShellConfig::start_server() const noexcept
  {
    return start_server_;
  }

  const std::string &ShellConfig::server_command() const noexcept
  {
    return server_command_;
  }

  const std::string &ShellConfig::server_working_directory() const noexcept
  {
    return server_working_directory_;
  }

  bool ShellConfig::has_name() const noexcept
  {
    return !name_.empty();
  }

  bool ShellConfig::has_title() const noexcept
  {
    return !title_.empty();
  }

  bool ShellConfig::has_url() const noexcept
  {
    return !url_.empty();
  }

  bool ShellConfig::has_server_command() const noexcept
  {
    return !server_command_.empty();
  }

  bool ShellConfig::has_server_working_directory() const noexcept
  {
    return !server_working_directory_.empty();
  }

  std::string ShellConfig::local_url() const
  {
    return "http://" + host_ + ":" + std::to_string(port_);
  }

  std::string ShellConfig::effective_url() const
  {
    if (!url_.empty())
    {
      return url_;
    }

    return local_url();
  }

} // namespace vix::ui
