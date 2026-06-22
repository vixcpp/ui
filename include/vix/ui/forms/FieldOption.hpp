/**
 *
 *  @file FieldOption.hpp
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
#ifndef VIX_UI_FORMS_FIELD_OPTION_HPP
#define VIX_UI_FORMS_FIELD_OPTION_HPP

#include <string>

#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Option descriptor for select and radio fields.
   *
   * FieldOption stores the data needed to render one selectable option:
   * - value
   * - label
   * - selected state
   * - disabled state
   * - custom HTML attributes
   *
   * It is used by richer form fields such as select lists, radio groups,
   * checkbox groups, and other server-rendered choice inputs.
   */
  class FieldOption
  {
  public:
    /**
     * @brief Construct an empty option.
     */
    FieldOption() = default;

    /**
     * @brief Construct an option from a value.
     *
     * @param value Option value.
     */
    explicit FieldOption(std::string value);

    /**
     * @brief Construct an option from a value and label.
     *
     * @param value Option value.
     * @param label Human-readable option label.
     */
    FieldOption(std::string value, std::string label);

    /**
     * @brief Create an option.
     *
     * @param value Option value.
     * @param label Human-readable option label.
     * @return Field option.
     */
    [[nodiscard]] static FieldOption make(
        std::string value = {},
        std::string label = {});

    /**
     * @brief Set the option value.
     *
     * @param value Option value.
     * @return This option.
     */
    FieldOption &set_value(std::string value);

    /**
     * @brief Set the option label.
     *
     * @param label Human-readable option label.
     * @return This option.
     */
    FieldOption &set_label(std::string label);

    /**
     * @brief Set whether this option is selected.
     *
     * @param selected Selected flag.
     * @return This option.
     */
    FieldOption &set_selected(bool selected = true) noexcept;

    /**
     * @brief Set whether this option is disabled.
     *
     * @param disabled Disabled flag.
     * @return This option.
     */
    FieldOption &set_disabled(bool disabled = true) noexcept;

    /**
     * @brief Set or replace a custom HTML attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This option.
     */
    FieldOption &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean HTML attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This option.
     */
    FieldOption &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Get the option value.
     *
     * @return Option value.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Get the option label.
     *
     * @return Option label.
     */
    [[nodiscard]] const std::string &label() const noexcept;

    /**
     * @brief Check whether this option is selected.
     *
     * @return True if selected.
     */
    [[nodiscard]] bool selected() const noexcept;

    /**
     * @brief Check whether this option is disabled.
     *
     * @return True if disabled.
     */
    [[nodiscard]] bool disabled() const noexcept;

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
     * @brief Check whether the option has a value.
     *
     * @return True if value is not empty.
     */
    [[nodiscard]] bool has_value() const noexcept;

    /**
     * @brief Check whether the option has a label.
     *
     * @return True if label is not empty.
     */
    [[nodiscard]] bool has_label() const noexcept;

    /**
     * @brief Resolve the display label.
     *
     * If label is empty, value is used as the display label.
     *
     * @return Display label.
     */
    [[nodiscard]] std::string display_label() const;

    /**
     * @brief Render this option as an HTML option element.
     *
     * @return Rendered option HTML.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Option value.
     */
    std::string value_;

    /**
     * @brief Human-readable option label.
     */
    std::string label_;

    /**
     * @brief Whether this option is selected.
     */
    bool selected_{false};

    /**
     * @brief Whether this option is disabled.
     */
    bool disabled_{false};

    /**
     * @brief Custom HTML attributes.
     */
    HtmlAttrs attrs_;
  };

} // namespace vix::ui

#endif // VIX_UI_FORMS_FIELD_OPTION_HPP
