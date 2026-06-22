/**
 *
 *  @file FlashMessage.hpp
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
#ifndef VIX_UI_LIVE_FLASH_MESSAGE_HPP
#define VIX_UI_LIVE_FLASH_MESSAGE_HPP

#include <string>
#include <string_view>

#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Supported flash message levels.
   */
  enum class FlashLevel
  {
    Info = 0,
    Success,
    Warning,
    Error,
    Neutral
  };

  /**
   * @brief Convert a flash level to a stable string.
   *
   * @param level Flash message level.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(FlashLevel level) noexcept;

  /**
   * @brief Server-rendered flash message helper.
   *
   * FlashMessage represents a short message rendered by the server and
   * displayed in a page, dashboard, admin panel, or live update response.
   *
   * It is intentionally small and transport-neutral. The rendered HTML can
   * be returned from a route or sent through a WebSocket update.
   */
  class FlashMessage
  {
  public:
    /**
     * @brief Construct an empty info flash message.
     */
    FlashMessage() = default;

    /**
     * @brief Construct a flash message.
     *
     * @param level Message level.
     * @param message Message body.
     */
    FlashMessage(FlashLevel level, std::string message);

    /**
     * @brief Create an info flash message.
     *
     * @param message Message body.
     * @return Flash message.
     */
    [[nodiscard]] static FlashMessage info(std::string message);

    /**
     * @brief Create a success flash message.
     *
     * @param message Message body.
     * @return Flash message.
     */
    [[nodiscard]] static FlashMessage success(std::string message);

    /**
     * @brief Create a warning flash message.
     *
     * @param message Message body.
     * @return Flash message.
     */
    [[nodiscard]] static FlashMessage warning(std::string message);

    /**
     * @brief Create an error flash message.
     *
     * @param message Message body.
     * @return Flash message.
     */
    [[nodiscard]] static FlashMessage error(std::string message);

    /**
     * @brief Create a neutral flash message.
     *
     * @param message Message body.
     * @return Flash message.
     */
    [[nodiscard]] static FlashMessage neutral(std::string message);

    /**
     * @brief Set the flash message level.
     *
     * @param level Message level.
     * @return This message.
     */
    FlashMessage &set_level(FlashLevel level) noexcept;

    /**
     * @brief Set the message body.
     *
     * @param message Message body.
     * @return This message.
     */
    FlashMessage &set_message(std::string message);

    /**
     * @brief Set an optional title.
     *
     * @param title Message title.
     * @return This message.
     */
    FlashMessage &set_title(std::string title);

    /**
     * @brief Set whether this message can be dismissed by the client.
     *
     * @param value Dismissible flag.
     * @return This message.
     */
    FlashMessage &set_dismissible(bool value) noexcept;

    /**
     * @brief Set or replace a custom wrapper attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This message.
     */
    FlashMessage &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean wrapper attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This message.
     */
    FlashMessage &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Get the flash message level.
     *
     * @return Message level.
     */
    [[nodiscard]] FlashLevel level() const noexcept;

    /**
     * @brief Get the message body.
     *
     * @return Message body.
     */
    [[nodiscard]] const std::string &message() const noexcept;

    /**
     * @brief Get the optional title.
     *
     * @return Message title.
     */
    [[nodiscard]] const std::string &title() const noexcept;

    /**
     * @brief Check whether the message is dismissible.
     *
     * @return True if dismissible.
     */
    [[nodiscard]] bool dismissible() const noexcept;

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
     * @brief Check whether the message has a body.
     *
     * @return True if message is not empty.
     */
    [[nodiscard]] bool has_message() const noexcept;

    /**
     * @brief Check whether the message has a title.
     *
     * @return True if title is not empty.
     */
    [[nodiscard]] bool has_title() const noexcept;

    /**
     * @brief Check whether the message is empty.
     *
     * @return True if no message body is set.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Render the flash message as HTML.
     *
     * @return Rendered flash message.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Flash message level.
     */
    FlashLevel level_{FlashLevel::Info};

    /**
     * @brief Optional title.
     */
    std::string title_;

    /**
     * @brief Message body.
     */
    std::string message_;

    /**
     * @brief Whether the message can be dismissed.
     */
    bool dismissible_{false};

    /**
     * @brief Custom wrapper attributes.
     */
    HtmlAttrs attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_LIVE_FLASH_MESSAGE_HPP
