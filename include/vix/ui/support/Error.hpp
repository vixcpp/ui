/**
 *
 *  @file Error.hpp
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
#ifndef VIX_UI_SUPPORT_ERROR_HPP
#define VIX_UI_SUPPORT_ERROR_HPP

#include <stdexcept>
#include <string>

namespace vix::ui
{
  /**
   * @brief Error categories for the UI module.
   *
   * ErrorCode provides stable categories for failures raised by the
   * UI foundation layer.
   */
  enum class ErrorCode
  {
    Unknown = 0,
    ViewError,
    HtmlError,
    AssetError,
    FormError,
    ShellError,
    PlatformError,
    ConfigError,
    RuntimeError
  };

  /**
   * @brief Base exception type for the UI module.
   *
   * Error carries a stable error code and a human-readable message.
   * It is used as the common parent for all UI-related failures.
   */
  class Error : public std::runtime_error
  {
  public:
    /**
     * @brief Construct a UI error.
     *
     * @param code Stable error code.
     * @param message Human-readable error message.
     */
    Error(ErrorCode code, std::string message);

    /**
     * @brief Get the error code.
     *
     * @return Stable error code.
     */
    [[nodiscard]] ErrorCode code() const noexcept;

  private:
    /**
     * @brief Stable error code associated with this error.
     */
    ErrorCode code_{ErrorCode::Unknown};
  };

  /**
   * @brief View-specific UI error.
   */
  class ViewError : public Error
  {
  public:
    /**
     * @brief Construct a view error.
     *
     * @param message Human-readable error message.
     */
    explicit ViewError(std::string message);
  };

  /**
   * @brief HTML-specific UI error.
   */
  class HtmlError : public Error
  {
  public:
    /**
     * @brief Construct an HTML error.
     *
     * @param message Human-readable error message.
     */
    explicit HtmlError(std::string message);
  };

  /**
   * @brief Asset-specific UI error.
   */
  class AssetError : public Error
  {
  public:
    /**
     * @brief Construct an asset error.
     *
     * @param message Human-readable error message.
     */
    explicit AssetError(std::string message);
  };

  /**
   * @brief Form-specific UI error.
   */
  class FormError : public Error
  {
  public:
    /**
     * @brief Construct a form error.
     *
     * @param message Human-readable error message.
     */
    explicit FormError(std::string message);
  };

  /**
   * @brief App shell-specific UI error.
   */
  class ShellError : public Error
  {
  public:
    /**
     * @brief Construct an app shell error.
     *
     * @param message Human-readable error message.
     */
    explicit ShellError(std::string message);
  };

  /**
   * @brief Platform-specific UI error.
   */
  class PlatformError : public Error
  {
  public:
    /**
     * @brief Construct a platform error.
     *
     * @param message Human-readable error message.
     */
    explicit PlatformError(std::string message);
  };

  /**
   * @brief Configuration-specific UI error.
   */
  class ConfigError : public Error
  {
  public:
    /**
     * @brief Construct a configuration error.
     *
     * @param message Human-readable error message.
     */
    explicit ConfigError(std::string message);
  };

  /**
   * @brief Runtime-specific UI error.
   */
  class RuntimeError : public Error
  {
  public:
    /**
     * @brief Construct a runtime error.
     *
     * @param message Human-readable error message.
     */
    explicit RuntimeError(std::string message);
  };

} // namespace vix::ui

#endif // VIX_UI_SUPPORT_ERROR_HPP
