/**
 *
 *  @file ViewData.hpp
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
#ifndef VIX_UI_CORE_VIEW_DATA_HPP
#define VIX_UI_CORE_VIEW_DATA_HPP

#include <string>
#include <utility>

#include <vix/template/Value.hpp>

namespace vix::ui
{
  /**
   * @brief Plain data container for a UI view.
   *
   * ViewData stores the minimal information needed to describe a
   * server-rendered UI view:
   * - the template name to render
   * - an optional page title
   * - an optional layout name
   * - template values passed to the rendering layer
   *
   * It intentionally stays simple and does not perform rendering by itself.
   * Rendering is handled by higher-level view and response helpers.
   */
  struct ViewData
  {
    /**
     * @brief Template name used to render the view.
     *
     * Example:
     * - "home.html"
     * - "dashboard.html"
     * - "pages/profile.html"
     */
    std::string template_name;

    /**
     * @brief Optional layout name associated with the view.
     *
     * This field is metadata for higher-level UI helpers. The underlying
     * template engine still supports layout inheritance directly through
     * template syntax such as `{% extends "base.html" %}`.
     */
    std::string layout_name;

    /**
     * @brief Optional human-readable page title.
     *
     * Higher-level helpers may also copy this value into the rendering
     * context as `page_title`.
     */
    std::string title;

    /**
     * @brief Values passed to the template rendering context.
     *
     * This uses the same dynamic object type as the Vix template module so
     * UI views can render directly through `vix::template_::Context`.
     */
    vix::template_::Object values;

    /**
     * @brief Construct empty view data.
     */
    ViewData() = default;

    /**
     * @brief Construct view data with a template name.
     *
     * @param name Template name.
     */
    explicit ViewData(std::string name)
        : template_name(std::move(name))
    {
    }

    /**
     * @brief Check whether a template name is set.
     *
     * @return True if the template name is not empty.
     */
    [[nodiscard]] bool has_template() const noexcept
    {
      return !template_name.empty();
    }

    /**
     * @brief Check whether a layout name is set.
     *
     * @return True if the layout name is not empty.
     */
    [[nodiscard]] bool has_layout() const noexcept
    {
      return !layout_name.empty();
    }

    /**
     * @brief Check whether a page title is set.
     *
     * @return True if the title is not empty.
     */
    [[nodiscard]] bool has_title() const noexcept
    {
      return !title.empty();
    }

    /**
     * @brief Check whether the view contains template values.
     *
     * @return True if at least one value is stored.
     */
    [[nodiscard]] bool has_values() const noexcept
    {
      return !values.empty();
    }
  };

} // namespace vix::ui

#endif // VIX_UI_CORE_VIEW_DATA_HPP
