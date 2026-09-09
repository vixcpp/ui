/**
 *
 *  @file AndroidProject.cpp
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
#include <vix/ui/mobile/AndroidProject.hpp>

#include <utility>

namespace vix::ui
{
  AndroidProject::AndroidProject(MobileProject project)
      : MobileProject(std::move(project))
  {
  }

  AndroidProject &AndroidProject::set_min_sdk(int min_sdk) noexcept
  {
    min_sdk_ = min_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_target_sdk(int target_sdk) noexcept
  {
    target_sdk_ = target_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_compile_sdk(int compile_sdk) noexcept
  {
    compile_sdk_ = compile_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_version_code(int version_code) noexcept
  {
    version_code_ = version_code;
    return *this;
  }

  AndroidProject &AndroidProject::set_android_gradle_plugin_version(
      std::string version)
  {
    android_gradle_plugin_version_ = std::move(version);
    return *this;
  }

  int AndroidProject::min_sdk() const noexcept
  {
    return min_sdk_;
  }

  int AndroidProject::target_sdk() const noexcept
  {
    return target_sdk_;
  }

  int AndroidProject::compile_sdk() const noexcept
  {
    return compile_sdk_;
  }

  int AndroidProject::version_code() const noexcept
  {
    return version_code_;
  }

  const std::string &AndroidProject::android_gradle_plugin_version() const noexcept
  {
    return android_gradle_plugin_version_;
  }

  Result<void> AndroidProject::validate() const
  {
    Result<void> mobile_validation = MobileProject::validate();
    if (mobile_validation.is_failed())
    {
      return mobile_validation;
    }

    if (min_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project min SDK must be greater than zero");
    }

    if (target_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project target SDK must be greater than zero");
    }

    if (compile_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project compile SDK must be greater than zero");
    }

    if (version_code_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project version code must be greater than zero");
    }

    if (min_sdk_ > target_sdk_)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project min SDK must not exceed target SDK");
    }

    if (target_sdk_ > compile_sdk_)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project target SDK must not exceed compile SDK");
    }

    if (android_gradle_plugin_version_.empty())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project Android Gradle Plugin version must not be empty");
    }

    return Result<void>::ok();
  }

} // namespace vix::ui
