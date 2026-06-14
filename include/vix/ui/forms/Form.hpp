/**
 *
 *  @file Form.hpp
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
#ifndef VIX_UI_FORMS_FORM_HPP
#define VIX_UI_FORMS_FORM_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include <vix/ui/forms/Field.hpp>
#include <vix/ui/forms/ValidationError.hpp>
#include <vix/ui/html/HtmlAttrs.hpp>

namespace vix::ui
{
  /**
   * @brief Supported HTML form methods.
   */
  enum class FormMethod
  {
    Get = 0,
    Post
  };

  /**
   * @brief Convert a form method to a stable string.
   *
   * @param method Form method.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(FormMethod method) noexcept;

  /**
   * @brief Server-rendered UI form descriptor.
   *
   * Form stores:
   * - the form action URL
   * - the form method
   * - form fields
   * - validation errors
   * - custom HTML attributes
   *
   * It is designed for simple server-rendered forms and keeps validation
   * state explicit.
   */
  class Form
  {
  public:
    /**
     * @brief Construct an empty form.
     */
    Form() = default;

    /**
     * @brief Construct a form with an action and method.
     *
     * @param action Form action URL.
     * @param method Form method.
     */
    explicit Form(
        std::string action,
        FormMethod method = FormMethod::Post);

    /**
     * @brief Create a form.
     *
     * @param action Form action URL.
     * @param method Form method.
     * @return Form instance.
     */
    [[nodiscard]] static Form make(
        std::string action = {},
        FormMethod method = FormMethod::Post);

    /**
     * @brief Create a GET form.
     *
     * @param action Form action URL.
     * @return GET form.
     */
    [[nodiscard]] static Form get(std::string action = {});

    /**
     * @brief Create a POST form.
     *
     * @param action Form action URL.
     * @return POST form.
     */
    [[nodiscard]] static Form post(std::string action = {});

    /**
     * @brief Set the form action URL.
     *
     * @param action Form action URL.
     * @return This form.
     */
    Form &set_action(std::string action);

    /**
     * @brief Set the form method.
     *
     * @param method Form method.
     * @return This form.
     */
    Form &set_method(FormMethod method) noexcept;

    /**
     * @brief Set or replace a custom HTML attribute.
     *
     * @param name Attribute name.
     * @param value Attribute value.
     * @return This form.
     */
    Form &set_attr(std::string name, std::string value);

    /**
     * @brief Set or remove a custom boolean HTML attribute.
     *
     * @param name Attribute name.
     * @param enabled Whether the attribute should be present.
     * @return This form.
     */
    Form &set_bool_attr(std::string name, bool enabled = true);

    /**
     * @brief Add a field to the form.
     *
     * If a field with the same name already exists, it is replaced.
     *
     * @param field Field to add.
     * @return This form.
     */
    Form &add_field(Field field);

    /**
     * @brief Add a text field.
     *
     * @param name Field name.
     * @return This form.
     */
    Form &add_text(std::string name);

    /**
     * @brief Add an email field.
     *
     * @param name Field name.
     * @return This form.
     */
    Form &add_email(std::string name);

    /**
     * @brief Add a password field.
     *
     * @param name Field name.
     * @return This form.
     */
    Form &add_password(std::string name);

    /**
     * @brief Add a hidden field.
     *
     * @param name Field name.
     * @param value Field value.
     * @return This form.
     */
    Form &add_hidden(std::string name, std::string value = {});

    /**
     * @brief Add a checkbox field.
     *
     * @param name Field name.
     * @return This form.
     */
    Form &add_checkbox(std::string name);

    /**
     * @brief Add a textarea field.
     *
     * @param name Field name.
     * @return This form.
     */
    Form &add_textarea(std::string name);

    /**
     * @brief Check whether a field exists.
     *
     * @param name Field name.
     * @return True if present.
     */
    [[nodiscard]] bool has_field(std::string_view name) const noexcept;

    /**
     * @brief Find a field by name.
     *
     * @param name Field name.
     * @return Pointer to the field if found, otherwise nullptr.
     */
    [[nodiscard]] const Field *field(std::string_view name) const noexcept;

    /**
     * @brief Find a mutable field by name.
     *
     * @param name Field name.
     * @return Pointer to the field if found, otherwise nullptr.
     */
    [[nodiscard]] Field *field(std::string_view name) noexcept;

    /**
     * @brief Remove a field by name.
     *
     * @param name Field name.
     * @return True if a field was removed.
     */
    bool remove_field(std::string_view name);

    /**
     * @brief Remove all fields.
     */
    void clear_fields() noexcept;

    /**
     * @brief Add a validation error to the form.
     *
     * @param error Validation error.
     * @return This form.
     */
    Form &add_error(ValidationError error);

    /**
     * @brief Add a custom validation error for a field.
     *
     * @param field Field name.
     * @param message Error message.
     * @return This form.
     */
    Form &add_error(std::string field, std::string message);

    /**
     * @brief Remove all validation errors.
     */
    void clear_errors() noexcept;

    /**
     * @brief Get the form action URL.
     *
     * @return Form action.
     */
    [[nodiscard]] const std::string &action() const noexcept;

    /**
     * @brief Get the form method.
     *
     * @return Form method.
     */
    [[nodiscard]] FormMethod method() const noexcept;

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
     * @brief Access form fields.
     *
     * @return Immutable field list.
     */
    [[nodiscard]] const std::vector<Field> &fields() const noexcept;

    /**
     * @brief Access form fields.
     *
     * @return Mutable field list.
     */
    [[nodiscard]] std::vector<Field> &fields() noexcept;

    /**
     * @brief Access validation errors.
     *
     * @return Immutable validation errors.
     */
    [[nodiscard]] const std::vector<ValidationError> &errors() const noexcept;

    /**
     * @brief Check whether the form has an action URL.
     *
     * @return True if action is not empty.
     */
    [[nodiscard]] bool has_action() const noexcept;

    /**
     * @brief Check whether the form has fields.
     *
     * @return True if fields are present.
     */
    [[nodiscard]] bool has_fields() const noexcept;

    /**
     * @brief Check whether the form has validation errors.
     *
     * @return True if errors are present.
     */
    [[nodiscard]] bool has_errors() const noexcept;

    /**
     * @brief Check whether the form has no fields.
     *
     * @return True if no fields are stored.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Get the number of fields.
     *
     * @return Field count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Get the number of validation errors.
     *
     * @return Error count.
     */
    [[nodiscard]] std::size_t error_count() const noexcept;

    /**
     * @brief Check whether the form is valid.
     *
     * @return True if no validation errors are present.
     */
    [[nodiscard]] bool valid() const noexcept;

    /**
     * @brief Check whether the form is invalid.
     *
     * @return True if validation errors are present.
     */
    [[nodiscard]] bool invalid() const noexcept;

    /**
     * @brief Render the opening form tag.
     *
     * @return Opening form tag.
     */
    [[nodiscard]] std::string render_open() const;

    /**
     * @brief Render the closing form tag.
     *
     * @return Closing form tag.
     */
    [[nodiscard]] std::string render_close() const;

    /**
     * @brief Render all fields.
     *
     * @return Rendered fields.
     */
    [[nodiscard]] std::string render_fields() const;

    /**
     * @brief Render all validation errors.
     *
     * @return Rendered validation errors.
     */
    [[nodiscard]] std::string render_errors() const;

    /**
     * @brief Render the full form.
     *
     * @return Rendered HTML form.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Form action URL.
     */
    std::string action_;

    /**
     * @brief Form method.
     */
    FormMethod method_{FormMethod::Post};

    /**
     * @brief Custom HTML attributes.
     */
    HtmlAttrs attrs_;

    /**
     * @brief Form fields in insertion order.
     */
    std::vector<Field> fields_;

    /**
     * @brief Form-level validation errors.
     */
    std::vector<ValidationError> errors_;
  };

} // namespace vix::ui

#endif // VIX_UI_FORMS_FORM_HPP
