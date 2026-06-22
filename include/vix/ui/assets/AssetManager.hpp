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
#include <vix/ui/assets/AssetMap.hpp>
#include <vix/ui/assets/AssetMode.hpp>

namespace vix::ui
{
  /**
   * @brief Manager for named UI assets.
   *
   * AssetManager stores named assets and provides helpers for resolving
   * public asset URLs from a common base URL.
   *
   * It supports development and production asset resolution:
   * - stable development paths
   * - optional version query strings
   * - optional manifest-style hashed path lookup through AssetMap
   * - grouped CSS and JavaScript rendering
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
     * @brief Set the asset resolution mode.
     *
     * @param mode Asset resolution mode.
     * @return This asset manager.
     */
    AssetManager &set_mode(AssetMode mode) noexcept;

    /**
     * @brief Set the asset version used for production versioned URLs.
     *
     * The version is appended as a query string only in production mode
     * when no AssetMap mapping exists for the requested path.
     *
     * Example:
     *
     * @code
     * /assets/css/app.css?v=0.7.0
     * @endcode
     *
     * @param version Version string.
     * @return This asset manager.
     */
    AssetManager &set_version(std::string version);

    /**
     * @brief Set the production asset map.
     *
     * The asset map is used to resolve logical paths to hashed or
     * build-generated paths.
     *
     * @param asset_map Asset map.
     * @return This asset manager.
     */
    AssetManager &set_asset_map(AssetMap asset_map);

    /**
     * @brief Set the production asset map.
     *
     * This is an alias for set_asset_map().
     *
     * @param asset_map Asset map.
     * @return This asset manager.
     */
    AssetManager &set_manifest(AssetMap asset_map);

    /**
     * @brief Remove all asset map entries.
     */
    void clear_asset_map() noexcept;

    /**
     * @brief Get the public base URL.
     *
     * @return Base URL.
     */
    [[nodiscard]] const std::string &base_url() const noexcept;

    /**
     * @brief Get the asset resolution mode.
     *
     * @return Asset mode.
     */
    [[nodiscard]] AssetMode mode() const noexcept;

    /**
     * @brief Get the configured asset version.
     *
     * @return Version string.
     */
    [[nodiscard]] const std::string &version() const noexcept;

    /**
     * @brief Access the production asset map.
     *
     * @return Immutable asset map.
     */
    [[nodiscard]] const AssetMap &asset_map() const noexcept;

    /**
     * @brief Access the production asset map.
     *
     * @return Mutable asset map.
     */
    [[nodiscard]] AssetMap &asset_map() noexcept;

    /**
     * @brief Check whether a version is configured.
     *
     * @return True if version is not empty.
     */
    [[nodiscard]] bool has_version() const noexcept;

    /**
     * @brief Check whether the asset map contains entries.
     *
     * @return True if the asset map is not empty.
     */
    [[nodiscard]] bool has_asset_map() const noexcept;

    /**
     * @brief Resolve an asset path against the base URL.
     *
     * This helper only joins the path with base_url().
     *
     * If the path is already absolute or the base URL is empty, the path
     * is returned unchanged.
     *
     * @param path Asset path.
     * @return Resolved asset URL.
     */
    [[nodiscard]] std::string resolve(std::string_view path) const;

    /**
     * @brief Resolve an asset path using the current asset mode.
     *
     * Development mode returns the path resolved against the base URL.
     *
     * Production mode first checks AssetMap. If a mapping exists, the mapped
     * path is resolved against the base URL. If no mapping exists and a
     * version is configured, the version is appended as a query string.
     *
     * @param path Asset path.
     * @return Resolved public asset URL.
     */
    [[nodiscard]] std::string path(std::string_view path) const;

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
     * @brief Add or replace a named module script asset.
     *
     * @param name Logical asset name.
     * @param src Script module path or URL.
     * @return This asset manager.
     */
    AssetManager &add_module_script(
        std::string name,
        std::string src);

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
     * @brief Add or replace a named asset and attach it to a group.
     *
     * @param group Logical group name.
     * @param name Logical asset name.
     * @param asset Asset descriptor.
     * @return This asset manager.
     *
     * @throws AssetError if group, name or asset URL is empty.
     */
    AssetManager &add_to_group(
        std::string group,
        std::string name,
        Asset asset);

    /**
     * @brief Add or replace a stylesheet asset in a group.
     *
     * @param group Logical group name.
     * @param name Logical asset name.
     * @param href Stylesheet path or URL.
     * @param media CSS media attribute.
     * @return This asset manager.
     */
    AssetManager &add_stylesheet_to(
        std::string group,
        std::string name,
        std::string href,
        std::string media = "all");

