/**
 *
 *  @file Field.hpp
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
#ifndef VIX_UI_FORMS_FIELD_HPP
#define VIX_UI_FORMS_FIELD_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include <vix/ui/forms/FieldOption.hpp>
#include <vix/ui/forms/ValidationError.hpp>
#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Supported UI form field types.
   */
  enum class FieldType
  {
    Text = 0,
    Email,
    Password,
    Number,
    Hidden,
    Checkbox,
    Radio,
    Textarea,
    Select,
    File,
    Custom
  };

  /**
   * @brief Convert a field type to a stable string.
   *
   * @param type Field type.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(FieldType type) noexcept;

  /**
   * @brief UI form field descriptor.
   *
   * Field stores the data and validation state for a single form input.
   * It is designed for server-rendered forms and can render basic HTML.
   *
   * v0.6.0 extends Field with:
   * - select options
   * - radio option groups
   * - checkbox checked state
   * - multiple select/file support
   * - file accept helper
   */
  class Field
  {
  public:
    /**
     * @brief Construct an empty field.
     */
    Field() = default;

    /**
     * @brief Construct a field with a name and type.
     *
     * @param name Field name.
     * @param type Field type.
     */
    explicit Field(
        std::string name,
        FieldType type = FieldType::Text);

    /**
     * @brief Create a text field.
     *
     * @param name Field name.
     * @return Text field.
     */
    [[nodiscard]] static Field text(std::string name);

    /**
     * @brief Create an email field.
     *
     * @param name Field name.
     * @return Email field.
     */
    [[nodiscard]] static Field email(std::string name);

    /**
     * @brief Create a password field.
     *
     * @param name Field name.
     * @return Password field.
     */
    [[nodiscard]] static Field password(std::string name);

    /**
     * @brief Create a number field.
     *
     * @param name Field name.
     * @return Number field.
     */
    [[nodiscard]] static Field number(std::string name);

    /**
     * @brief Create a hidden field.
     *
     * @param name Field name.
     * @param value Field value.
     * @return Hidden field.
     */
    [[nodiscard]] static Field hidden(
        std::string name,
        std::string value = {});

    /**
     * @brief Create a checkbox field.
     *
     * @param name Field name.
     * @return Checkbox field.
     */
    [[nodiscard]] static Field checkbox(std::string name);

    /**
     * @brief Create a radio field.
     *
     * @param name Field name.
     * @return Radio field.
     */
    [[nodiscard]] static Field radio(std::string name);

    /**
     * @brief Create a textarea field.
     *
     * @param name Field name.
     * @return Textarea field.
     */
    [[nodiscard]] static Field textarea(std::string name);

    /**
     * @brief Create a select field.
     *
     * @param name Field name.
     * @return Select field.
     */
    [[nodiscard]] static Field select(std::string name);

    /**
     * @brief Create a file field.
     *
     * @param name Field name.
     * @return File field.
     */
    [[nodiscard]] static Field file(std::string name);

    /**
     * @brief Set the field name.
     *
     * @param name Field name.
     * @return This field.
     */
    Field &set_name(std::string name);

    /**
     * @brief Set the field type.
     *
     * @param type Field type.
     * @return This field.
     */
    Field &set_type(FieldType type) noexcept;

    /**
     * @brief Set the field value.
     *
     * @param value Field value.
     * @return This field.
     */
    Field &set_value(std::string value);

    /**
     * @brief Set the field label.
     *
     * @param label Field label.
     * @return This field.
     */
    Field &set_label(std::string label);

    /**
     * @brief Set the field placeholder.
     *
     * @param placeholder Placeholder text.
     * @return This field.
     */
    Field &set_placeholder(std::string placeholder);

    /**
     * @brief Set whether the field is required.
     *
     * @param required Required flag.
     * @return This field.
     */
    Field &set_required(bool required = true) noexcept;

    /**
     * @brief Set whether the field is disabled.
     *
     * @param disabled Disabled flag.
     * @return This field.
     */
    Field &set_disabled(bool disabled = true) noexcept;

    /**
     * @brief Set whether the field is readonly.
     *
     * @param readonly Readonly flag.
     * @return This field.
     */
    Field &set_readonly(bool readonly = true) noexcept;

    /**
     * @brief Set whether checkbox/radio field is checked.
     *
     * @param checked Checked flag.
     * @return This field.
     */
    Field &set_checked(bool checked = true) noexcept;

    /**
     * @brief Set whether select/file field accepts multiple values.
     *
     * @param multiple Multiple flag.
     * @return This field.
     */
    Field &set_multiple(bool multiple = true) noexcept;

    /**
     * @brief Set file accept attribute.
     *
     * Primarily used for file inputs.
     *
     * @param accept Accept value.
     * @return This field.
     */
    Field &set_accept(std::string accept);

    /**
     * @brief Set or replace a custom HTML attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This field.
     */
    Field &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean HTML attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This field.
     */
    Field &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Add a selectable option.
     *
     * @param option Field option.
     * @return This field.
     */
    Field &add_option(FieldOption option);

    /**
     * @brief Add a selectable option from value and label.
     *
     * @param value Option value.
     * @param label Option label.
     * @return This field.
     */
    Field &add_option(std::string value, std::string label = {});

    /**
     * @brief Replace all field options.
     *
     * @param options New options.
     * @return This field.
     */
    Field &set_options(std::vector<FieldOption> options);

    /**
     * @brief Remove all field options.
     */
    void clear_options() noexcept;

    /**
     * @brief Add a validation error.
     *
     * @param error Validation error.
     * @return This field.
     */
    Field &add_error(ValidationError error);

    /**
     * @brief Add a custom validation error message for this field.
     *
     * @param message Error message.
     * @return This field.
     */
    Field &add_error(std::string message);

    /**
     * @brief Remove all validation errors.
     */
    void clear_errors() noexcept;

    /**
     * @brief Get the field name.
     *
     * @return Field name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Get the field type.
     *
     * @return Field type.
     */
    [[nodiscard]] FieldType type() const noexcept;

    /**
     * @brief Get the field value.
     *
     * @return Field value.
     */
    [[nodiscard]] const std::string &value() const noexcept;

    /**
     * @brief Get the field label.
     *
     * @return Field label.
     */
    [[nodiscard]] const std::string &label() const noexcept;

    /**
     * @brief Get the field placeholder.
     *
     * @return Placeholder text.
     */
    [[nodiscard]] const std::string &placeholder() const noexcept;

    /**
     * @brief Check whether the field is required.
     *
     * @return True if required.
     */
    [[nodiscard]] bool required() const noexcept;

    /**
     * @brief Check whether the field is disabled.
     *
     * @return True if disabled.
     */
    [[nodiscard]] bool disabled() const noexcept;

    /**
     * @brief Check whether the field is readonly.
     *
     * @return True if readonly.
     */
    [[nodiscard]] bool readonly() const noexcept;

    /**
     * @brief Check whether the field is checked.
     *
     * @return True if checked.
     */
    [[nodiscard]] bool checked() const noexcept;

    /**
     * @brief Check whether the field accepts multiple values.
     *
     * @return True if multiple.
     */
    [[nodiscard]] bool multiple() const noexcept;

    /**
     * @brief Get the file accept value.
     *
     * @return Accept value.
     */
    [[nodiscard]] const std::string &accept() const noexcept;

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
     * @brief Access selectable options.
     *
     * @return Immutable options.
     */
    [[nodiscard]] const std::vector<FieldOption> &options() const noexcept;

    /**
     * @brief Access selectable options.
     *
     * @return Mutable options.
     */
    [[nodiscard]] std::vector<FieldOption> &options() noexcept;

    /**
     * @brief Access validation errors.
     *
     * @return Immutable validation errors.
     */
    [[nodiscard]] const std::vector<ValidationError> &errors() const noexcept;

    /**
     * @brief Check whether the field has a name.
     *
     * @return True if name is not empty.
     */
    [[nodiscard]] bool has_name() const noexcept;

    /**
     * @brief Check whether the field has a value.
     *
     * @return True if value is not empty.
     */
    [[nodiscard]] bool has_value() const noexcept;

    /**
     * @brief Check whether the field has a label.
     *
     * @return True if label is not empty.
     */
    [[nodiscard]] bool has_label() const noexcept;

    /**
     * @brief Check whether the field has a placeholder.
     *
     * @return True if placeholder is not empty.
     */
    [[nodiscard]] bool has_placeholder() const noexcept;

    /**
     * @brief Check whether the field has an accept value.
     *
     * @return True if accept is not empty.
     */
    [[nodiscard]] bool has_accept() const noexcept;

    /**
     * @brief Check whether the field has selectable options.
     *
     * @return True if options are present.
     */
    [[nodiscard]] bool has_options() const noexcept;

    /**
     * @brief Check whether the field has validation errors.
     *
     * @return True if errors are present.
     */
    [[nodiscard]] bool has_errors() const noexcept;

    /**
     * @brief Check whether the field is valid.
     *
     * @return True if no validation errors are present.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Check whether the field is invalid.
     *
     * @return True if validation errors are present.
     */
    [[nodiscard]] bool invalid() const noexcept;

    /**
     * @brief Get the number of selectable options.
     *
     * @return Option count.
     */
    [[nodiscard]] std::size_t option_count() const noexcept;

    /**
     * @brief Get the number of validation errors.
     *
     * @return Error count.
     */
    [[nodiscard]] std::size_t error_count() const noexcept;

    /**
     * @brief Render the field as basic HTML.
     *
     * @return Rendered HTML field.
     *
     * @throws FormError if the field type cannot be rendered.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Field name.
     */
    std::string name_;

    /**
     * @brief Field type.
     */
    FieldType type_{FieldType::Text};

    /**
     * @brief Field value.
     */
    std::string value_;

    /**
     * @brief Field label.
     */
    std::string label_;

    /**
     * @brief Placeholder text.
     */
    std::string placeholder_;

    /**
     * @brief File accept value.
     */
    std::string accept_;

    /**
     * @brief Required flag.
     */
    bool required_{false};

    /**
     * @brief Disabled flag.
     */
    bool disabled_{false};

    /**
     * @brief Readonly flag.
     */
    bool readonly_{false};

    /**
     * @brief Checked flag for checkbox/radio fields.
     */
    bool checked_{false};

    /**
     * @brief Multiple flag for select/file fields.
     */
    bool multiple_{false};

    /**
     * @brief Custom HTML attributes.
     */
    HtmlAttrs attrs_;

    /**
     * @brief Selectable options for select/radio fields.
     */
    std::vector<FieldOption> options_;

    /**
     * @brief Validation errors.
     */
    std::vector<ValidationError> errors_;
  };

} // namespace vix::ui

#endif // VIX_UI_FORMS_FIELD_HPP
