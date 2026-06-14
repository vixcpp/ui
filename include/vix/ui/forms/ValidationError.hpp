/**
 *
 *  @file ValidationError.hpp
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
#ifndef VIX_UI_FORMS_VALIDATION_ERROR_HPP
#define VIX_UI_FORMS_VALIDATION_ERROR_HPP

#include <string>
#include <string_view>

namespace vix::ui
{
  /**
   * @brief Validation error category for UI forms.
   *
   * ValidationErrorCode provides stable categories for common form
   * validation failures.
   */
  enum class ValidationErrorCode
  {
    Unknown = 0,
    Required,
    Invalid,
    TooShort,
    TooLong,
    TooSmall,
    TooLarge,
    PatternMismatch,
    Custom
  };

  /**
   * @brief Convert a validation error code to a stable string.
   *
   * @param code Validation error code.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(
      ValidationErrorCode code) noexcept;

  /**
   * @brief Single validation error for a form field.
   *
   * ValidationError stores:
   * - the field name
   * - a stable validation error code
   * - a human-readable message
   *
   * It is used by Field and Form to expose validation failures without
   * requiring exceptions for normal user input errors.
   */
  class ValidationError
  {
  public:
    /**
     * @brief Construct an empty validation error.
     */
    ValidationError() = default;

    /**
     * @brief Construct a validation error.
     *
     * @param field Field name.
     * @param code Stable validation error code.
     * @param message Human-readable error message.
     */
    ValidationError(
        std::string field,
        ValidationErrorCode code,
        std::string message);

    /**
     * @brief Create a required-field validation error.
     *
     * @param field Field name.
     * @param message Human-readable error message.
     * @return Validation error.
     */
    [[nodiscard]] static ValidationError required(
        std::string field,
        std::string message = "This field is required.");

    /**
     * @brief Create an invalid-field validation error.
     *
     * @param field Field name.
     * @param message Human-readable error message.
     * @return Validation error.
     */
    [[nodiscard]] static ValidationError invalid(
        std::string field,
        std::string message = "This field is invalid.");

    /**
     * @brief Create a custom validation error.
     *
     * @param field Field name.
     * @param message Human-readable error message.
     * @return Validation error.
     */
    [[nodiscard]] static ValidationError custom(
        std::string field,
        std::string message);

    /**
     * @brief Set the field name.
     *
     * @param field Field name.
     * @return This validation error.
     */
    ValidationError &set_field(std::string field);

    /**
     * @brief Set the validation error code.
     *
     * @param code Stable validation error code.
     * @return This validation error.
     */
    ValidationError &set_code(ValidationErrorCode code) noexcept;

    /**
     * @brief Set the human-readable message.
     *
     * @param message Human-readable error message.
     * @return This validation error.
     */
    ValidationError &set_message(std::string message);

    /**
     * @brief Get the field name.
     *
     * @return Field name.
     */
    [[nodiscard]] const std::string &field() const noexcept;

    /**
     * @brief Get the validation error code.
     *
     * @return Validation error code.
     */
    [[nodiscard]] ValidationErrorCode code() const noexcept;

    /**
     * @brief Get the human-readable error message.
     *
     * @return Error message.
     */
    [[nodiscard]] const std::string &message() const noexcept;

    /**
     * @brief Check whether the error has a field name.
     *
     * @return True if field name is not empty.
     */
    [[nodiscard]] bool has_field() const noexcept;

    /**
     * @brief Check whether the error has a message.
     *
     * @return True if message is not empty.
     */
    [[nodiscard]] bool has_message() const noexcept;

    /**
     * @brief Check whether this validation error is empty.
     *
     * @return True if no field and no message are set.
     */
    [[nodiscard]] bool empty() const noexcept;

  private:
    /**
     * @brief Field name associated with the validation error.
     */
    std::string field_;

    /**
     * @brief Stable validation error code.
     */
    ValidationErrorCode code_{ValidationErrorCode::Unknown};

    /**
     * @brief Human-readable validation error message.
     */
    std::string message_;
  };

} // namespace vix::ui

#endif // VIX_UI_FORMS_VALIDATION_ERROR_HPP
