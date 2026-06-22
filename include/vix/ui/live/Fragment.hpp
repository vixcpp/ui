/**
 *
 *  @file Fragment.hpp
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
#ifndef VIX_UI_LIVE_FRAGMENT_HPP
#define VIX_UI_LIVE_FRAGMENT_HPP

#include <string>

#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Small server-rendered HTML fragment.
   *
   * Fragment represents a named piece of HTML that can be returned from a
   * route, embedded in a page, or sent later through a WebSocket-friendly
   * update payload.
   *
   * It does not know anything about WebSocket, HTTP, or a frontend runtime.
   * It only stores fragment metadata and rendered HTML.
   */
  class Fragment
  {
  public:
    /**
     * @brief Construct an empty fragment.
     */
    Fragment() = default;

    /**
     * @brief Construct a fragment with a name.
     *
     * @param name Logical fragment name.
     */
    explicit Fragment(std::string name);

    /**
     * @brief Construct a fragment with a name and HTML.
     *
     * @param name Logical fragment name.
     * @param html Rendered HTML.
     */
    Fragment(std::string name, std::string html);

    /**
     * @brief Create a fragment with an optional name.
     *
     * @param name Logical fragment name.
     * @return Fragment instance.
     */
    [[nodiscard]] static Fragment make(std::string name = {});

    /**
     * @brief Create a fragment from rendered HTML.
     *
     * @param html Rendered HTML.
     * @return Fragment instance.
     */
    [[nodiscard]] static Fragment html(std::string html);

    /**
     * @brief Set the logical fragment name.
     *
     * @param name Fragment name.
     * @return This fragment.
     */
    Fragment &set_name(std::string name);

    /**
     * @brief Set the target selector associated with this fragment.
     *
     * The target is optional metadata used by higher-level live updates.
     *
     * Example:
     * `#stats-card`
     *
     * @param target CSS-like target selector.
     * @return This fragment.
     */
    Fragment &set_target(std::string target);

    /**
     * @brief Replace the fragment HTML.
     *
     * The HTML is stored as-is. Use Html::text() before calling this method
     * when user-provided text must be escaped.
     *
     * @param html Rendered HTML.
     * @return This fragment.
     */
    Fragment &set_html(std::string html);

    /**
     * @brief Set or replace a custom HTML attribute.
     *
     * These attributes can be used by render_wrapped() when a wrapper tag is
     * needed around the fragment HTML.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This fragment.
     */
    Fragment &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean HTML attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This fragment.
     */
    Fragment &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Get the logical fragment name.
     *
     * @return Fragment name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Get the target selector.
     *
     * @return Target selector.
     */
    [[nodiscard]] const std::string &target() const noexcept;

    /**
     * @brief Get the fragment HTML.
     *
     * @return Rendered HTML.
     */
    [[nodiscard]] const std::string &html() const noexcept;

    /**
     * @brief Access custom HTML attributes.
     *
     * @return Immutable attributes.
     */
    [[nodiscard]] const HtmlAttrs &attrs() const noexcept;

    /**
     * @brief Access custom HTML attributes.
     *
     * @return Mutable attributes.
     */
    [[nodiscard]] HtmlAttrs &attrs() noexcept;

    /**
     * @brief Check whether the fragment has a name.
     *
     * @return True if name is not empty.
     */
    [[nodiscard]] bool has_name() const noexcept;

    /**
     * @brief Check whether the fragment has a target selector.
     *
     * @return True if target is not empty.
     */
    [[nodiscard]] bool has_target() const noexcept;

    /**
     * @brief Check whether the fragment has HTML.
     *
     * @return True if HTML is not empty.
     */
    [[nodiscard]] bool has_html() const noexcept;

    /**
     * @brief Check whether the fragment is empty.
     *
     * @return True if no HTML is stored.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the HTML size in bytes.
     *
     * @return HTML size.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Render the fragment HTML as-is.
     *
     * @return Fragment HTML.
     */
    [[nodiscard]] std::string render() const;

    /**
     * @brief Render the fragment inside a wrapper element.
     *
     * The wrapper receives:
     * - custom attributes from attrs()
     * - data-fragment when name is set
     * - data-target when target is set
     *
     * @param tag Wrapper tag name.
     * @return Wrapped fragment HTML.
     */
    [[nodiscard]] std::string render_wrapped(
        std::string tag = "div") const;

  private:
    /**
     * @brief Logical fragment name.
     */
    std::string name_;

    /**
     * @brief Optional target selector.
     */
    std::string target_;

    /**
     * @brief Rendered fragment HTML.
     */
    std::string html_;

    /**
     * @brief Optional wrapper attributes.
     */
    HtmlAttrs attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_LIVE_FRAGMENT_HPP
