/**
 *
 *  @file Error.cpp
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
#include <vix/ui/support/Error.hpp>

#include <utility>

namespace vix::ui
{
  Error::Error(ErrorCode code, std::string message)
      : std::runtime_error(std::move(message)),
        code_(code)
  {
  }

  ErrorCode Error::code() const noexcept
  {
    return code_;
  }

  ViewError::ViewError(std::string message)
      : Error(ErrorCode::ViewError, std::move(message))
  {
  }

  HtmlError::HtmlError(std::string message)
      : Error(ErrorCode::HtmlError, std::move(message))
  {
  }

  AssetError::AssetError(std::string message)
      : Error(ErrorCode::AssetError, std::move(message))
  {
  }

  FormError::FormError(std::string message)
      : Error(ErrorCode::FormError, std::move(message))
  {
  }

  ShellError::ShellError(std::string message)
      : Error(ErrorCode::ShellError, std::move(message))
  {
  }

  PlatformError::PlatformError(std::string message)
      : Error(ErrorCode::PlatformError, std::move(message))
  {
  }

  ConfigError::ConfigError(std::string message)
      : Error(ErrorCode::ConfigError, std::move(message))
  {
  }

  RuntimeError::RuntimeError(std::string message)
      : Error(ErrorCode::RuntimeError, std::move(message))
  {
  }

} // namespace vix::ui
