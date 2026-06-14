/**
 *
 *  @file HtmlAttrs.hpp
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
#ifndef VIX_UI_HTML_HTML_ATTRS_HPP
#define VIX_UI_HTML_HTML_ATTRS_HPP

#include <cstddef>
#include <map>
#include <string>
#include <string_view>

namespace vix::ui
{
  /**
   * @brief Single HTML attribute stored by HtmlAttrs.
   *
   * HtmlAttr supports both normal attributes:
   * - class="card"
   * - href="/docs"
   *
   * and boolean attributes:
   * - disabled
   * - checked
   * - selected
   */
  struct HtmlAttr
  {
    /**
     * @brief Attribute value.
     */
    std::string value;

    /**
     * @brief Whether this is a boolean attribute.
     */
    bool boolean{false};
  };

  /**
   * @brief HTML attribute collection helper.
   *
   * HtmlAttrs stores a deterministic set of HTML attributes and can render
   * them into a safe HTML attribute string.
   *
   * Attribute values are escaped during rendering. Attribute names are
   * validated before insertion.
   */
  class HtmlAttrs
  {
  public:
    /**
     * @brief Construct an empty attribute collection.
     */
    HtmlAttrs() = default;

    /**
     * @brief Construct attributes from a plain string map.
     *
     * @param attrs Initial attribute values.
     */
    explicit HtmlAttrs(std::map<std::string, std::string> attrs);

    /**
     * @brief Create an empty attribute collection.
     *
     * @return Empty attribute collection.
     */
    [[nodiscard]] static HtmlAttrs make();

    /**
     * @brief Set or replace a normal HTML attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This attribute collection.
     *
     * @throws HtmlError if the attribute name is invalid.
     */
    HtmlAttrs &set(std::string name, std::string value);

    /**
     * @brief Set an attribute only when a condition is true.
     *
     * @param condition Whether the attribute should be set.
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This attribute collection.
     *
     * @throws HtmlError if the attribute name is invalid.
     */
    HtmlAttrs &set_if(
        bool condition,
        std::string name,
        std::string value);

    /**
     * @brief Set or remove a boolean HTML attribute.
     *
     * When enabled is true, the attribute is rendered without a value.
     * When enabled is false, the attribute is removed.
     *
     * @param name Attribute name.
     * @param enabled Whether the boolean attribute should be present.
     * @return This attribute collection.
     *
     * @throws HtmlError if the attribute name is invalid.
     */
    HtmlAttrs &boolean(std::string name, bool enabled = true);

    /**
     * @brief Check whether an attribute exists.
     *
     * @param name Attribute name.
     * @return True if present.
     */
    [[nodiscard]] bool has(const std::string &name) const;

    /**
     * @brief Get an attribute by name.
     *
     * @param name Attribute name.
     * @return Pointer to the attribute if found, otherwise nullptr.
     */
    [[nodiscard]] const HtmlAttr *get(const std::string &name) const noexcept;

    /**
     * @brief Get a mutable attribute by name.
     *
     * @param name Attribute name.
     * @return Pointer to the attribute if found, otherwise nullptr.
     */
    [[nodiscard]] HtmlAttr *get(const std::string &name) noexcept;

    /**
     * @brief Remove an attribute.
     *
     * @param name Attribute name.
     * @return True if an attribute was removed.
     */
    bool erase(const std::string &name);

    /**
     * @brief Remove all attributes.
     */
    void clear() noexcept;

    /**
     * @brief Check whether the collection is empty.
     *
     * @return True if no attributes are stored.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the number of stored attributes.
     *
     * @return Attribute count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Access the stored attributes.
     *
     * @return Immutable attribute map.
     */
    [[nodiscard]] const std::map<std::string, HtmlAttr> &values() const noexcept;

    /**
     * @brief Access the stored attributes.
     *
     * @return Mutable attribute map.
     */
    [[nodiscard]] std::map<std::string, HtmlAttr> &values() noexcept;

    /**
     * @brief Render attributes without a leading space.
     *
     * Example:
     * class="card" disabled
     *
     * @return Rendered attribute string.
     */
    [[nodiscard]] std::string render() const;

    /**
     * @brief Render attributes with a leading space when not empty.
     *
     * Example:
     *  class="card" disabled
     *
     * @return Rendered attribute string or empty string.
     */
    [[nodiscard]] std::string render_with_leading_space() const;

    /**
     * @brief Check whether an HTML attribute name is valid.
     *
     * This validation is intentionally conservative and accepts common
     * attribute characters:
     * - letters
     * - numbers
     * - underscore
     * - dash
     * - colon
     * - dot
     *
     * @param name Attribute name.
     * @return True if valid.
     */
    [[nodiscard]] static bool is_valid_name(std::string_view name) noexcept;

  private:
    /**
     * @brief Stored attributes indexed by name.
     */
    std::map<std::string, HtmlAttr> attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_HTML_HTML_ATTRS_HPP