    /**
     * @brief Add or replace a script asset in a group.
     *
     * @param group Logical group name.
     * @param name Logical asset name.
     * @param src Script path or URL.
     * @param loading Script loading behavior.
     * @return This asset manager.
     */
    AssetManager &add_script_to(
        std::string group,
        std::string name,
        std::string src,
        AssetLoading loading = AssetLoading::Default);

    /**
     * @brief Add or replace a module script asset in a group.
     *
     * @param group Logical group name.
     * @param name Logical asset name.
     * @param src Script module path or URL.
     * @return This asset manager.
     */
    AssetManager &add_module_script_to(
        std::string group,
        std::string name,
        std::string src);

    /**
     * @brief Check whether a named asset exists.
     *
     * @param name Logical asset name.
     * @return True if the asset exists.
     */
    [[nodiscard]] bool has(const std::string &name) const;

    /**
     * @brief Check whether an asset group exists.
     *
     * @param group Logical group name.
     * @return True if the group exists.
     */
    [[nodiscard]] bool has_group(const std::string &group) const;

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
     * @brief Get asset names stored in a group.
     *
     * @param group Logical group name.
     * @return Pointer to grouped asset names if found, otherwise nullptr.
     */
    [[nodiscard]] const std::vector<std::string> *group(
        const std::string &group) const noexcept;

    /**
     * @brief Remove a named asset.
     *
     * The asset is also removed from all groups.
     *
     * @param name Logical asset name.
     * @return True if an asset was removed.
     */
    bool remove(const std::string &name);

    /**
     * @brief Remove an asset group.
     *
     * Assets remain registered. Only the group entry is removed.
     *
     * @param group Logical group name.
     * @return True if a group was removed.
     */
    bool remove_group(const std::string &group);

    /**
     * @brief Remove all assets and groups.
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
     * @brief Access all asset groups.
     *
     * @return Immutable group map.
     */
    [[nodiscard]] const std::map<std::string, std::vector<std::string>> &
    groups() const noexcept;

    /**
     * @brief Access all asset groups.
     *
     * @return Mutable group map.
     */
    [[nodiscard]] std::map<std::string, std::vector<std::string>> &
    groups() noexcept;

    /**
     * @brief Build an asset manifest from all stored assets.
     *
     * @return Asset manifest.
     */
    [[nodiscard]] AssetManifest manifest() const;

    /**
     * @brief Build an asset manifest from one group.
     *
     * @param group Logical group name.
     * @return Asset manifest containing assets from the group.
     */
    [[nodiscard]] AssetManifest manifest(std::string_view group) const;

    /**
     * @brief Render all stored assets as HTML tags.
     *
     * @return Rendered HTML asset tags separated by newlines.
     */
    [[nodiscard]] std::string render() const;

    /**
     * @brief Render one asset group as HTML tags.
     *
     * @param group Logical group name.
     * @return Rendered HTML asset tags separated by newlines.
     */
    [[nodiscard]] std::string render_group(std::string_view group) const;

    /**
     * @brief Render only assets of a specific type.
     *
     * @param type Asset type.
     * @return Rendered HTML asset tags separated by newlines.
     */
    [[nodiscard]] std::string render_type(AssetType type) const;

    /**
     * @brief Render only assets of a specific type from a group.
     *
     * @param group Logical group name.
     * @param type Asset type.
     * @return Rendered HTML asset tags separated by newlines.
     */
    [[nodiscard]] std::string render_type(
        std::string_view group,
        AssetType type) const;

    /**
     * @brief Render stylesheet assets.
     *
     * @return Rendered stylesheet tags.
     */
    [[nodiscard]] std::string render_stylesheets() const;

    /**
     * @brief Render stylesheet assets from a group.
     *
     * @param group Logical group name.
     * @return Rendered stylesheet tags.
     */
    [[nodiscard]] std::string render_stylesheets(
        std::string_view group) const;

    /**
     * @brief Render script assets.
     *
     * @return Rendered script tags.
     */
    [[nodiscard]] std::string render_scripts() const;

    /**
     * @brief Render script assets from a group.
     *
     * @param group Logical group name.
     * @return Rendered script tags.
     */
    [[nodiscard]] std::string render_scripts(
        std::string_view group) const;

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
     * @brief Asset resolution mode.
     */
    AssetMode mode_{AssetMode::Development};

    /**
     * @brief Optional asset version for production versioned URLs.
     */
    std::string version_;

    /**
     * @brief Optional production asset map.
     */
    AssetMap asset_map_;

    /**
     * @brief Asset insertion order.
     */
    std::vector<std::string> order_;

    /**
     * @brief Named assets.
     */
    std::map<std::string, Asset> assets_;

    /**
     * @brief Asset groups.
     */
    std::map<std::string, std::vector<std::string>> groups_;
  };

} // namespace vix::ui

#endif // VIX_UI_ASSETS_ASSET_MANAGER_HPP
