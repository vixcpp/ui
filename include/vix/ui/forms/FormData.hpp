/**
 *
 *  @file FormData.hpp
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
#ifndef VIX_UI_FORMS_FORM_DATA_HPP
#define VIX_UI_FORMS_FORM_DATA_HPP

#include <cstddef>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace vix::ui
{
  /**
   * @brief Server-rendered form input data container.
   *
   * FormData stores old input values or submitted form values in a simple
   * transport-neutral structure.
   *
   * It supports:
   * - single-value fields
   * - multi-value fields such as checkbox groups
   * - old input lookup
   * - form binding helpers
   */
  class FormData
  {
  public:
    /**
     * @brief Construct an empty form data container.
     */
    FormData() = default;

    /**
     * @brief Construct form data from a string map.
     *
     * @param values Initial single-value fields.
     */
    explicit FormData(std::map<std::string, std::string> values);

    /**
     * @brief Create an empty form data container.
     *
     * @return Form data.
     */
    [[nodiscard]] static FormData make();

    /**
     * @brief Create form data from a string map.
     *
     * @param values Initial single-value fields.
     * @return Form data.
     */
    [[nodiscard]] static FormData from(
        std::map<std::string, std::string> values);

    /**
     * @brief Set or replace a single value.
     *
     * Existing values for the field are replaced.
     *
     * @param name Field name.
     * @param value Field value.
     * @return This form data.
     */
    FormData &set(std::string name, std::string value);

    /**
     * @brief Add a value to a multi-value field.
     *
     * @param name Field name.
     * @param value Field value.
     * @return This form data.
     */
    FormData &add(std::string name, std::string value);

    /**
     * @brief Set or replace multiple values for a field.
     *
     * @param name Field name.
     * @param values Field values.
     * @return This form data.
     */
    FormData &set_all(std::string name, std::vector<std::string> values);

    /**
     * @brief Check whether a field exists.
     *
     * @param name Field name.
     * @return True if present.
     */
    [[nodiscard]] bool has(std::string_view name) const noexcept;

    /**
     * @brief Check whether a field contains a specific value.
     *
     * This is useful for checkbox groups, radio groups, and select fields.
     *
     * @param name Field name.
     * @param value Expected value.
     * @return True if the field contains the value.
     */
    [[nodiscard]] bool contains(
        std::string_view name,
        std::string_view value) const noexcept;

    /**
     * @brief Get the first value for a field.
     *
     * @param name Field name.
     * @return Pointer to the first value if found, otherwise nullptr.
     */
    [[nodiscard]] const std::string *get(
        std::string_view name) const noexcept;

    /**
     * @brief Get the first value for a field or return a fallback.
     *
     * @param name Field name.
     * @param fallback Fallback value.
     * @return Field value or fallback.
     */
    [[nodiscard]] std::string get_or(
        std::string_view name,
        std::string fallback = {}) const;

    /**
     * @brief Get all values for a field.
     *
     * @param name Field name.
     * @return Pointer to values if found, otherwise nullptr.
     */
    [[nodiscard]] const std::vector<std::string> *get_all(
        std::string_view name) const noexcept;

    /**
     * @brief Get all values for a field or an empty list.
     *
     * @param name Field name.
     * @return Field values.
     */
    [[nodiscard]] std::vector<std::string> get_all_or_empty(
        std::string_view name) const;

    /**
     * @brief Remove a field and all its values.
     *
     * @param name Field name.
     * @return True if a field was removed.
     */
    bool erase(std::string_view name);

    /**
     * @brief Remove all stored values.
     */
    void clear() noexcept;

    /**
     * @brief Check whether no fields are stored.
     *
     * @return True if empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the number of stored fields.
     *
     * @return Field count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Get the number of values stored for a field.
     *
     * @param name Field name.
     * @return Value count.
     */
    [[nodiscard]] std::size_t value_count(
        std::string_view name) const noexcept;

    /**
     * @brief Access all stored values.
     *
     * @return Immutable values map.
     */
    [[nodiscard]] const std::map<std::string, std::vector<std::string>> &
    values() const noexcept;

    /**
     * @brief Access all stored values.
     *
     * @return Mutable values map.
     */
    [[nodiscard]] std::map<std::string, std::vector<std::string>> &
    values() noexcept;

  private:
    /**
     * @brief Stored form values indexed by field name.
     */
    std::map<std::string, std::vector<std::string>> values_;
  };

} // namespace vix::ui

#endif // VIX_UI_FORMS_FORM_DATA_HPP
