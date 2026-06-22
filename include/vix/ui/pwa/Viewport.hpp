/**
 *
 *  @file Viewport.hpp
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
#ifndef VIX_UI_PWA_VIEWPORT_HPP
#define VIX_UI_PWA_VIEWPORT_HPP

#include <string>
#include <string_view>

#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Viewport fit behavior for mobile browsers.
   *
   * ViewportFit controls how the page uses the available screen area,
   * especially on devices with display cutouts, rounded corners, or
   * safe-area insets.
   */
  enum class ViewportFit
  {
    Auto = 0,
    Contain,
    Cover
  };

  /**
   * @brief Convert a viewport fit value to a stable string.
   *
   * @param fit Viewport fit value.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(ViewportFit fit) noexcept;

  /**
   * @brief Mobile viewport meta tag helper.
   *
   * Viewport builds the HTML meta viewport tag used by mobile browsers and
   * installable web apps.
   *
   * It is intended for server-rendered Vix UI pages that need predictable
   * mobile behavior without manually writing viewport strings.
   *
   * Example output:
   *
   * @code
   * <meta content="width=device-width, initial-scale=1" name="viewport">
   * @endcode
   */
  class Viewport
  {
  public:
    /**
     * @brief Construct a default responsive viewport.
     */
    Viewport() = default;

    /**
     * @brief Create a default responsive viewport.
     *
     * @return Responsive viewport.
     */
    [[nodiscard]] static Viewport responsive();

    /**
     * @brief Create a mobile app-oriented viewport.
     *
     * This enables viewport-fit=cover so the page can use safe-area CSS
     * variables on modern mobile devices.
     *
     * @return Mobile app viewport.
     */
    [[nodiscard]] static Viewport mobile_app();

    /**
     * @brief Set the viewport width value.
     *
     * Common value:
     *
     * @code
     * device-width
     * @endcode
     *
     * @param width Viewport width value.
     * @return This viewport.
     */
    Viewport &set_width(std::string width);

    /**
     * @brief Set the initial scale value.
     *
     * Common value:
     *
     * @code
     * 1
     * @endcode
     *
     * @param scale Initial scale value.
     * @return This viewport.
     */
    Viewport &set_initial_scale(std::string scale);

    /**
     * @brief Set the minimum scale value.
     *
     * @param scale Minimum scale value.
     * @return This viewport.
     */
    Viewport &set_minimum_scale(std::string scale);

    /**
     * @brief Set the maximum scale value.
     *
     * @param scale Maximum scale value.
     * @return This viewport.
     */
    Viewport &set_maximum_scale(std::string scale);

    /**
     * @brief Set whether the user can zoom the page.
     *
     * @param value True to allow scaling, false to disable it.
     * @return This viewport.
     */
    Viewport &set_user_scalable(bool value);

    /**
     * @brief Set viewport-fit behavior.
     *
     * @param fit Viewport fit behavior.
     * @return This viewport.
     */
    Viewport &set_viewport_fit(ViewportFit fit) noexcept;

    /**
     * @brief Set or replace a custom meta tag attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This viewport.
     */
    Viewport &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean meta tag attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This viewport.
     */
    Viewport &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Get the viewport width value.
     *
     * @return Width value.
     */
    [[nodiscard]] const std::string &width() const noexcept;

    /**
     * @brief Get the initial scale value.
     *
     * @return Initial scale value.
     */
    [[nodiscard]] const std::string &initial_scale() const noexcept;

    /**
     * @brief Get the minimum scale value.
     *
     * @return Minimum scale value.
     */
    [[nodiscard]] const std::string &minimum_scale() const noexcept;

    /**
     * @brief Get the maximum scale value.
     *
     * @return Maximum scale value.
     */
    [[nodiscard]] const std::string &maximum_scale() const noexcept;

    /**
     * @brief Get the user-scalable value.
     *
     * @return User-scalable value.
     */
    [[nodiscard]] const std::string &user_scalable() const noexcept;

    /**
     * @brief Get the viewport-fit value.
     *
     * @return Viewport fit value.
     */
    [[nodiscard]] ViewportFit viewport_fit() const noexcept;

    /**
     * @brief Access custom meta tag attributes.
     *
     * @return Immutable attributes.
     */
    [[nodiscard]] const HtmlAttrs &attrs() const noexcept;

    /**
     * @brief Access custom meta tag attributes.
     *
     * @return Mutable attributes.
     */
    [[nodiscard]] HtmlAttrs &attrs() noexcept;

    /**
     * @brief Check whether width is set.
     *
     * @return True if width is not empty.
     */
    [[nodiscard]] bool has_width() const noexcept;

    /**
     * @brief Check whether initial scale is set.
     *
     * @return True if initial scale is not empty.
     */
    [[nodiscard]] bool has_initial_scale() const noexcept;

    /**
     * @brief Check whether minimum scale is set.
     *
     * @return True if minimum scale is not empty.
     */
    [[nodiscard]] bool has_minimum_scale() const noexcept;

    /**
     * @brief Check whether maximum scale is set.
     *
     * @return True if maximum scale is not empty.
     */
    [[nodiscard]] bool has_maximum_scale() const noexcept;

    /**
     * @brief Check whether user-scalable is set.
     *
     * @return True if user-scalable is not empty.
     */
    [[nodiscard]] bool has_user_scalable() const noexcept;

    /**
     * @brief Build the viewport content attribute.
     *
     * @return Viewport content value.
     */
    [[nodiscard]] std::string content() const;

    /**
     * @brief Render the viewport as a meta tag.
     *
     * @return Rendered viewport meta tag.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Viewport width value.
     */
    std::string width_{"device-width"};

    /**
     * @brief Initial scale value.
     */
    std::string initial_scale_{"1"};

    /**
     * @brief Optional minimum scale value.
     */
    std::string minimum_scale_;

    /**
     * @brief Optional maximum scale value.
     */
    std::string maximum_scale_;

    /**
     * @brief Optional user-scalable value.
     */
    std::string user_scalable_;

    /**
     * @brief Viewport fit behavior.
     */
    ViewportFit viewport_fit_{ViewportFit::Auto};

    /**
     * @brief Custom meta tag attributes.
     */
    HtmlAttrs attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_PWA_VIEWPORT_HPP
