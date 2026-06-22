/**
 *
 *  @file Field.cpp
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
#include <vix/ui/forms/Field.hpp>
#include <vix/ui/html/Html.hpp>
#include <vix/ui/support/Error.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    HtmlAttrs build_field_attrs(const Field &field)
    {
      HtmlAttrs attrs = field.attrs();

      if (field.has_name())
      {
        attrs.set("name", field.name());
        attrs.set("id", field.name());
      }

      if (field.has_placeholder())
      {
        attrs.set("placeholder", field.placeholder());
      }

      if (field.required())
      {
        attrs.boolean("required", true);
      }

      if (field.disabled())
      {
        attrs.boolean("disabled", true);
      }

      if (field.readonly())
      {
        attrs.boolean("readonly", true);
      }

      return attrs;
    }

    std::string render_options(const Field &field)
    {
      std::string output;

      for (const auto &option : field.options())
      {
        FieldOption current = option;

        if (!current.selected() &&
            field.has_value() &&
            current.value() == field.value())
        {
          current.set_selected(true);
        }

        output += current.render();
      }

      return output;
    }

    std::string safe_option_id(
        std::string_view field_name,
        std::string_view option_value)
    {
      std::string output;

      output.reserve(field_name.size() + option_value.size() + 2);

      output.append(field_name.data(), field_name.size());
      output.push_back('_');

      for (char ch : option_value)
      {
        if ((ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9') ||
            ch == '_' ||
            ch == '-')
        {
          output.push_back(ch);
        }
        else
        {
          output.push_back('_');
        }
      }

      return output;
    }

    std::string render_radio_group(const Field &field)
    {
      std::string output;

      for (const auto &option : field.options())
      {
        HtmlAttrs input_attrs = option.attrs();

        input_attrs.set("type", "radio");
        input_attrs.set("name", field.name());
        input_attrs.set("value", option.value());

        const std::string id = safe_option_id(field.name(), option.value());
        input_attrs.set("id", id);

        const bool selected =
            option.selected() ||
            (field.has_value() && option.value() == field.value());

        input_attrs.boolean("checked", selected);
        input_attrs.boolean("disabled", option.disabled() || field.disabled());
        input_attrs.boolean("required", field.required());

        std::string content;

        content += Html::void_tag("input", input_attrs);

        HtmlAttrs label_attrs;
        label_attrs.set("for", id);

        content += Html::tag(
            "label",
            Html::text(option.display_label()),
            label_attrs);

        HtmlAttrs wrapper_attrs;
        wrapper_attrs.set("class", "radio-option");

        output += Html::tag("div", content, wrapper_attrs);
      }

      HtmlAttrs group_attrs;
      group_attrs.set("class", "radio-group");

      if (field.has_name())
      {
        group_attrs.set("data-field", field.name());
      }

      return Html::tag("div", output, group_attrs);
    }
  } // namespace

  std::string_view to_string(FieldType type) noexcept
  {
    switch (type)
    {
    case FieldType::Text:
      return "text";

    case FieldType::Email:
      return "email";

    case FieldType::Password:
      return "password";

    case FieldType::Number:
      return "number";

    case FieldType::Hidden:
      return "hidden";

    case FieldType::Checkbox:
      return "checkbox";

    case FieldType::Radio:
      return "radio";

    case FieldType::Textarea:
      return "textarea";

    case FieldType::Select:
      return "select";

    case FieldType::File:
      return "file";

    case FieldType::Custom:
      return "custom";
    }

    return "custom";
  }

  Field::Field(
      std::string name,
      FieldType type)
      : name_(std::move(name)),
        type_(type)
  {
  }

  Field Field::text(std::string name)
  {
    return Field(std::move(name), FieldType::Text);
  }

  Field Field::email(std::string name)
  {
    return Field(std::move(name), FieldType::Email);
  }

  Field Field::password(std::string name)
  {
    return Field(std::move(name), FieldType::Password);
  }

  Field Field::number(std::string name)
  {
    return Field(std::move(name), FieldType::Number);
  }

  Field Field::hidden(
      std::string name,
      std::string value)
  {
    Field field(std::move(name), FieldType::Hidden);
    field.set_value(std::move(value));
    return field;
  }

  Field Field::checkbox(std::string name)
  {
    return Field(std::move(name), FieldType::Checkbox);
  }

  Field Field::radio(std::string name)
  {
    return Field(std::move(name), FieldType::Radio);
  }

  Field Field::textarea(std::string name)
  {
    return Field(std::move(name), FieldType::Textarea);
  }

  Field Field::select(std::string name)
  {
    return Field(std::move(name), FieldType::Select);
  }

  Field Field::file(std::string name)
  {
    return Field(std::move(name), FieldType::File);
  }

  Field &Field::set_name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  Field &Field::set_type(FieldType type) noexcept
  {
    type_ = type;
    return *this;
  }

  Field &Field::set_value(std::string value)
  {
    value_ = std::move(value);
    return *this;
  }

  Field &Field::set_label(std::string label)
  {
    label_ = std::move(label);
    return *this;
  }

  Field &Field::set_placeholder(std::string placeholder)
  {
    placeholder_ = std::move(placeholder);
    return *this;
  }

  Field &Field::set_required(bool required) noexcept
  {
    required_ = required;
    return *this;
  }

  Field &Field::set_disabled(bool disabled) noexcept
  {
    disabled_ = disabled;
    return *this;
  }

  Field &Field::set_readonly(bool readonly) noexcept
  {
    readonly_ = readonly;
    return *this;
  }

  Field &Field::set_checked(bool checked) noexcept
  {
    checked_ = checked;
    return *this;
  }

  Field &Field::set_multiple(bool multiple) noexcept
  {
    multiple_ = multiple;
    return *this;
  }

  Field &Field::set_accept(std::string accept)
  {
    accept_ = std::move(accept);
    return *this;
  }

  Field &Field::set_attr(std::string name, std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  Field &Field::set_bool_attr(std::string name, bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  Field &Field::add_option(FieldOption option)
  {
    options_.push_back(std::move(option));
    return *this;
  }

  Field &Field::add_option(std::string value, std::string label)
  {
    options_.push_back(
        FieldOption::make(std::move(value), std::move(label)));

    return *this;
  }

  Field &Field::set_options(std::vector<FieldOption> options)
  {
    options_ = std::move(options);
    return *this;
  }

  void Field::clear_options() noexcept
  {
    options_.clear();
  }

  Field &Field::add_error(ValidationError error)
  {
    errors_.push_back(std::move(error));
    return *this;
  }

  Field &Field::add_error(std::string message)
  {
    errors_.push_back(
        ValidationError::custom(name_, std::move(message)));

    return *this;
  }

  void Field::clear_errors() noexcept
  {
    errors_.clear();
  }

  const std::string &Field::name() const noexcept
  {
    return name_;
  }

  FieldType Field::type() const noexcept
  {
    return type_;
  }

  const std::string &Field::value() const noexcept
  {
    return value_;
  }

  const std::string &Field::label() const noexcept
  {
    return label_;
  }

  const std::string &Field::placeholder() const noexcept
  {
    return placeholder_;
  }

  bool Field::required() const noexcept
  {
    return required_;
  }

  bool Field::disabled() const noexcept
  {
    return disabled_;
  }

  bool Field::readonly() const noexcept
  {
    return readonly_;
  }

  bool Field::checked() const noexcept
  {
    return checked_;
  }

  bool Field::multiple() const noexcept
  {
    return multiple_;
  }

  const std::string &Field::accept() const noexcept
  {
    return accept_;
  }

  const HtmlAttrs &Field::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &Field::attrs() noexcept
  {
    return attrs_;
  }

  const std::vector<FieldOption> &Field::options() const noexcept
  {
    return options_;
  }

  std::vector<FieldOption> &Field::options() noexcept
  {
    return options_;
  }

  const std::vector<ValidationError> &Field::errors() const noexcept
  {
    return errors_;
  }

  bool Field::has_name() const noexcept
  {
    return !name_.empty();
  }

  bool Field::has_value() const noexcept
  {
    return !value_.empty();
  }

  bool Field::has_label() const noexcept
  {
    return !label_.empty();
  }

  bool Field::has_placeholder() const noexcept
  {
    return !placeholder_.empty();
  }

  bool Field::has_accept() const noexcept
  {
    return !accept_.empty();
  }

  bool Field::has_options() const noexcept
  {
    return !options_.empty();
  }

  bool Field::has_errors() const noexcept
  {
    return !errors_.empty();
  }

  bool Field::valid() const noexcept
  {
    return errors_.empty();
  }

  bool Field::invalid() const noexcept
  {
    return !valid();
  }

  std::size_t Field::option_count() const noexcept
  {
    return options_.size();
  }

  std::size_t Field::error_count() const noexcept
  {
    return errors_.size();
  }

  std::string Field::render() const
  {
    HtmlAttrs attrs = build_field_attrs(*this);

    switch (type_)
    {
    case FieldType::Textarea:
      return Html::tag("textarea", Html::text(value_), attrs);

    case FieldType::Select:
      attrs.boolean("multiple", multiple_);
      return Html::tag("select", render_options(*this), attrs);

    case FieldType::Radio:
      if (has_options())
      {
        return render_radio_group(*this);
      }

      attrs.set("type", std::string(to_string(type_)));
      attrs.set("value", value_);
      attrs.boolean("checked", checked_);
      return Html::void_tag("input", attrs);

    case FieldType::Text:
    case FieldType::Email:
    case FieldType::Password:
    case FieldType::Number:
    case FieldType::Hidden:
    case FieldType::Checkbox:
    case FieldType::File:
      attrs.set("type", std::string(to_string(type_)));

      if (type_ == FieldType::Checkbox)
      {
        attrs.boolean("checked", checked_);
      }

      if (type_ == FieldType::File)
      {
        attrs.boolean("multiple", multiple_);

        if (has_accept())
        {
          attrs.set("accept", accept_);
        }
      }

      if (type_ != FieldType::File &&
          type_ != FieldType::Password)
      {
        attrs.set("value", value_);
      }

      return Html::void_tag("input", attrs);

    case FieldType::Custom:
      break;
    }

    throw FormError("cannot render custom UI field without custom HTML");
  }

} // namespace vix::ui
