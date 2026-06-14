/**
 *
 *  @file Result.hpp
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
#ifndef VIX_UI_SUPPORT_RESULT_HPP
#define VIX_UI_SUPPORT_RESULT_HPP

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <vix/ui/support/Error.hpp>

namespace vix::ui
{
  /**
   * @brief Result state for UI operations.
   */
  enum class ResultStatus
  {
    Ok,
    Failed
  };

  /**
   * @brief Convert a result status to a stable string.
   *
   * @param status Result status.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(ResultStatus status) noexcept;

  /**
   * @brief Generic result type for UI operations.
   *
   * Result stores either:
   * - a successful value
   * - an error code and message
   *
   * It is intended for non-exceptional UI workflows where callers want
   * explicit success/failure handling.
   *
   * @tparam T Successful value type.
   */
  template <typename T>
  class Result
  {
  public:
    /**
     * @brief Construct a successful result.
     *
     * @param value Successful value.
     * @return Successful result.
     */
    [[nodiscard]] static Result ok(T value)
    {
      Result result;
      result.status_ = ResultStatus::Ok;
      result.value_ = std::move(value);
      return result;
    }

    /**
     * @brief Construct a failed result.
     *
     * @param code Stable error code.
     * @param message Human-readable error message.
     * @return Failed result.
     */
    [[nodiscard]] static Result fail(
        ErrorCode code,
        std::string message)
    {
      Result result;
      result.status_ = ResultStatus::Failed;
      result.error_code_ = code;
      result.error_message_ = std::move(message);
      return result;
    }

    /**
     * @brief Check whether the result is successful.
     *
     * @return True if successful.
     */
    [[nodiscard]] bool is_ok() const noexcept
    {
      return status_ == ResultStatus::Ok;
    }

    /**
     * @brief Check whether the result is failed.
     *
     * @return True if failed.
     */
    [[nodiscard]] bool is_failed() const noexcept
    {
      return status_ == ResultStatus::Failed;
    }

    /**
     * @brief Convert the result to bool.
     *
     * @return True if successful.
     */
    [[nodiscard]] explicit operator bool() const noexcept
    {
      return is_ok();
    }

    /**
     * @brief Get the result status.
     *
     * @return Result status.
     */
    [[nodiscard]] ResultStatus status() const noexcept
    {
      return status_;
    }

    /**
     * @brief Get the successful value.
     *
     * @return Immutable successful value.
     *
     * @throws RuntimeError if the result is failed.
     */
    [[nodiscard]] const T &value() const
    {
      if (!value_.has_value())
      {
        throw RuntimeError("attempted to access value from failed UI result");
      }

      return *value_;
    }

    /**
     * @brief Get the successful value.
     *
     * @return Mutable successful value.
     *
     * @throws RuntimeError if the result is failed.
     */
    [[nodiscard]] T &value()
    {
      if (!value_.has_value())
      {
        throw RuntimeError("attempted to access value from failed UI result");
      }

      return *value_;
    }

    /**
     * @brief Move the successful value out of the result.
     *
     * @return Moved successful value.
     *
     * @throws RuntimeError if the result is failed.
     */
    [[nodiscard]] T move_value()
    {
      if (!value_.has_value())
      {
        throw RuntimeError("attempted to move value from failed UI result");
      }

      return std::move(*value_);
    }

    /**
     * @brief Get the error code.
     *
     * @return Stable error code.
     */
    [[nodiscard]] ErrorCode error_code() const noexcept
    {
      return error_code_;
    }

    /**
     * @brief Get the error message.
     *
     * @return Human-readable error message.
     */
    [[nodiscard]] const std::string &error_message() const noexcept
    {
      return error_message_;
    }

  private:
    /**
     * @brief Result status.
     */
    ResultStatus status_{ResultStatus::Failed};

    /**
     * @brief Successful value.
     */
    std::optional<T> value_{};

    /**
     * @brief Error code for failed results.
     */
    ErrorCode error_code_{ErrorCode::Unknown};

    /**
     * @brief Human-readable error message for failed results.
     */
    std::string error_message_{};
  };

  /**
   * @brief Void result type for UI operations without return values.
   *
   * Result<void> stores only success/failure state and optional error
   * information.
   */
  template <>
  class Result<void>
  {
  public:
    /**
     * @brief Construct a successful void result.
     *
     * @return Successful result.
     */
    [[nodiscard]] static Result ok()
    {
      Result result;
      result.status_ = ResultStatus::Ok;
      return result;
    }

    /**
     * @brief Construct a failed void result.
     *
     * @param code Stable error code.
     * @param message Human-readable error message.
     * @return Failed result.
     */
    [[nodiscard]] static Result fail(
        ErrorCode code,
        std::string message)
    {
      Result result;
      result.status_ = ResultStatus::Failed;
      result.error_code_ = code;
      result.error_message_ = std::move(message);
      return result;
    }

    /**
     * @brief Check whether the result is successful.
     *
     * @return True if successful.
     */
    [[nodiscard]] bool is_ok() const noexcept
    {
      return status_ == ResultStatus::Ok;
    }

    /**
     * @brief Check whether the result is failed.
     *
     * @return True if failed.
     */
    [[nodiscard]] bool is_failed() const noexcept
    {
      return status_ == ResultStatus::Failed;
    }

    /**
     * @brief Convert the result to bool.
     *
     * @return True if successful.
     */
    [[nodiscard]] explicit operator bool() const noexcept
    {
      return is_ok();
    }

    /**
     * @brief Get the result status.
     *
     * @return Result status.
     */
    [[nodiscard]] ResultStatus status() const noexcept
    {
      return status_;
    }

    /**
     * @brief Get the error code.
     *
     * @return Stable error code.
     */
    [[nodiscard]] ErrorCode error_code() const noexcept
    {
      return error_code_;
    }

    /**
     * @brief Get the error message.
     *
     * @return Human-readable error message.
     */
    [[nodiscard]] const std::string &error_message() const noexcept
    {
      return error_message_;
    }

  private:
    /**
     * @brief Result status.
     */
    ResultStatus status_{ResultStatus::Failed};

    /**
     * @brief Error code for failed results.
     */
    ErrorCode error_code_{ErrorCode::Unknown};

    /**
     * @brief Human-readable error message for failed results.
     */
    std::string error_message_{};
  };

} // namespace vix::ui

#endif // VIX_UI_SUPPORT_RESULT_HPP
