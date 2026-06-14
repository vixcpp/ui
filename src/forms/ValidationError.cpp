/**
 *
 *  @file ValidationError.cpp
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
#include <vix/ui/forms/ValidationError.hpp>

#include <utility>

namespace vix::ui
{
  std::string_view to_string(ValidationErrorCode code) noexcept
  {
    switch (code)
    {
    case ValidationErrorCode::Unknown:
      return "unknown";

    case ValidationErrorCode::Required:
      return "required";

    case ValidationErrorCode::Invalid:
      return "invalid";

    case ValidationErrorCode::TooShort:
      return "too_short";

    case ValidationErrorCode::TooLong:
      return "too_long";

    case ValidationErrorCode::TooSmall:
      return "too_small";

    case ValidationErrorCode::TooLarge:
      return "too_large";

    case ValidationErrorCode::PatternMismatch:
      return "pattern_mismatch";

    case ValidationErrorCode::Custom:
      return "custom";
    }

    return "unknown";
  }

  ValidationError::ValidationError(
      std::string field,
      ValidationErrorCode code,
      std::string message)
      : field_(std::move(field)),
        code_(code),
        message_(std::move(message))
  {
  }

  ValidationError ValidationError::required(
      std::string field,
      std::string message)
  {
    return ValidationError(
        std::move(field),
        ValidationErrorCode::Required,
        std::move(message));
  }

  ValidationError ValidationError::invalid(
      std::string field,
      std::string message)
  {
    return ValidationError(
        std::move(field),
        ValidationErrorCode::Invalid,
        std::move(message));
  }

  ValidationError ValidationError::custom(
      std::string field,
      std::string message)
  {
    return ValidationError(
        std::move(field),
        ValidationErrorCode::Custom,
        std::move(message));
  }

  ValidationError &ValidationError::set_field(std::string field)
  {
    field_ = std::move(field);
    return *this;
  }

  ValidationError &ValidationError::set_code(
      ValidationErrorCode code) noexcept
  {
    code_ = code;
    return *this;
  }

  ValidationError &ValidationError::set_message(std::string message)
  {
    message_ = std::move(message);
    return *this;
  }

  const std::string &ValidationError::field() const noexcept
  {
    return field_;
  }

  ValidationErrorCode ValidationError::code() const noexcept
  {
    return code_;
  }

  const std::string &ValidationError::message() const noexcept
  {
    return message_;
  }

  bool ValidationError::has_field() const noexcept
  {
    return !field_.empty();
  }

  bool ValidationError::has_message() const noexcept
  {
    return !message_.empty();
  }

  bool ValidationError::empty() const noexcept
  {
    return field_.empty() && message_.empty();
  }

} // namespace vix::ui
