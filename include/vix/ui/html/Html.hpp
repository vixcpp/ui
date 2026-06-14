/**
 *
 *  @file Html.hpp
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
#ifndef VIX_UI_HTML_HTML_HPP
#define VIX_UI_HTML_HTML_HPP

#include <string>
#include <string_view>

#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Small HTML generation helper for UI fragments.
   *
   * Html provides safe helpers for building small HTML strings from C++.
   * It is intended for UI helpers, examples, forms, assets and small
   * server-rendered fragments.
   *
   * It does not replace the Vix template module. Large pages should still
   * be rendered with templates. This helper exists for small generated
   * elements where using a full template would be unnecessary.
   */
  class Html
  {
  public:
    /**
     * @brief Escape text for safe HTML output.
     *
     * @param input Raw input text.
     * @return Escaped HTML text.
     */
    [[nodiscard]] static std::string escape(std::string_view input);

    /**
     * @brief Create an escaped HTML text node.
     *
     * @param input Raw text content.
     * @return Escaped HTML text.
     */
    [[nodiscard]] static std::string text(std::string_view input);

    /**
     * @brief Return raw HTML without escaping.
     *
     * This should only be used with trusted HTML.
     *
     * @param input Trusted HTML content.
     * @return Raw HTML string.
     */
    [[nodiscard]] static std::string raw(std::string_view input);

    /**
     * @brief Create a standard HTML doctype.
     *
     * @return HTML5 doctype string.
     */
    [[nodiscard]] static std::string doctype();

    /**
     * @brief Create an opening HTML tag.
     *
     * Example:
     * `<div class="card">`
     *
     * @param name Tag name.
     * @param attrs Optional HTML attributes.
     * @return Opening tag string.
     *
     * @throws HtmlError if the tag name is invalid.
     */
    [[nodiscard]] static std::string open_tag(
        std::string_view name,
        const HtmlAttrs &attrs = {});

    /**
     * @brief Create a closing HTML tag.
     *
     * Example:
     * `</div>`
     *
     * @param name Tag name.
     * @return Closing tag string.
     *
     * @throws HtmlError if the tag name is invalid.
     */
    [[nodiscard]] static std::string close_tag(std::string_view name);

    /**
     * @brief Create a complete HTML element.
     *
     * Content is inserted as-is. Use Html::text() when content must be
     * escaped before insertion.
     *
     * @param name Tag name.
     * @param content Element content.
     * @param attrs Optional HTML attributes.
     * @return Complete HTML element.
     *
     * @throws HtmlError if the tag name is invalid.
     */
    [[nodiscard]] static std::string tag(
        std::string_view name,
        std::string_view content,
        const HtmlAttrs &attrs = {});

    /**
     * @brief Create a void HTML tag.
     *
     * Example:
     * `<meta charset="utf-8">`
     *
     * @param name Tag name.
     * @param attrs Optional HTML attributes.
     * @return Void tag string.
     *
     * @throws HtmlError if the tag name is invalid.
     */
    [[nodiscard]] static std::string void_tag(
        std::string_view name,
        const HtmlAttrs &attrs = {});

    /**
     * @brief Check whether an HTML tag name is valid.
     *
     * This validation is intentionally conservative and accepts common
     * tag name characters:
     * - letters
     * - numbers
     * - dash
     * - colon
     *
     * @param name Tag name.
     * @return True if valid.
     */
    [[nodiscard]] static bool is_valid_tag_name(
        std::string_view name) noexcept;
  };

} // namespace vix::ui

#endif // VIX_UI_HTML_HTML_HPP
