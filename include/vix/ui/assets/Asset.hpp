/**
 *
 *  @file Asset.hpp
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
#ifndef VIX_UI_ASSETS_ASSET_HPP
#define VIX_UI_ASSETS_ASSET_HPP

#include <string>
#include <string_view>

#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Type of UI asset.
   *
   * AssetType describes how an asset should be used when rendered into
   * HTML or managed by the UI asset layer.
   */
  enum class AssetType
  {
    Unknown = 0,
    Stylesheet,
    Script,
    Image,
    Font,
    Other
  };

  /**
   * @brief Script loading behavior for asset rendering.
   */
  enum class AssetLoading
  {
    Default = 0,
    Deferred,
    Async
  };

  /**
   * @brief Convert an asset type to a stable string.
   *
   * @param type Asset type.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(AssetType type) noexcept;

  /**
   * @brief Convert asset loading behavior to a stable string.
   *
   * @param loading Asset loading behavior.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(AssetLoading loading) noexcept;

  /**
   * @brief UI asset descriptor.
   *
   * Asset describes a static resource used by server-rendered UI pages:
   * - CSS stylesheets
   * - JavaScript files
   * - JavaScript modules
   * - images
   * - fonts
   * - custom assets
   *
   * It stores metadata only. It does not read files from disk and does not
   * serve assets by itself.
   */
  class Asset
  {
  public:
    /**
     * @brief Construct an empty asset.
     */
    Asset() = default;

    /**
     * @brief Construct an asset.
     *
     * @param type Asset type.
     * @param url Public asset URL or path.
     */
    Asset(AssetType type, std::string url);

    /**
     * @brief Create a stylesheet asset.
     *
     * @param href Stylesheet URL.
     * @param media CSS media attribute.
     * @return Stylesheet asset.
     */
    [[nodiscard]] static Asset stylesheet(
        std::string href,
        std::string media = "all");

    /**
     * @brief Create a script asset.
     *
     * @param src Script URL.
     * @param loading Script loading behavior.
     * @return Script asset.
     */
    [[nodiscard]] static Asset script(
        std::string src,
        AssetLoading loading = AssetLoading::Default);

    /**
     * @brief Create a JavaScript module asset.
     *
     * The rendered script tag receives:
     *
     * @code
     * type="module"
     * @endcode
     *
     * @param src Script module URL.
     * @return Module script asset.
     */
    [[nodiscard]] static Asset module_script(std::string src);

    /**
     * @brief Create an image asset.
     *
     * @param src Image URL.
     * @param alt Alternative text.
     * @return Image asset.
     */
    [[nodiscard]] static Asset image(
        std::string src,
        std::string alt = {});

    /**
     * @brief Create a font preload asset.
     *
     * @param href Font URL.
     * @return Font asset.
     */
    [[nodiscard]] static Asset font(std::string href);

    /**
     * @brief Set the asset type.
     *
     * @param type Asset type.
     * @return This asset.
     */
    Asset &set_type(AssetType type) noexcept;

    /**
     * @brief Set the public asset URL or path.
     *
     * @param url Public asset URL or path.
     * @return This asset.
     */
    Asset &set_url(std::string url);

    /**
     * @brief Set the subresource integrity value.
     *
     * @param integrity Integrity value.
     * @return This asset.
     */
    Asset &set_integrity(std::string integrity);

    /**
     * @brief Set the crossorigin attribute value.
     *
     * @param crossorigin Cross-origin value.
     * @return This asset.
     */
    Asset &set_crossorigin(std::string crossorigin);

    /**
     * @brief Set the media attribute.
     *
     * Primarily used for stylesheet assets.
     *
     * @param media Media value.
     * @return This asset.
     */
    Asset &set_media(std::string media);

    /**
     * @brief Set script loading behavior.
     *
     * @param loading Loading behavior.
     * @return This asset.
     */
    Asset &set_loading(AssetLoading loading) noexcept;

    /**
     * @brief Set whether this script asset should render as a module script.
     *
     * When enabled, script assets render with:
     *
     * @code
     * type="module"
     * @endcode
     *
     * This flag is ignored by non-script asset types.
     *
     * @param value Module script flag.
     * @return This asset.
     */
    Asset &set_module(bool value = true) noexcept;

    /**
     * @brief Set or replace a custom HTML attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This asset.
     */
    Asset &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean HTML attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This asset.
     */
    Asset &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Get the asset type.
     *
     * @return Asset type.
     */
    [[nodiscard]] AssetType type() const noexcept;

    /**
     * @brief Get the public asset URL or path.
     *
     * @return Asset URL.
     */
    [[nodiscard]] const std::string &url() const noexcept;

    /**
     * @brief Get the subresource integrity value.
     *
     * @return Integrity value.
     */
    [[nodiscard]] const std::string &integrity() const noexcept;

    /**
     * @brief Get the crossorigin value.
     *
     * @return Cross-origin value.
     */
    [[nodiscard]] const std::string &crossorigin() const noexcept;

    /**
     * @brief Get the media value.
     *
     * @return Media value.
     */
    [[nodiscard]] const std::string &media() const noexcept;

    /**
     * @brief Get script loading behavior.
     *
     * @return Loading behavior.
     */
    [[nodiscard]] AssetLoading loading() const noexcept;

    /**
     * @brief Check whether this script asset renders as a module script.
     *
     * @return True if module script rendering is enabled.
     */
    [[nodiscard]] bool module() const noexcept;

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
     * @brief Check whether the asset has a URL.
     *
     * @return True if URL is not empty.
     */
    [[nodiscard]] bool has_url() const noexcept;

    /**
     * @brief Check whether the asset has integrity metadata.
     *
     * @return True if integrity is set.
     */
    [[nodiscard]] bool has_integrity() const noexcept;

    /**
     * @brief Check whether the asset has crossorigin metadata.
     *
     * @return True if crossorigin is set.
     */
    [[nodiscard]] bool has_crossorigin() const noexcept;

    /**
     * @brief Check whether the asset has media metadata.
     *
     * @return True if media is set.
     */
    [[nodiscard]] bool has_media() const noexcept;

    /**
     * @brief Render the asset as an HTML tag.
     *
     * @return HTML representation.
     *
     * @throws AssetError if the asset cannot be rendered.
     */
    [[nodiscard]] std::string to_html() const;

  private:
    /**
     * @brief Asset type.
     */
    AssetType type_{AssetType::Unknown};

    /**
     * @brief Public asset URL or path.
     */
    std::string url_;

    /**
     * @brief Subresource integrity value.
     */
    std::string integrity_;

    /**
     * @brief Cross-origin value.
     */
    std::string crossorigin_;

    /**
     * @brief Media value.
     */
    std::string media_;

    /**
     * @brief Script loading behavior.
     */
    AssetLoading loading_{AssetLoading::Default};

    /**
     * @brief Whether script assets should render with type="module".
     */
    bool module_{false};

    /**
     * @brief Custom HTML attributes.
     */
    HtmlAttrs attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_ASSETS_ASSET_HPP
