/**
 *
 *  @file WebAppManifest.hpp
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
#ifndef VIX_UI_PWA_WEB_APP_MANIFEST_HPP
#define VIX_UI_PWA_WEB_APP_MANIFEST_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace vix::ui
{
  /**
   * @brief Web app display mode.
   *
   * WebAppDisplayMode describes how a web app should be displayed when
   * launched from a home screen or app launcher.
   */
  enum class WebAppDisplayMode
  {
    Browser = 0,
    MinimalUi,
    Standalone,
    Fullscreen
  };

  /**
   * @brief Web app orientation preference.
   */
  enum class WebAppOrientation
  {
    Any = 0,
    Natural,
    Portrait,
    Landscape,
    PortraitPrimary,
    PortraitSecondary,
    LandscapePrimary,
    LandscapeSecondary
  };

  /**
   * @brief Web app text direction.
   */
  enum class WebAppDirection
  {
    Auto = 0,
    Ltr,
    Rtl
  };

  /**
   * @brief Convert a display mode to a stable manifest string.
   *
   * @param mode Display mode.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(
      WebAppDisplayMode mode) noexcept;

  /**
   * @brief Convert an orientation to a stable manifest string.
   *
   * @param orientation Orientation value.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(
      WebAppOrientation orientation) noexcept;

  /**
   * @brief Convert a text direction to a stable manifest string.
   *
   * @param direction Text direction.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(
      WebAppDirection direction) noexcept;

  /**
   * @brief Web app manifest icon descriptor.
   *
   * WebAppManifestIcon stores the metadata for one icon entry in a web app
   * manifest.
   *
   * Example JSON:
   *
   * @code
   * {
   *   "src": "/icons/icon-192.png",
   *   "sizes": "192x192",
   *   "type": "image/png",
   *   "purpose": "any maskable"
   * }
   * @endcode
   */
  class WebAppManifestIcon
  {
  public:
    /**
     * @brief Construct an empty icon.
     */
    WebAppManifestIcon() = default;

    /**
     * @brief Construct an icon from a source path.
     *
     * @param src Icon source path.
     */
    explicit WebAppManifestIcon(std::string src);

    /**
     * @brief Construct an icon from common manifest fields.
     *
     * @param src Icon source path.
     * @param sizes Icon sizes value.
     * @param type Icon MIME type.
     */
    WebAppManifestIcon(
        std::string src,
        std::string sizes,
        std::string type = "image/png");

    /**
     * @brief Create an icon from common manifest fields.
     *
     * @param src Icon source path.
     * @param sizes Icon sizes value.
     * @param type Icon MIME type.
     * @return Icon descriptor.
     */
    [[nodiscard]] static WebAppManifestIcon make(
        std::string src,
        std::string sizes = {},
        std::string type = "image/png");

    /**
     * @brief Set the icon source path.
     *
     * @param src Icon source path.
     * @return This icon.
     */
    WebAppManifestIcon &set_src(std::string src);

    /**
     * @brief Set the icon sizes value.
     *
     * @param sizes Icon sizes value.
     * @return This icon.
     */
    WebAppManifestIcon &set_sizes(std::string sizes);

    /**
     * @brief Set the icon MIME type.
     *
     * @param type Icon MIME type.
     * @return This icon.
     */
    WebAppManifestIcon &set_type(std::string type);

    /**
     * @brief Set the icon purpose.
     *
     * Common values:
     * - any
     * - maskable
     * - monochrome
     * - any maskable
     *
     * @param purpose Icon purpose.
     * @return This icon.
     */
    WebAppManifestIcon &set_purpose(std::string purpose);

    /**
     * @brief Get the icon source path.
     *
     * @return Icon source path.
     */
    [[nodiscard]] const std::string &src() const noexcept;

    /**
     * @brief Get the icon sizes value.
     *
     * @return Icon sizes.
     */
    [[nodiscard]] const std::string &sizes() const noexcept;

    /**
     * @brief Get the icon MIME type.
     *
     * @return Icon MIME type.
     */
    [[nodiscard]] const std::string &type() const noexcept;

    /**
     * @brief Get the icon purpose.
     *
     * @return Icon purpose.
     */
    [[nodiscard]] const std::string &purpose() const noexcept;

    /**
     * @brief Check whether the icon has a source path.
     *
     * @return True if src is not empty.
     */
    [[nodiscard]] bool has_src() const noexcept;

    /**
     * @brief Check whether the icon has a sizes value.
     *
     * @return True if sizes is not empty.
     */
    [[nodiscard]] bool has_sizes() const noexcept;

    /**
     * @brief Check whether the icon has a MIME type.
     *
     * @return True if type is not empty.
     */
    [[nodiscard]] bool has_type() const noexcept;

    /**
     * @brief Check whether the icon has a purpose.
     *
     * @return True if purpose is not empty.
     */
    [[nodiscard]] bool has_purpose() const noexcept;

    /**
     * @brief Check whether the icon is empty.
     *
     * @return True if no source path is set.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Serialize the icon to JSON.
     *
     * @return JSON object string.
     */
    [[nodiscard]] std::string to_json() const;

  private:
    /**
     * @brief Icon source path.
     */
    std::string src_;

    /**
     * @brief Icon sizes value.
     */
    std::string sizes_;

    /**
     * @brief Icon MIME type.
     */
    std::string type_;

    /**
     * @brief Icon purpose.
     */
    std::string purpose_;
  };

  /**
   * @brief Web app manifest metadata helper.
   *
   * WebAppManifest builds a small manifest JSON document for installable
   * web apps and PWA-oriented Vix UI applications.
   *
   * It does not write files to disk, serve assets, or inspect the browser.
   * It only stores metadata and serializes it to deterministic JSON.
   */
  class WebAppManifest
  {
  public:
    /**
     * @brief Construct a default web app manifest.
     */
    WebAppManifest() = default;

    /**
     * @brief Create a default web app manifest.
     *
     * @return Web app manifest.
     */
    [[nodiscard]] static WebAppManifest make();

    /**
     * @brief Create a basic installable web app manifest.
     *
     * @param name Application name.
     * @param short_name Short application name.
     * @return Web app manifest.
     */
    [[nodiscard]] static WebAppManifest app(
        std::string name,
        std::string short_name = {});

    /**
     * @brief Set the application name.
     *
     * @param name Application name.
     * @return This manifest.
     */
    WebAppManifest &set_name(std::string name);

    /**
     * @brief Set the short application name.
     *
     * @param short_name Short application name.
     * @return This manifest.
     */
    WebAppManifest &set_short_name(std::string short_name);

    /**
     * @brief Set the application description.
     *
     * @param description Application description.
     * @return This manifest.
     */
    WebAppManifest &set_description(std::string description);

    /**
     * @brief Set the start URL.
     *
     * @param start_url Start URL.
     * @return This manifest.
     */
    WebAppManifest &set_start_url(std::string start_url);

    /**
     * @brief Set the navigation scope.
     *
     * @param scope Navigation scope.
     * @return This manifest.
     */
    WebAppManifest &set_scope(std::string scope);

    /**
     * @brief Set the application id.
     *
     * @param id Application id.
     * @return This manifest.
     */
    WebAppManifest &set_id(std::string id);

    /**
     * @brief Set the manifest language.
     *
     * @param lang Language tag.
     * @return This manifest.
     */
    WebAppManifest &set_lang(std::string lang);

    /**
     * @brief Set the text direction.
     *
     * @param direction Text direction.
     * @return This manifest.
     */
    WebAppManifest &set_direction(WebAppDirection direction) noexcept;

    /**
     * @brief Set the display mode.
     *
     * @param display Display mode.
     * @return This manifest.
     */
    WebAppManifest &set_display(WebAppDisplayMode display) noexcept;

    /**
     * @brief Set the orientation preference.
     *
     * @param orientation Orientation preference.
     * @return This manifest.
     */
    WebAppManifest &set_orientation(
        WebAppOrientation orientation) noexcept;

    /**
     * @brief Set the background color.
     *
     * @param color CSS color value.
     * @return This manifest.
     */
    WebAppManifest &set_background_color(std::string color);

    /**
     * @brief Set the theme color.
     *
     * @param color CSS color value.
     * @return This manifest.
     */
    WebAppManifest &set_theme_color(std::string color);

    /**
     * @brief Add an icon descriptor.
     *
     * Empty icons are ignored.
     *
     * @param icon Icon descriptor.
     * @return This manifest.
     */
    WebAppManifest &add_icon(WebAppManifestIcon icon);

    /**
     * @brief Add an icon from common manifest fields.
     *
     * @param src Icon source path.
     * @param sizes Icon sizes value.
     * @param type Icon MIME type.
     * @param purpose Icon purpose.
     * @return This manifest.
     */
    WebAppManifest &add_icon(
        std::string src,
        std::string sizes,
        std::string type = "image/png",
        std::string purpose = {});

    /**
     * @brief Remove all icons.
     */
    void clear_icons() noexcept;

    /**
     * @brief Get the application name.
     *
     * @return Application name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Get the short application name.
     *
     * @return Short application name.
     */
    [[nodiscard]] const std::string &short_name() const noexcept;

    /**
     * @brief Get the application description.
     *
     * @return Application description.
     */
    [[nodiscard]] const std::string &description() const noexcept;

    /**
     * @brief Get the start URL.
     *
     * @return Start URL.
     */
    [[nodiscard]] const std::string &start_url() const noexcept;

    /**
     * @brief Get the navigation scope.
     *
     * @return Navigation scope.
     */
    [[nodiscard]] const std::string &scope() const noexcept;

    /**
     * @brief Get the application id.
     *
     * @return Application id.
     */
    [[nodiscard]] const std::string &id() const noexcept;

    /**
     * @brief Get the manifest language.
     *
     * @return Language tag.
     */
    [[nodiscard]] const std::string &lang() const noexcept;

    /**
     * @brief Get the text direction.
     *
     * @return Text direction.
     */
    [[nodiscard]] WebAppDirection direction() const noexcept;

    /**
     * @brief Get the display mode.
     *
     * @return Display mode.
     */
    [[nodiscard]] WebAppDisplayMode display() const noexcept;

    /**
     * @brief Get the orientation preference.
     *
     * @return Orientation preference.
     */
    [[nodiscard]] WebAppOrientation orientation() const noexcept;

    /**
     * @brief Get the background color.
     *
     * @return Background color.
     */
    [[nodiscard]] const std::string &background_color() const noexcept;

    /**
     * @brief Get the theme color.
     *
     * @return Theme color.
     */
    [[nodiscard]] const std::string &theme_color() const noexcept;

    /**
     * @brief Access manifest icons.
     *
     * @return Immutable icon list.
     */
    [[nodiscard]] const std::vector<WebAppManifestIcon> &
    icons() const noexcept;

    /**
     * @brief Access manifest icons.
     *
     * @return Mutable icon list.
     */
    [[nodiscard]] std::vector<WebAppManifestIcon> &
    icons() noexcept;

    /**
     * @brief Check whether name is set.
     *
     * @return True if name is not empty.
     */
    [[nodiscard]] bool has_name() const noexcept;

    /**
     * @brief Check whether short name is set.
     *
     * @return True if short name is not empty.
     */
    [[nodiscard]] bool has_short_name() const noexcept;

    /**
     * @brief Check whether description is set.
     *
     * @return True if description is not empty.
     */
    [[nodiscard]] bool has_description() const noexcept;

    /**
     * @brief Check whether start URL is set.
     *
     * @return True if start URL is not empty.
     */
    [[nodiscard]] bool has_start_url() const noexcept;

    /**
     * @brief Check whether scope is set.
     *
     * @return True if scope is not empty.
     */
    [[nodiscard]] bool has_scope() const noexcept;

    /**
     * @brief Check whether id is set.
     *
     * @return True if id is not empty.
     */
    [[nodiscard]] bool has_id() const noexcept;

    /**
     * @brief Check whether language is set.
     *
     * @return True if language is not empty.
     */
    [[nodiscard]] bool has_lang() const noexcept;

    /**
     * @brief Check whether background color is set.
     *
     * @return True if background color is not empty.
     */
    [[nodiscard]] bool has_background_color() const noexcept;

    /**
     * @brief Check whether theme color is set.
     *
     * @return True if theme color is not empty.
     */
    [[nodiscard]] bool has_theme_color() const noexcept;

    /**
     * @brief Check whether icons are present.
     *
     * @return True if at least one icon is stored.
     */
    [[nodiscard]] bool has_icons() const noexcept;

    /**
     * @brief Get the number of stored icons.
     *
     * @return Icon count.
     */
    [[nodiscard]] std::size_t icon_count() const noexcept;

    /**
     * @brief Serialize the manifest to JSON.
     *
     * @return JSON object string.
     */
    [[nodiscard]] std::string to_json() const;

    /**
     * @brief Render the manifest JSON.
     *
     * This is an alias for to_json().
     *
     * @return JSON object string.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Application name.
     */
    std::string name_;

    /**
     * @brief Short application name.
     */
    std::string short_name_;

    /**
     * @brief Application description.
     */
    std::string description_;

    /**
     * @brief Start URL.
     */
    std::string start_url_{"/"};

    /**
     * @brief Navigation scope.
     */
    std::string scope_{"/"};

    /**
     * @brief Application id.
     */
    std::string id_;

    /**
     * @brief Manifest language.
     */
    std::string lang_;

    /**
     * @brief Text direction.
     */
    WebAppDirection direction_{WebAppDirection::Auto};

    /**
     * @brief Display mode.
     */
    WebAppDisplayMode display_{WebAppDisplayMode::Standalone};

    /**
     * @brief Orientation preference.
     */
    WebAppOrientation orientation_{WebAppOrientation::Any};

    /**
     * @brief Background color.
     */
    std::string background_color_;

    /**
     * @brief Theme color.
     */
    std::string theme_color_;

    /**
     * @brief Manifest icons.
     */
    std::vector<WebAppManifestIcon> icons_;
  };

} // namespace vix::ui

#endif // VIX_UI_PWA_WEB_APP_MANIFEST_HPP
