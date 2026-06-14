/**
 *
 *  @file ViewContext.hpp
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
#ifndef VIX_UI_CORE_VIEW_CONTEXT_HPP
#define VIX_UI_CORE_VIEW_CONTEXT_HPP

#include <cstddef>
#include <string>

#include <vix/template/Context.hpp>
#include <vix/template/Value.hpp>

namespace vix::ui
{
  /**
   * @brief Runtime data context for a UI view.
   *
   * ViewContext stores named values that will be passed to the Vix
   * template engine when rendering a view.
   *
   * It is a small UI-focused wrapper around the template value model.
   * The goal is to make view data construction fluent while keeping full
   * compatibility with `vix::template_::Context`.
   */
  class ViewContext
  {
  public:
    /**
     * @brief Construct an empty view context.
     */
    ViewContext() = default;

    /**
     * @brief Construct a view context from template values.
     *
     * @param values Initial named values.
     */
    explicit ViewContext(vix::template_::Object values);

    /**
     * @brief Insert or replace a named value.
     *
     * @param key Variable name.
     * @param value Variable value.
     * @return This context.
     */
    ViewContext &set(
        std::string key,
        vix::template_::Value value);

    /**
     * @brief Check whether a key exists.
     *
     * @param key Variable name.
     * @return True if the key exists.
     */
    [[nodiscard]] bool has(const std::string &key) const;

    /**
     * @brief Retrieve a value by key.
     *
     * @param key Variable name.
     * @return Pointer to the value if found, otherwise nullptr.
     */
    [[nodiscard]] const vix::template_::Value *get(
        const std::string &key) const noexcept;

    /**
     * @brief Retrieve a mutable value by key.
     *
     * @param key Variable name.
     * @return Pointer to the value if found, otherwise nullptr.
     */
    [[nodiscard]] vix::template_::Value *get(
        const std::string &key) noexcept;

    /**
     * @brief Get a value by key or return a fallback.
     *
     * @param key Variable name.
     * @param fallback Fallback value.
     * @return Found value or fallback.
     */
    [[nodiscard]] vix::template_::Value get_or(
        const std::string &key,
        vix::template_::Value fallback) const;

    /**
     * @brief Remove a value from the context.
     *
     * @param key Variable name.
     * @return True if a value was removed.
     */
    bool erase(const std::string &key);

    /**
     * @brief Remove all values.
     */
    void clear() noexcept;

    /**
     * @brief Check whether the context is empty.
     *
     * @return True if no values are stored.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the number of stored values.
     *
     * @return Number of values.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Access the underlying values.
     *
     * @return Immutable values object.
     */
    [[nodiscard]] const vix::template_::Object &values() const noexcept;

    /**
     * @brief Access the underlying values.
     *
     * @return Mutable values object.
     */
    [[nodiscard]] vix::template_::Object &values() noexcept;

    /**
     * @brief Convert this view context to a template context.
     *
     * @return Template rendering context.
     */
    [[nodiscard]] vix::template_::Context to_template_context() const;

  private:
    /**
     * @brief Stored named values.
     */
    vix::template_::Object values_;
  };

} // namespace vix::ui

#endif // VIX_UI_CORE_VIEW_CONTEXT_HPP
