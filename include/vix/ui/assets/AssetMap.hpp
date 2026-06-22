/**
 *
 *  @file AssetMap.hpp
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
#ifndef VIX_UI_ASSETS_ASSET_MAP_HPP
#define VIX_UI_ASSETS_ASSET_MAP_HPP

#include <cstddef>
#include <map>
#include <string>
#include <string_view>

namespace vix::ui
{
  /**
   * @brief Logical asset path to resolved asset path map.
   *
   * AssetMap stores mappings usually produced by an external asset build
   * step.
   *
   * Example:
   *
   * @code
   * css/app.css -> css/app.8fd21c.css
   * js/app.js   -> js/app.91af03.js
   * @endcode
   *
   * It does not build, hash, minify, bundle, or serve files. It only keeps
   * a deterministic lookup table used by higher-level asset helpers.
   */
  class AssetMap
  {
  public:
    /**
     * @brief Construct an empty asset map.
     */
    AssetMap() = default;

    /**
     * @brief Construct an asset map from existing entries.
     *
     * @param entries Initial asset mappings.
     *
     * @throws AssetError if one mapping has an empty logical or resolved path.
     */
    explicit AssetMap(std::map<std::string, std::string> entries);

    /**
     * @brief Create an empty asset map.
     *
     * @return Empty asset map.
     */
    [[nodiscard]] static AssetMap make();

    /**
     * @brief Create an asset map from existing entries.
     *
     * @param entries Initial asset mappings.
     * @return Asset map.
     *
     * @throws AssetError if one mapping has an empty logical or resolved path.
     */
    [[nodiscard]] static AssetMap from(
        std::map<std::string, std::string> entries);

    /**
     * @brief Add or replace an asset path mapping.
     *
     * @param logical_path Original logical asset path.
     * @param resolved_path Resolved production asset path.
     * @return This asset map.
     *
     * @throws AssetError if logical_path or resolved_path is empty.
     */
    AssetMap &set(
        std::string logical_path,
        std::string resolved_path);

    /**
     * @brief Check whether a logical asset path exists.
     *
     * @param logical_path Original logical asset path.
     * @return True if a mapping exists.
     */
    [[nodiscard]] bool has(std::string_view logical_path) const noexcept;

    /**
     * @brief Get a resolved asset path.
     *
     * @param logical_path Original logical asset path.
     * @return Pointer to the resolved path if found, otherwise nullptr.
     */
    [[nodiscard]] const std::string *get(
        std::string_view logical_path) const noexcept;

    /**
     * @brief Get a resolved asset path or a fallback value.
     *
     * @param logical_path Original logical asset path.
     * @param fallback Fallback value returned when no mapping exists.
     * @return Resolved asset path or fallback.
     */
    [[nodiscard]] std::string get_or(
        std::string_view logical_path,
        std::string fallback = {}) const;

    /**
     * @brief Resolve an asset path through the map.
     *
     * If no mapping exists, the original path is returned unchanged.
     *
     * @param logical_path Original logical asset path.
     * @return Resolved path or original path.
     */
    [[nodiscard]] std::string resolve(
        std::string_view logical_path) const;

    /**
     * @brief Remove a mapping.
     *
     * @param logical_path Original logical asset path.
     * @return True if a mapping was removed.
     */
    bool remove(std::string_view logical_path);

    /**
     * @brief Remove all mappings.
     */
    void clear() noexcept;

    /**
     * @brief Check whether the asset map is empty.
     *
     * @return True if no mappings are stored.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the number of stored mappings.
     *
     * @return Mapping count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Access all mappings.
     *
     * @return Immutable mapping table.
     */
    [[nodiscard]] const std::map<std::string, std::string> &
    entries() const noexcept;

    /**
     * @brief Access all mappings.
     *
     * @return Mutable mapping table.
     */
    [[nodiscard]] std::map<std::string, std::string> &
    entries() noexcept;

  private:
    /**
     * @brief Stored logical asset path to resolved asset path mappings.
     */
    std::map<std::string, std::string> entries_;
  };

} // namespace vix::ui

#endif // VIX_UI_ASSETS_ASSET_MAP_HPP
