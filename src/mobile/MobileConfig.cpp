/**
 *
 *  @file MobileConfig.cpp
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
#include <vix/ui/mobile/MobileConfig.hpp>

#include <utility>

namespace vix::ui
{
  MobileConfig MobileConfig::make()
  {
    return MobileConfig{};
  }

  MobileConfig &MobileConfig::set_name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  MobileConfig &MobileConfig::set_app_id(std::string app_id)
  {
    app_id_ = std::move(app_id);
    return *this;
  }

  MobileConfig &MobileConfig::set_version(std::string version)
  {
    version_ = std::move(version);
    return *this;
  }

  MobileConfig &MobileConfig::set_url(std::string url)
  {
    url_ = std::move(url);
    return *this;
  }

  MobileConfig &MobileConfig::set_icon_path(std::string path)
  {
    icon_path_ = std::move(path);
    return *this;
  }

  const std::string &MobileConfig::name() const noexcept
  {
    return name_;
  }

  const std::string &MobileConfig::app_id() const noexcept
  {
    return app_id_;
  }

  const std::string &MobileConfig::version() const noexcept
  {
    return version_;
  }

  const std::string &MobileConfig::url() const noexcept
  {
    return url_;
  }

  const std::string &MobileConfig::icon_path() const noexcept
  {
    return icon_path_;
  }

  bool MobileConfig::has_name() const noexcept
  {
    return !name_.empty();
  }

  bool MobileConfig::has_app_id() const noexcept
  {
    return !app_id_.empty();
  }

  bool MobileConfig::has_version() const noexcept
  {
    return !version_.empty();
  }

  bool MobileConfig::has_url() const noexcept
  {
    return !url_.empty();
  }

  bool MobileConfig::has_icon_path() const noexcept
  {
    return !icon_path_.empty();
  }

  Result<void> MobileConfig::validate() const
  {
    if (!has_name())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "mobile application name must not be empty");
    }

    if (!has_app_id())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "mobile application id must not be empty");
    }

    if (!has_version())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "mobile application version must not be empty");
    }

    if (!has_url())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "mobile application URL must not be empty");
    }

    return Result<void>::ok();
  }

} // namespace vix::ui
