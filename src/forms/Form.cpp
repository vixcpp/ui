/**
 *
 *  @file Form.cpp
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
#include <vix/ui/forms/Form.hpp>
#include <vix/ui/html/Html.hpp>

#include <algorithm>
#include <utility>

namespace vix::ui
{
  namespace
  {
    void append_line(std::string &output, std::string_view value)
    {
      if (!output.empty())
      {
        output.push_back('\n');
      }

      output.append(value.data(), value.size());
    }

    std::vector<Field>::iterator find_field_it(
        std::vector<Field> &fields,
        std::string_view name)
    {
      return std::find_if(
          fields.begin(),
          fields.end(),
          [name](const Field &field)
          {
            return field.name() == name;
          });
    }

    std::vector<Field>::const_iterator find_field_it(
        const std::vector<Field> &fields,
        std::string_view name)
    {
      return std::find_if(
          fields.begin(),
          fields.end(),
          [name](const Field &field)
          {
            return field.name() == name;
          });
    }

    bool has_file_field(const std::vector<Field> &fields) noexcept
    {
      for (const auto &field : fields)
      {
        if (field.type() == FieldType::File)
        {
          return true;
        }
      }

      return false;
    }

    void apply_bound_data(Field &field, const FormData &data)
    {
      if (!field.has_name())
      {
        return;
      }

      if (!data.has(field.name()))
      {
        return;
      }

      if (field.type() == FieldType::File)
      {
        return;
      }

      const std::vector<std::string> *values = data.get_all(field.name());

      if (values == nullptr)
      {
        return;
      }

      switch (field.type())
      {
      case FieldType::Checkbox:
        if (field.has_value())
        {
          field.set_checked(data.contains(field.name(), field.value()));
        }
        else
        {
          field.set_checked(!values->empty());
        }
        return;

      case FieldType::Radio:
      case FieldType::Select:
        if (!values->empty())
        {
          field.set_value(values->front());
        }

        for (auto &option : field.options())
        {
          option.set_selected(data.contains(field.name(), option.value()));
        }
        return;

      case FieldType::Text:
      case FieldType::Email:
      case FieldType::Number:
      case FieldType::Hidden:
      case FieldType::Textarea:
        if (!values->empty())
        {
          field.set_value(values->front());
        }
        return;

      case FieldType::Password:
      case FieldType::File:
      case FieldType::Custom:
        return;
      }
    }

    std::string render_field_wrapper(const Field &field)
    {
      if (field.type() == FieldType::Hidden)
      {
        return field.render();
      }

      std::string output;

      if (field.has_label())
      {
        HtmlAttrs label_attrs;
        if (field.has_name())
        {
          label_attrs.set("for", field.name());
        }

        append_line(
            output,
            Html::tag("label", Html::text(field.label()), label_attrs));
      }

      append_line(output, field.render());

      if (field.has_errors())
      {
        for (const auto &error : field.errors())
        {
          HtmlAttrs attrs;
          attrs.set("class", "field-error");

          append_line(
              output,
              Html::tag("div", Html::text(error.message()), attrs));
        }
      }

      HtmlAttrs wrapper_attrs;
      wrapper_attrs.set("class", field.invalid() ? "field invalid" : "field");

      return Html::tag("div", output, wrapper_attrs);
    }
  } // namespace

  std::string_view to_string(FormMethod method) noexcept
  {
    switch (method)
    {
    case FormMethod::Get:
      return "get";

    case FormMethod::Post:
      return "post";
    }

    return "post";
  }

  Form::Form(
      std::string action,
      FormMethod method)
      : action_(std::move(action)),
        method_(method)
  {
  }

  Form Form::make(
      std::string action,
      FormMethod method)
  {
    return Form(std::move(action), method);
  }

  Form Form::get(std::string action)
  {
    return Form(std::move(action), FormMethod::Get);
  }

  Form Form::post(std::string action)
  {
    return Form(std::move(action), FormMethod::Post);
  }

  Form &Form::set_action(std::string action)
  {
    action_ = std::move(action);
    return *this;
  }

  Form &Form::set_method(FormMethod method) noexcept
  {
    method_ = method;
    return *this;
  }

  Form &Form::set_attr(std::string name, std::string value)
  {
    attrs_.set(std::move(name), std::move(value));
    return *this;
  }

  Form &Form::set_bool_attr(std::string name, bool enabled)
  {
    attrs_.boolean(std::move(name), enabled);
    return *this;
  }

  Form &Form::bind(FormData data)
  {
    data_ = std::move(data);

    for (auto &field : fields_)
    {
      apply_bound_data(field, data_);
    }

    return *this;
  }

  Form &Form::set_data(FormData data)
  {
    data_ = std::move(data);
    return *this;
  }

  void Form::clear_data() noexcept
  {
    data_.clear();
  }

  Form &Form::set_csrf(CsrfToken token)
  {
    csrf_ = std::move(token);
    has_csrf_ = true;
    return *this;
  }

  Form &Form::set_csrf(std::string value)
  {
    csrf_ = CsrfToken::make(std::move(value));
    has_csrf_ = true;
    return *this;
  }

  void Form::clear_csrf() noexcept
  {
    csrf_ = CsrfToken{};
    has_csrf_ = false;
  }

  Form &Form::add_field(Field field)
  {
    if (Field *existing = this->field(field.name()))
    {
      *existing = std::move(field);
      return *this;
    }

    fields_.push_back(std::move(field));
    return *this;
  }

  Form &Form::add_text(std::string name)
  {
    return add_field(Field::text(std::move(name)));
  }

  Form &Form::add_email(std::string name)
  {
    return add_field(Field::email(std::move(name)));
  }

  Form &Form::add_password(std::string name)
  {
    return add_field(Field::password(std::move(name)));
  }

  Form &Form::add_number(std::string name)
  {
    return add_field(Field::number(std::move(name)));
  }

  Form &Form::add_hidden(
      std::string name,
      std::string value)
  {
    return add_field(Field::hidden(std::move(name), std::move(value)));
  }

  Form &Form::add_checkbox(std::string name)
  {
    return add_field(Field::checkbox(std::move(name)));
  }

  Form &Form::add_radio(std::string name)
  {
    return add_field(Field::radio(std::move(name)));
  }

  Form &Form::add_textarea(std::string name)
  {
    return add_field(Field::textarea(std::move(name)));
  }

  Form &Form::add_select(std::string name)
  {
    return add_field(Field::select(std::move(name)));
  }

  Form &Form::add_file(std::string name)
  {
    return add_field(Field::file(std::move(name)));
  }

  bool Form::has_field(std::string_view name) const noexcept
  {
    return field(name) != nullptr;
  }

  const Field *Form::field(std::string_view name) const noexcept
  {
    const auto it = find_field_it(fields_, name);
    if (it == fields_.end())
    {
      return nullptr;
    }

    return &*it;
  }

  Field *Form::field(std::string_view name) noexcept
  {
    const auto it = find_field_it(fields_, name);
    if (it == fields_.end())
    {
      return nullptr;
    }

    return &*it;
  }

  bool Form::remove_field(std::string_view name)
  {
    const auto before = fields_.size();

    fields_.erase(
        std::remove_if(
            fields_.begin(),
            fields_.end(),
            [name](const Field &field)
            {
              return field.name() == name;
            }),
        fields_.end());

    return fields_.size() != before;
  }

  void Form::clear_fields() noexcept
  {
    fields_.clear();
  }

  Form &Form::add_error(ValidationError error)
  {
    if (Field *target = field(error.field()))
    {
      target->add_error(error);
    }

    errors_.push_back(std::move(error));
    return *this;
  }

  Form &Form::add_error(
      std::string field,
      std::string message)
  {
    return add_error(
        ValidationError::custom(std::move(field), std::move(message)));
  }

  void Form::clear_errors() noexcept
  {
    errors_.clear();

    for (auto &field : fields_)
    {
      field.clear_errors();
    }
  }

  const std::string &Form::action() const noexcept
  {
    return action_;
  }

  FormMethod Form::method() const noexcept
  {
    return method_;
  }

  const HtmlAttrs &Form::attrs() const noexcept
  {
    return attrs_;
  }

  HtmlAttrs &Form::attrs() noexcept
  {
    return attrs_;
  }

  const FormData &Form::data() const noexcept
  {
    return data_;
  }

  FormData &Form::data() noexcept
  {
    return data_;
  }

  const CsrfToken &Form::csrf() const noexcept
  {
    return csrf_;
  }

  const std::vector<Field> &Form::fields() const noexcept
  {
    return fields_;
  }

  std::vector<Field> &Form::fields() noexcept
  {
    return fields_;
  }

  const std::vector<ValidationError> &Form::errors() const noexcept
  {
    return errors_;
  }

  bool Form::has_action() const noexcept
  {
    return !action_.empty();
  }

  bool Form::has_fields() const noexcept
  {
    return !fields_.empty();
  }

  bool Form::has_errors() const noexcept
  {
    return !errors_.empty();
  }

  bool Form::has_data() const noexcept
  {
    return !data_.empty();
  }

  bool Form::has_csrf() const noexcept
  {
    return has_csrf_;
  }

  bool Form::empty() const noexcept
  {
    return fields_.empty();
  }

  std::size_t Form::size() const noexcept
  {
    return fields_.size();
  }

  std::size_t Form::error_count() const noexcept
  {
    return errors_.size();
  }

  bool Form::valid() const noexcept
  {
    if (!errors_.empty())
    {
      return false;
    }

    for (const auto &field : fields_)
    {
      if (field.invalid())
      {
        return false;
      }
    }

    return true;
  }

  bool Form::invalid() const noexcept
  {
    return !valid();
  }

  std::string Form::render_open() const
  {
    HtmlAttrs attrs = attrs_;

    if (has_action())
    {
      attrs.set("action", action_);
    }

    attrs.set("method", std::string(to_string(method_)));

    if (has_file_field(fields_) && !attrs.has("enctype"))
    {
      attrs.set("enctype", "multipart/form-data");
    }

    return Html::open_tag("form", attrs);
  }

  std::string Form::render_close() const
  {
    return Html::close_tag("form");
  }

  std::string Form::render_fields() const
  {
    std::string output;

    for (const auto &field : fields_)
    {
      append_line(output, render_field_wrapper(field));
    }

    return output;
  }

  std::string Form::render_errors() const
  {
    if (errors_.empty())
    {
      return {};
    }

    std::string items;

    for (const auto &error : errors_)
    {
      append_line(items, Html::tag("li", Html::text(error.message())));
    }

    HtmlAttrs attrs;
    attrs.set("class", "form-errors");

    return Html::tag("ul", items, attrs);
  }

  std::string Form::render_error_summary() const
  {
    const std::string errors = render_errors();

    if (errors.empty())
    {
      return {};
    }

    HtmlAttrs attrs;
    attrs.set("class", "form-error-summary");
    attrs.set("role", "alert");

    return Html::tag("div", errors, attrs);
  }

  std::string Form::render_csrf() const
  {
    if (!has_csrf_)
    {
      return {};
    }

    return csrf_.render();
  }

  std::string Form::render() const
  {
    std::string output;

    append_line(output, render_open());

    const std::string csrf = render_csrf();
    if (!csrf.empty())
    {
      append_line(output, csrf);
    }

    const std::string errors = render_error_summary();
    if (!errors.empty())
    {
      append_line(output, errors);
    }

    const std::string fields = render_fields();
    if (!fields.empty())
    {
      append_line(output, fields);
    }

    append_line(output, render_close());

    return output;
  }

} // namespace vix::ui
