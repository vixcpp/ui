/**
 *
 *  @file AssetManager.hpp
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
#ifndef VIX_UI_ASSETS_ASSET_MANAGER_HPP
#define VIX_UI_ASSETS_ASSET_MANAGER_HPP

#include <cstddef>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include <vix/ui/assets/Asset.hpp>
#include <vix/ui/assets/AssetManifest.hpp>

namespace vix::ui
{
  /**
   * @brief Manager for named UI assets.
   *
   * AssetManager stores named assets and provides helpers for resolving
   * public asset URLs from a common base URL.
   *
   * It is intended for application-level UI integration where pages need
   * predictable access to CSS, JavaScript, image and font assets.
   */
  class AssetManager
  {
  public:
    /**
     * @brief Construct an empty asset manager.
     */
    AssetManager() = default;

    /**
     * @brief Construct an asset manager with a base URL.
     *
     * @param base_url Public base URL for assets.
     */
    explicit AssetManager(std::string base_url);

    /**
     * @brief Create an asset manager with an optional base URL.
     *
     * @param base_url Public base URL for assets.
     * @return Asset manager.
     */
    [[nodiscard]] static AssetManager make(std::string base_url = {});

    /**
     * @brief Set the public base URL.
     *
     * @param base_url Public base URL for assets.
     * @return This asset manager.
     */
    AssetManager &set_base_url(std::string base_url);

    /**
     * @brief Get the public base URL.
     *
     * @return Base URL.
     */
    [[nodiscard]] const std::string &base_url() const noexcept;

    /**
     * @brief Resolve an asset path against the base URL.
     *
     * If the path is already absolute or the base URL is empty, the path
     * is returned unchanged.
     *
     * @param path Asset path.
     * @return Resolved asset URL.
     */
    [[nodiscard]] std::string resolve(std::string_view path) const;

    /**
     * @brief Add or replace a named asset.
     *
     * @param name Logical asset name.
     * @param asset Asset descriptor.
     * @return This asset manager.
     *
     * @throws AssetError if the name or asset URL is empty.
     */
    AssetManager &add(std::string name, Asset asset);

    /**
     * @brief Add or replace a named stylesheet asset.
     *
     * @param name Logical asset name.
     * @param href Stylesheet path or URL.
     * @param media CSS media attribute.
     * @return This asset manager.
     */
    AssetManager &add_stylesheet(
        std::string name,
        std::string href,
        std::string media = "all");

    /**
     * @brief Add or replace a named script asset.
     *
     * @param name Logical asset name.
     * @param src Script path or URL.
     * @param loading Script loading behavior.
     * @return This asset manager.
     */
    AssetManager &add_script(
        std::string name,
        std::string src,
        AssetLoading loading = AssetLoading::Default);

    /**
     * @brief Add or replace a named image asset.
     *
     * @param name Logical asset name.
     * @param src Image path or URL.
     * @param alt Alternative text.
     * @return This asset manager.
     */
    AssetManager &add_image(
        std::string name,
        std::string src,
        std::string alt = {});

    /**
     * @brief Add or replace a named font asset.
     *
     * @param name Logical asset name.
     * @param href Font path or URL.
     * @return This asset manager.
     */
    AssetManager &add_font(
        std::string name,
        std::string href);

    /**
     * @brief Check whether a named asset exists.
     *
     * @param name Logical asset name.
     * @return True if the asset exists.
     */
    [[nodiscard]] bool has(const std::string &name) const;

    /**
     * @brief Get a named asset.
     *
     * @param name Logical asset name.
     * @return Pointer to the asset if found, otherwise nullptr.
     */
    [[nodiscard]] const Asset *get(const std::string &name) const noexcept;

    /**
     * @brief Get a mutable named asset.
     *
     * @param name Logical asset name.
     * @return Pointer to the asset if found, otherwise nullptr.
     */
    [[nodiscard]] Asset *get(const std::string &name) noexcept;

    /**
     * @brief Remove a named asset.
     *
     * @param name Logical asset name.
     * @return True if an asset was removed.
     */
    bool remove(const std::string &name);

    /**
     * @brief Remove all assets.
     */
    void clear() noexcept;

    /**
     * @brief Check whether the manager is empty.
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
     * @brief Access all named assets.
     *
     * @return Immutable asset map.
     */
    [[nodiscard]] const std::map<std::string, Asset> &assets() const noexcept;

    /**
     * @brief Access all named assets.
     *
     * @return Mutable asset map.
     */
    [[nodiscard]] std::map<std::string, Asset> &assets() noexcept;

    /**
     * @brief Build an asset manifest from all stored assets.
     *
     * @return Asset manifest.
     */
    [[nodiscard]] AssetManifest manifest() const;

    /**
     * @brief Render all stored assets as HTML tags.
     *
     * @return Rendered HTML asset tags separated by newlines.
     */
    [[nodiscard]] std::string render() const;

    /**
     * @brief Render only assets of a specific type.
     *
     * @param type Asset type.
     * @return Rendered HTML asset tags separated by newlines.
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
     * @brief Public base URL for assets.
     */
    std::string base_url_;

    /**
     * @brief Asset insertion order.
     */
    std::vector<std::string> order_;

    /**
     * @brief Named assets.
     */
    std::map<std::string, Asset> assets_;
  };

} // namespace vix::ui

#endif // VIX_UI_ASSETS_ASSET_MANAGER_HPP
