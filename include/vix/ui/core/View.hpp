/**
 *
 *  @file View.hpp
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
#ifndef VIX_UI_CORE_VIEW_HPP
#define VIX_UI_CORE_VIEW_HPP

#include <string>

#include <vix/template/Context.hpp>
#include <vix/template/Engine.hpp>
#include <vix/template/Value.hpp>
#include <vix/ui/core/ViewContext.hpp>
#include <vix/ui/core/ViewData.hpp>
#include <vix/ui/core/ViewResult.hpp>

namespace vix::ui
{
  /**
   * @brief Server-rendered UI view descriptor.
   *
   * View is a small high-level wrapper around Vix templates.
   * It stores:
   * - a template name
   * - optional view metadata such as layout and title
   * - named values passed to the template renderer
   *
   * View does not replace the template module. It builds on top of
   * `vix::template_::Engine`, `vix::template_::Context`, and
   * `vix::template_::Value` to provide a cleaner UI-facing API.
   */
  class View
  {
  public:
    /**
     * @brief Construct an empty view.
     */
    View() = default;

    /**
     * @brief Construct a view from a template name.
     *
     * @param template_name Template name to render.
     */
    explicit View(std::string template_name);

    /**
     * @brief Construct a view from view data.
     *
     * @param data Initial view data.
     */
    explicit View(ViewData data);

    /**
     * @brief Create a view from a template name.
     *
     * @param template_name Template name to render.
     * @return View instance.
     */
    [[nodiscard]] static View make(std::string template_name);

    /**
     * @brief Set the template name.
     *
     * @param template_name Template name to render.
     * @return This view.
     */
    View &set_template(std::string template_name);

    /**
     * @brief Set the layout name metadata.
     *
     * This does not directly change template inheritance. The template
     * engine still handles layouts through template syntax such as
     * `{% extends "base.html" %}`.
     *
     * @param layout_name Layout name.
     * @return This view.
     */
    View &set_layout(std::string layout_name);

    /**
     * @brief Set the view title metadata.
     *
     * This also stores the title in the template values as `page_title`.
     *
     * @param title View title.
     * @return This view.
     */
    View &set_title(std::string title);

    /**
     * @brief Insert or replace a template value.
     *
     * @param key Variable name.
     * @param value Variable value.
     * @return This view.
     */
    View &set(
        std::string key,
        vix::template_::Value value);

    /**
     * @brief Merge values into the view.
     *
     * Existing keys are replaced.
     *
     * @param values Values to merge.
     * @return This view.
     */
    View &merge(vix::template_::Object values);

    /**
     * @brief Check whether a template value exists.
     *
     * @param key Variable name.
     * @return True if the value exists.
     */
    [[nodiscard]] bool has(const std::string &key) const;

    /**
     * @brief Retrieve a template value.
     *
     * @param key Variable name.
     * @return Pointer to the value if found, otherwise nullptr.
     */
    [[nodiscard]] const vix::template_::Value *get(
        const std::string &key) const noexcept;

    /**
     * @brief Retrieve a mutable template value.
     *
     * @param key Variable name.
     * @return Pointer to the value if found, otherwise nullptr.
     */
    [[nodiscard]] vix::template_::Value *get(
        const std::string &key) noexcept;

    /**
     * @brief Remove a template value.
     *
     * @param key Variable name.
     * @return True if a value was removed.
     */
    bool erase(const std::string &key);

    /**
     * @brief Remove all template values.
     */
    void clear_values() noexcept;

    /**
     * @brief Get the template name.
     *
     * @return Template name.
     */
    [[nodiscard]] const std::string &template_name() const noexcept;

    /**
     * @brief Get the layout name metadata.
     *
     * @return Layout name.
     */
    [[nodiscard]] const std::string &layout_name() const noexcept;

    /**
     * @brief Get the view title metadata.
     *
     * @return View title.
     */
    [[nodiscard]] const std::string &title() const noexcept;

    /**
     * @brief Check whether the view has a template name.
     *
     * @return True if a template name is set.
     */
    [[nodiscard]] bool has_template() const noexcept;

    /**
     * @brief Check whether the view has a layout name.
     *
     * @return True if a layout name is set.
     */
    [[nodiscard]] bool has_layout() const noexcept;

    /**
     * @brief Check whether the view has a title.
     *
     * @return True if a title is set.
     */
    [[nodiscard]] bool has_title() const noexcept;

    /**
     * @brief Access the underlying view data.
     *
     * @return Immutable view data.
     */
    [[nodiscard]] const ViewData &data() const noexcept;

    /**
     * @brief Access the underlying view data.
     *
     * @return Mutable view data.
     */
    [[nodiscard]] ViewData &data() noexcept;

    /**
     * @brief Access the template values.
     *
     * @return Immutable values object.
     */
    [[nodiscard]] const vix::template_::Object &values() const noexcept;

    /**
     * @brief Access the template values.
     *
     * @return Mutable values object.
     */
    [[nodiscard]] vix::template_::Object &values() noexcept;

    /**
     * @brief Build a UI view context from this view.
     *
     * @return View context.
     */
    [[nodiscard]] ViewContext context() const;

    /**
     * @brief Build a template rendering context from this view.
     *
     * @return Template context.
     */
    [[nodiscard]] vix::template_::Context to_template_context() const;

    /**
     * @brief Render the view through a template engine.
     *
     * @param engine Template engine.
     * @return View render result.
     *
     * @throws ViewError if no template name is set.
     */
    [[nodiscard]] ViewResult render(vix::template_::Engine &engine) const;

  private:
    /**
     * @brief Stored view data.
     */
    ViewData data_;
  };

} // namespace vix::ui

#endif // VIX_UI_CORE_VIEW_HPP
