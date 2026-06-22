/**
 *
 *  @file Toast.hpp
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
#ifndef VIX_UI_LIVE_TOAST_HPP
#define VIX_UI_LIVE_TOAST_HPP

#include <chrono>
#include <string>
#include <string_view>

#include <vix/ui/html/HtmlAttrs.hpp>
#include <vix/ui/live/FlashMessage.hpp>

namespace vix::ui
{
  /**
   * @brief Supported toast positions.
   */
  enum class ToastPosition
  {
    TopRight = 0,
    TopLeft,
    BottomRight,
    BottomLeft,
    TopCenter,
    BottomCenter
  };

  /**
   * @brief Convert a toast position to a stable string.
   *
   * @param position Toast position.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(
      ToastPosition position) noexcept;

  /**
   * @brief Server-rendered toast notification helper.
   *
   * Toast represents a short notification that can be rendered by the
   * server and inserted into a dashboard, admin panel, internal tool, or
   * live update response.
   *
   * It is transport-neutral and does not require JavaScript by itself.
   * Client-side code may use the generated data attributes to animate,
   * dismiss, or auto-remove the toast.
   */
  class Toast
  {
  public:
    /**
     * @brief Construct an empty info toast.
     */
    Toast() = default;

    /**
     * @brief Construct a toast.
     *
     * @param level Toast level.
     * @param message Toast message.
     */
    Toast(FlashLevel level, std::string message);

    /**
     * @brief Create an info toast.
     *
     * @param message Toast message.
     * @return Toast.
     */
    [[nodiscard]] static Toast info(std::string message);

    /**
     * @brief Create a success toast.
     *
     * @param message Toast message.
     * @return Toast.
     */
    [[nodiscard]] static Toast success(std::string message);

    /**
     * @brief Create a warning toast.
     *
     * @param message Toast message.
     * @return Toast.
     */
    [[nodiscard]] static Toast warning(std::string message);

    /**
     * @brief Create an error toast.
     *
     * @param message Toast message.
     * @return Toast.
     */
    [[nodiscard]] static Toast error(std::string message);

    /**
     * @brief Create a neutral toast.
     *
     * @param message Toast message.
     * @return Toast.
     */
    [[nodiscard]] static Toast neutral(std::string message);

    /**
     * @brief Set the toast level.
     *
     * @param level Toast level.
     * @return This toast.
     */
    Toast &set_level(FlashLevel level) noexcept;

    /**
     * @brief Set the toast message.
     *
     * @param message Toast message.
     * @return This toast.
     */
    Toast &set_message(std::string message);

    /**
     * @brief Set the optional toast title.
     *
     * @param title Toast title.
     * @return This toast.
     */
    Toast &set_title(std::string title);

    /**
     * @brief Set the toast position.
     *
     * @param position Toast position.
     * @return This toast.
     */
    Toast &set_position(ToastPosition position) noexcept;

    /**
     * @brief Set whether the toast can be dismissed by the client.
     *
     * @param value Dismissible flag.
     * @return This toast.
     */
    Toast &set_dismissible(bool value) noexcept;

    /**
     * @brief Set whether the toast should auto-hide.
     *
     * @param value Auto-hide flag.
     * @return This toast.
     */
    Toast &set_auto_hide(bool value) noexcept;

    /**
     * @brief Set the auto-hide timeout.
     *
     * @param timeout Auto-hide timeout.
     * @return This toast.
     */
    Toast &set_timeout(std::chrono::milliseconds timeout) noexcept;

    /**
     * @brief Set or replace a custom wrapper attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This toast.
     */
    Toast &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean wrapper attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This toast.
     */
    Toast &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Get the toast level.
     *
     * @return Toast level.
     */
    [[nodiscard]] FlashLevel level() const noexcept;

    /**
     * @brief Get the toast message.
     *
     * @return Toast message.
     */
    [[nodiscard]] const std::string &message() const noexcept;

    /**
     * @brief Get the optional toast title.
     *
     * @return Toast title.
     */
    [[nodiscard]] const std::string &title() const noexcept;

    /**
     * @brief Get the toast position.
     *
     * @return Toast position.
     */
    [[nodiscard]] ToastPosition position() const noexcept;

    /**
     * @brief Check whether the toast can be dismissed.
     *
     * @return True if dismissible.
     */
    [[nodiscard]] bool dismissible() const noexcept;

    /**
     * @brief Check whether the toast should auto-hide.
     *
     * @return True if auto-hide is enabled.
     */
    [[nodiscard]] bool auto_hide() const noexcept;

    /**
     * @brief Get the auto-hide timeout.
     *
     * @return Auto-hide timeout.
     */
    [[nodiscard]] std::chrono::milliseconds timeout() const noexcept;

    /**
     * @brief Access custom wrapper attributes.
     *
     * @return Immutable attributes.
     */
    [[nodiscard]] const HtmlAttrs &attrs() const noexcept;

    /**
     * @brief Access custom wrapper attributes.
     *
     * @return Mutable attributes.
     */
    [[nodiscard]] HtmlAttrs &attrs() noexcept;

    /**
     * @brief Check whether the toast has a message.
     *
     * @return True if message is not empty.
     */
    [[nodiscard]] bool has_message() const noexcept;

    /**
     * @brief Check whether the toast has a title.
     *
     * @return True if title is not empty.
     */
    [[nodiscard]] bool has_title() const noexcept;

    /**
     * @brief Check whether the toast is empty.
     *
     * @return True if no message is set.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Render the toast as HTML.
     *
     * @return Rendered toast.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Toast level.
     */
    FlashLevel level_{FlashLevel::Info};

    /**
     * @brief Optional toast title.
     */
    std::string title_;

    /**
     * @brief Toast message.
     */
    std::string message_;

    /**
     * @brief Toast position.
     */
    ToastPosition position_{ToastPosition::TopRight};

    /**
     * @brief Whether the toast can be dismissed.
     */
    bool dismissible_{true};

    /**
     * @brief Whether the toast should auto-hide.
     */
    bool auto_hide_{true};

    /**
     * @brief Auto-hide timeout.
     */
    std::chrono::milliseconds timeout_{5000};

    /**
     * @brief Custom wrapper attributes.
     */
    HtmlAttrs attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_LIVE_TOAST_HPP
