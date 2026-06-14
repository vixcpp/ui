/**
 *
 *  @file HtmlEscape.hpp
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
#ifndef VIX_UI_HTML_HTML_ESCAPE_HPP
#define VIX_UI_HTML_HTML_ESCAPE_HPP

#include <string>
#include <string_view>

namespace vix::ui
{
  /**
   * @brief HTML escaping helper for UI output.
   *
   * HtmlEscape provides small utilities for escaping strings before they
   * are inserted into HTML attributes, text nodes, or manually generated
   * UI fragments.
   *
   * This helper is intentionally simple and deterministic. It does not
   * replace the template module auto-escaping behavior. It exists for UI
   * helpers that build HTML directly.
   */
  class HtmlEscape
  {
  public:
    /**
     * @brief Escape a string for safe HTML output.
     *
     * The following characters are escaped:
     * - '&'  -> "&amp;"
     * - '<'  -> "&lt;"
     * - '>'  -> "&gt;"
     * - '"'  -> "&quot;"
     * - '\'' -> "&#39;"
     *
     * @param input Raw input string.
     * @return Escaped HTML string.
     */
    [[nodiscard]] static std::string text(std::string_view input);

    /**
     * @brief Escape a string for safe HTML attribute output.
     *
     * This currently uses the same escaping rules as text output.
     *
     * @param input Raw input string.
     * @return Escaped HTML attribute string.
     */
    [[nodiscard]] static std::string attr(std::string_view input);

    /**
     * @brief Check whether a character requires HTML escaping.
     *
     * @param c Character to inspect.
     * @return True if the character must be escaped.
     */
    [[nodiscard]] static bool needs_escape(char c) noexcept;
  };

} // namespace vix::ui

#endif // VIX_UI_HTML_HTML_ESCAPE_HPP
