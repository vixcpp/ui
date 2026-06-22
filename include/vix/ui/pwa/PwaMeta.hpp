/**
 *
 *  @file PwaMeta.hpp
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
#ifndef VIX_UI_PWA_PWA_META_HPP
#define VIX_UI_PWA_PWA_META_HPP

#include <string>
#include <string_view>

#include <vix/ui/html/HtmlAttrs.hpp>
#include <vix/ui/pwa/Viewport.hpp>

namespace vix::ui
{
  /**
   * @brief iOS web app status bar style.
   *
   * PwaAppleStatusBarStyle controls the value rendered in the
   * apple-mobile-web-app-status-bar-style meta tag.
   */
  enum class PwaAppleStatusBarStyle
  {
    Default = 0,
    Black,
    BlackTranslucent
  };

  /**
   * @brief Convert an iOS status bar style to a stable string.
   *
   * @param style Status bar style.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(
      PwaAppleStatusBarStyle style) noexcept;

  /**
   * @brief PWA-oriented HTML metadata helper.
   *
   * PwaMeta renders the small HTML tags commonly needed by mobile-friendly
   * pages and installable web apps:
   * - viewport meta tag
   * - web app manifest link
   * - theme color meta tag
   * - application name meta tag
   * - selected iOS web app meta tags
   *
   * It does not generate the manifest JSON itself. WebAppManifest handles
   * manifest metadata, while PwaMeta renders the HTML tags that point to it.
   */
  class PwaMeta
  {
  public:
    /**
     * @brief Construct default PWA metadata.
     */
    PwaMeta() = default;

    /**
     * @brief Create default PWA metadata.
     *
     * @return PWA metadata helper.
     */
    [[nodiscard]] static PwaMeta make();

    /**
     * @brief Create mobile app-oriented PWA metadata.
     *
     * This enables viewport-fit=cover and iOS standalone metadata.
     *
     * @param app_name Application name.
     * @param theme_color Theme color.
     * @return PWA metadata helper.
     */
    [[nodiscard]] static PwaMeta mobile_app(
        std::string app_name = {},
        std::string theme_color = {});

    /**
     * @brief Set whether the viewport meta tag should be rendered.
     *
     * @param value Include viewport flag.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_include_viewport(bool value) noexcept;

    /**
     * @brief Set whether the web app manifest link should be rendered.
     *
     * @param value Include manifest flag.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_include_manifest(bool value) noexcept;

    /**
     * @brief Set the viewport helper.
     *
     * @param viewport Viewport helper.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_viewport(Viewport viewport);

    /**
     * @brief Set the manifest href.
     *
     * @param href Manifest URL or path.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_manifest_href(std::string href);

    /**
     * @brief Set the theme color.
     *
     * @param color CSS color value.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_theme_color(std::string color);

    /**
     * @brief Set the application name.
     *
     * @param name Application name.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_application_name(std::string name);

    /**
     * @brief Set the iOS mobile web app title.
     *
     * @param title iOS web app title.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_apple_mobile_web_app_title(std::string title);

    /**
     * @brief Set whether iOS should open the app in standalone mode.
     *
     * @param value True to render yes, false to render no.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_apple_mobile_web_app_capable(bool value) noexcept;

    /**
     * @brief Set the iOS status bar style.
     *
     * @param style Status bar style.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_apple_status_bar_style(
        PwaAppleStatusBarStyle style) noexcept;

    /**
     * @brief Set the color scheme meta value.
     *
     * Example:
     *
     * @code
     * light dark
     * @endcode
     *
     * @param scheme Color scheme value.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_color_scheme(std::string scheme);

    /**
     * @brief Set the format-detection meta value.
     *
     * Example:
     *
     * @code
     * telephone=no
     * @endcode
     *
     * @param value Format detection value.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_format_detection(std::string value);

    /**
     * @brief Set or replace a custom manifest link attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_manifest_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean manifest link attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This PWA metadata helper.
     */
    PwaMeta &set_manifest_bool_attr(
        std::string name,
        bool enabled = true);

    /**
     * @brief Check whether viewport rendering is enabled.
     *
     * @return True if viewport should be rendered.
     */
    [[nodiscard]] bool include_viewport() const noexcept;

    /**
     * @brief Check whether manifest link rendering is enabled.
     *
     * @return True if manifest link should be rendered.
     */
    [[nodiscard]] bool include_manifest() const noexcept;

    /**
     * @brief Get the viewport helper.
     *
     * @return Viewport helper.
     */
    [[nodiscard]] const Viewport &viewport() const noexcept;

    /**
     * @brief Get the mutable viewport helper.
     *
     * @return Viewport helper.
     */
    [[nodiscard]] Viewport &viewport() noexcept;

    /**
     * @brief Get the manifest href.
     *
     * @return Manifest href.
     */
    [[nodiscard]] const std::string &manifest_href() const noexcept;

    /**
     * @brief Get the theme color.
     *
     * @return Theme color.
     */
    [[nodiscard]] const std::string &theme_color() const noexcept;

    /**
     * @brief Get the application name.
     *
     * @return Application name.
     */
    [[nodiscard]] const std::string &application_name() const noexcept;

    /**
     * @brief Get the iOS mobile web app title.
     *
     * @return iOS web app title.
     */
    [[nodiscard]] const std::string &
    apple_mobile_web_app_title() const noexcept;

    /**
     * @brief Check whether iOS standalone mode metadata was configured.
     *
     * @return True if apple-mobile-web-app-capable should be rendered.
     */
    [[nodiscard]] bool has_apple_mobile_web_app_capable() const noexcept;

    /**
     * @brief Get the iOS standalone mode value.
     *
     * @return True if standalone mode is enabled.
     */
    [[nodiscard]] bool apple_mobile_web_app_capable() const noexcept;

    /**
     * @brief Check whether iOS status bar style was configured.
     *
     * @return True if status bar style should be rendered.
     */
    [[nodiscard]] bool has_apple_status_bar_style() const noexcept;

    /**
     * @brief Get the iOS status bar style.
     *
     * @return Status bar style.
     */
    [[nodiscard]] PwaAppleStatusBarStyle
    apple_status_bar_style() const noexcept;

    /**
     * @brief Get the color scheme value.
     *
     * @return Color scheme.
     */
    [[nodiscard]] const std::string &color_scheme() const noexcept;

    /**
     * @brief Get the format detection value.
     *
     * @return Format detection value.
     */
    [[nodiscard]] const std::string &format_detection() const noexcept;

    /**
     * @brief Access custom manifest link attributes.
     *
     * @return Immutable manifest link attributes.
     */
    [[nodiscard]] const HtmlAttrs &manifest_attrs() const noexcept;

    /**
     * @brief Access custom manifest link attributes.
     *
     * @return Mutable manifest link attributes.
     */
    [[nodiscard]] HtmlAttrs &manifest_attrs() noexcept;

    /**
     * @brief Check whether manifest href is set.
     *
     * @return True if manifest href is not empty.
     */
    [[nodiscard]] bool has_manifest_href() const noexcept;

    /**
     * @brief Check whether theme color is set.
     *
     * @return True if theme color is not empty.
     */
    [[nodiscard]] bool has_theme_color() const noexcept;

    /**
     * @brief Check whether application name is set.
     *
     * @return True if application name is not empty.
     */
    [[nodiscard]] bool has_application_name() const noexcept;

    /**
     * @brief Check whether iOS web app title is set.
     *
     * @return True if iOS web app title is not empty.
     */
    [[nodiscard]] bool has_apple_mobile_web_app_title() const noexcept;

    /**
     * @brief Check whether color scheme is set.
     *
     * @return True if color scheme is not empty.
     */
    [[nodiscard]] bool has_color_scheme() const noexcept;

    /**
     * @brief Check whether format detection is set.
     *
     * @return True if format detection is not empty.
     */
    [[nodiscard]] bool has_format_detection() const noexcept;

    /**
     * @brief Render the viewport meta tag.
     *
     * @return Rendered viewport meta tag or empty string.
     */
    [[nodiscard]] std::string render_viewport() const;

    /**
     * @brief Render the manifest link tag.
     *
     * @return Rendered manifest link tag or empty string.
     */
    [[nodiscard]] std::string render_manifest_link() const;

    /**
     * @brief Render the theme color meta tag.
     *
     * @return Rendered theme color meta tag or empty string.
     */
    [[nodiscard]] std::string render_theme_color() const;

    /**
     * @brief Render the application name meta tag.
     *
     * @return Rendered application name meta tag or empty string.
     */
    [[nodiscard]] std::string render_application_name() const;

    /**
     * @brief Render the iOS standalone capable meta tag.
     *
     * @return Rendered iOS capable meta tag or empty string.
     */
    [[nodiscard]] std::string render_apple_mobile_web_app_capable() const;

    /**
     * @brief Render the iOS mobile web app title meta tag.
     *
     * @return Rendered iOS title meta tag or empty string.
     */
    [[nodiscard]] std::string render_apple_mobile_web_app_title() const;

    /**
     * @brief Render the iOS status bar style meta tag.
     *
     * @return Rendered iOS status bar style meta tag or empty string.
     */
    [[nodiscard]] std::string render_apple_status_bar_style() const;

    /**
     * @brief Render the color scheme meta tag.
     *
     * @return Rendered color scheme meta tag or empty string.
     */
    [[nodiscard]] std::string render_color_scheme() const;

    /**
     * @brief Render the format detection meta tag.
     *
     * @return Rendered format detection meta tag or empty string.
     */
    [[nodiscard]] std::string render_format_detection() const;

    /**
     * @brief Render all configured PWA metadata tags.
     *
     * @return Rendered metadata tags separated by newlines.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Viewport helper.
     */
    Viewport viewport_{Viewport::mobile_app()};

    /**
     * @brief Whether the viewport meta tag should be rendered.
     */
    bool include_viewport_{true};

    /**
     * @brief Whether the manifest link should be rendered.
     */
    bool include_manifest_{true};

    /**
     * @brief Manifest href.
     */
    std::string manifest_href_{"/manifest.webmanifest"};

    /**
     * @brief Theme color.
     */
    std::string theme_color_;

    /**
     * @brief Application name.
     */
    std::string application_name_;

    /**
     * @brief iOS mobile web app title.
     */
    std::string apple_mobile_web_app_title_;

    /**
     * @brief Whether iOS standalone metadata should be rendered.
     */
    bool apple_mobile_web_app_capable_set_{false};

    /**
     * @brief iOS standalone metadata value.
     */
    bool apple_mobile_web_app_capable_{false};

    /**
     * @brief Whether iOS status bar style should be rendered.
     */
    bool apple_status_bar_style_set_{false};

    /**
     * @brief iOS status bar style.
     */
    PwaAppleStatusBarStyle apple_status_bar_style_{
        PwaAppleStatusBarStyle::Default};

    /**
     * @brief Color scheme meta value.
     */
    std::string color_scheme_;

    /**
     * @brief Format detection meta value.
     */
    std::string format_detection_;

    /**
     * @brief Custom manifest link attributes.
     */
    HtmlAttrs manifest_attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_PWA_PWA_META_HPP
