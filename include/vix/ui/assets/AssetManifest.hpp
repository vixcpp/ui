/**
 *
 *  @file AssetManifest.hpp
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
#ifndef VIX_UI_ASSETS_ASSET_MANIFEST_HPP
#define VIX_UI_ASSETS_ASSET_MANIFEST_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include <vix/ui/assets/Asset.hpp>

namespace vix::ui
{
  /**
   * @brief Collection of UI assets used by a page or app shell.
   *
   * AssetManifest stores the static assets needed by a rendered UI:
   * - stylesheets
   * - scripts
   * - images
   * - fonts
   * - custom assets
   *
   * It does not serve files directly. It only describes and renders asset
   * references as HTML tags.
   */
  class AssetManifest
  {
  public:
    /**
     * @brief Construct an empty asset manifest.
     */
    AssetManifest() = default;

    /**
     * @brief Construct a manifest from an asset list.
     *
     * @param assets Initial assets.
     */
    explicit AssetManifest(std::vector<Asset> assets);

    /**
     * @brief Create an empty asset manifest.
     *
     * @return Empty asset manifest.
     */
    [[nodiscard]] static AssetManifest make();

    /**
     * @brief Add an asset to the manifest.
     *
     * @param asset Asset to add.
     * @return This manifest.
     *
     * @throws AssetError if the asset has no URL.
     */
    AssetManifest &add(Asset asset);

    /**
     * @brief Add a stylesheet asset.
     *
     * @param href Stylesheet URL.
     * @param media CSS media attribute.
     * @return This manifest.
     */
    AssetManifest &add_stylesheet(
        std::string href,
        std::string media = "all");

    /**
     * @brief Add a script asset.
     *
     * @param src Script URL.
     * @param loading Script loading behavior.
     * @return This manifest.
     */
    AssetManifest &add_script(
        std::string src,
        AssetLoading loading = AssetLoading::Default);

    /**
     * @brief Add an image asset.
     *
     * @param src Image URL.
     * @param alt Alternative text.
     * @return This manifest.
     */
    AssetManifest &add_image(
        std::string src,
        std::string alt = {});

    /**
     * @brief Add a font preload asset.
     *
     * @param href Font URL.
     * @return This manifest.
     */
    AssetManifest &add_font(std::string href);

    /**
     * @brief Check whether an asset URL exists in the manifest.
     *
     * @param url Asset URL.
     * @return True if found.
     */
    [[nodiscard]] bool contains(std::string_view url) const noexcept;

    /**
     * @brief Find an asset by URL.
     *
     * @param url Asset URL.
     * @return Pointer to the asset if found, otherwise nullptr.
     */
    [[nodiscard]] const Asset *find(std::string_view url) const noexcept;

    /**
     * @brief Find a mutable asset by URL.
     *
     * @param url Asset URL.
     * @return Pointer to the asset if found, otherwise nullptr.
     */
    [[nodiscard]] Asset *find(std::string_view url) noexcept;

    /**
     * @brief Remove all assets with a matching URL.
     *
     * @param url Asset URL.
     * @return Number of removed assets.
     */
    std::size_t remove(std::string_view url);

    /**
     * @brief Remove all assets.
     */
    void clear() noexcept;

    /**
     * @brief Check whether the manifest is empty.
     *
     * @return True if no assets are stored.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the number of stored assets.
     *
     * @return Asset count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Access all assets.
     *
     * @return Immutable asset list.
     */
    [[nodiscard]] const std::vector<Asset> &assets() const noexcept;

    /**
     * @brief Access all assets.
     *
     * @return Mutable asset list.
     */
    [[nodiscard]] std::vector<Asset> &assets() noexcept;

    /**
     * @brief Render all assets as HTML tags.
     *
     * @return Rendered HTML asset tags separated by newlines.
     *
     * @throws AssetError if any asset cannot be rendered.
     */
    [[nodiscard]] std::string render() const;

    /**
     * @brief Render only assets of a specific type.
     *
     * @param type Asset type to render.
     * @return Rendered HTML asset tags separated by newlines.
     *
     * @throws AssetError if any matching asset cannot be rendered.
     */
    [[nodiscard]] std::string render_type(AssetType type) const;

    /**
     * @brief Render stylesheet assets.
     *
     * @return Rendered stylesheet tags.
     */
    [[nodiscard]] std::string render_stylesheets() const;

    /**
     * @brief Render script assets.
     *
     * @return Rendered script tags.
     */
    [[nodiscard]] std::string render_scripts() const;

    /**
     * @brief Render font assets.
     *
     * @return Rendered font preload tags.
     */
    [[nodiscard]] std::string render_fonts() const;

  private:
    /**
     * @brief Stored assets in insertion order.
     */
    std::vector<Asset> assets_;
  };

} // namespace vix::ui

#endif // VIX_UI_ASSETS_ASSET_MANIFEST_HPP
