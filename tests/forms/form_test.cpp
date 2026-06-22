/**
*
* @file form_test.cpp
* @author Gaspard Kirira
*
* Copyright 2025, Gaspard Kirira.
* All rights reserved.
* https://github.com/vixcpp/vix
*
* Use of this source code is governed by a MIT license
* that can be found in the License file.
*
* Vix.cpp
*

*/
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <vix/ui/forms/Field.hpp>
#include <vix/ui/forms/FieldOption.hpp>
#include <vix/ui/forms/Form.hpp>
#include <vix/ui/forms/ValidationError.hpp>
#include <vix/ui/support/Error.hpp>

using namespace vix::ui;

static void test_validation_error_code_to_string()
{
  assert(to_string(ValidationErrorCode::Unknown) == "unknown");
  assert(to_string(ValidationErrorCode::Required) == "required");
  assert(to_string(ValidationErrorCode::Invalid) == "invalid");
  assert(to_string(ValidationErrorCode::TooShort) == "too_short");
  assert(to_string(ValidationErrorCode::TooLong) == "too_long");
  assert(to_string(ValidationErrorCode::TooSmall) == "too_small");
  assert(to_string(ValidationErrorCode::TooLarge) == "too_large");
  assert(to_string(ValidationErrorCode::PatternMismatch) == "pattern_mismatch");
  assert(to_string(ValidationErrorCode::Custom) == "custom");
}

static void test_validation_error_required()
{
  ValidationError error = ValidationError::required("email");

  assert(error.field() == "email");
  assert(error.code() == ValidationErrorCode::Required);
  assert(error.message() == "This field is required.");
  assert(error.has_field());
  assert(error.has_message());
  assert(!error.empty());
}

static void test_validation_error_custom()
{
  ValidationError error = ValidationError::custom("name", "Name is invalid.");

  assert(error.field() == "name");
  assert(error.code() == ValidationErrorCode::Custom);
  assert(error.message() == "Name is invalid.");
}

static void test_field_type_to_string()
{
  assert(to_string(FieldType::Text) == "text");
  assert(to_string(FieldType::Email) == "email");
  assert(to_string(FieldType::Password) == "password");
  assert(to_string(FieldType::Number) == "number");
  assert(to_string(FieldType::Hidden) == "hidden");
  assert(to_string(FieldType::Checkbox) == "checkbox");
  assert(to_string(FieldType::Radio) == "radio");
  assert(to_string(FieldType::Textarea) == "textarea");
  assert(to_string(FieldType::Select) == "select");
  assert(to_string(FieldType::File) == "file");
  assert(to_string(FieldType::Custom) == "custom");
}

static void test_text_field()
{
  Field field = Field::text("username");

  field.set_label("Username");
  field.set_value("gaspard");
  field.set_placeholder("Enter username");
  field.set_required();

  assert(field.name() == "username");
  assert(field.type() == FieldType::Text);
  assert(field.label() == "Username");
  assert(field.value() == "gaspard");
  assert(field.placeholder() == "Enter username");
  assert(field.required());
  assert(!field.disabled());
  assert(!field.readonly());
  assert(!field.checked());
  assert(!field.multiple());
  assert(field.accept().empty());
  assert(field.valid());
}

static void test_number_field_factory()
{
  Field field = Field::number("quantity");

  field.set_value("42");

  assert(field.name() == "quantity");
  assert(field.type() == FieldType::Number);
  assert(field.value() == "42");

  assert(
      field.render() ==
      R"(<input id="quantity" name="quantity" type="number" value="42">)");
}

static void test_radio_field_factory()
{
  Field field = Field::radio("role");

  field.set_value("admin");

  assert(field.name() == "role");
  assert(field.type() == FieldType::Radio);
  assert(field.value() == "admin");
}

static void test_select_field_factory()
{
  Field field = Field::select("country");

  assert(field.name() == "country");
  assert(field.type() == FieldType::Select);
}

static void test_file_field_factory()
{
  Field field = Field::file("avatar");

  assert(field.name() == "avatar");
  assert(field.type() == FieldType::File);
}

static void test_email_field_render()
{
  Field field = Field::email("email");

  field.set_value("hello@example.com");
  field.set_required();

  const std::string rendered = field.render();

  assert(rendered.find("<input") != std::string::npos);
  assert(rendered.find(R"(id="email")") != std::string::npos);
  assert(rendered.find(R"(name="email")") != std::string::npos);
  assert(rendered.find("required") != std::string::npos);
  assert(rendered.find(R"(type="email")") != std::string::npos);
  assert(rendered.find(R"(value="hello@example.com")") != std::string::npos);
}

static void test_password_field_render()
{
  Field field = Field::password("password");

  field.set_placeholder("Password");

  assert(
      field.render() ==
      R"(<input id="password" name="password" placeholder="Password" type="password">)");
}

static void test_password_field_does_not_render_value()
{
  Field field = Field::password("password");

  field.set_value("secret");

  assert(
      field.render() ==
      R"(<input id="password" name="password" type="password">)");
}

static void test_hidden_field_render()
{
  Field field = Field::hidden("_token", "abc123");

  assert(
      field.render() ==
      R"(<input id="_token" name="_token" type="hidden" value="abc123">)");
}

static void test_textarea_field_render()
{
  Field field = Field::textarea("bio");

  field.set_value("<builder>");
  field.set_placeholder("Your bio");

  assert(
      field.render() ==
      R"(<textarea id="bio" name="bio" placeholder="Your bio">&lt;builder&gt;</textarea>)");
}

static void test_checkbox_field_render_unchecked()
{
  Field field = Field::checkbox("remember");

  assert(
      field.render() ==
      R"(<input id="remember" name="remember" type="checkbox" value="">)");
}

static void test_checkbox_field_render_checked()
{
  Field field = Field::checkbox("remember");

  field.set_checked(true);

  assert(field.checked());

  assert(
      field.render() ==
      R"(<input checked id="remember" name="remember" type="checkbox" value="">)");
}

static void test_checkbox_checked_can_be_disabled()
{
  Field field = Field::checkbox("remember");

  field.set_checked(true);
  assert(field.checked());

  field.set_checked(false);
  assert(!field.checked());

  assert(
      field.render() ==
      R"(<input id="remember" name="remember" type="checkbox" value="">)");
}

static void test_file_field_render()
{
  Field field = Field::file("avatar");

  assert(
      field.render() ==
      R"(<input id="avatar" name="avatar" type="file">)");
}

static void test_file_field_render_with_accept()
{
  Field field = Field::file("avatar");

  field.set_accept("image/png,image/jpeg");

  assert(field.has_accept());
  assert(field.accept() == "image/png,image/jpeg");

  assert(
      field.render() ==
      R"(<input accept="image/png,image/jpeg" id="avatar" name="avatar" type="file">)");
}

static void test_file_field_render_multiple()
{
  Field field = Field::file("photos");

  field.set_multiple(true);

  assert(field.multiple());

  assert(
      field.render() ==
      R"(<input id="photos" multiple name="photos" type="file">)");
}

static void test_file_field_does_not_render_value()
{
  Field field = Field::file("avatar");

  field.set_value("avatar.png");

  assert(
      field.render() ==
      R"(<input id="avatar" name="avatar" type="file">)");
}

static void test_select_field_options()
{
  Field field = Field::select("country");

  field.add_option("ug", "Uganda");
  field.add_option("cd", "DRC");
  field.add_option("rw", "Rwanda");

  assert(field.has_options());
  assert(field.option_count() == 3);
  assert(field.options()[0].value() == "ug");
  assert(field.options()[1].label() == "DRC");
}

static void test_select_field_render()
{
  Field field = Field::select("country");

  field.add_option("ug", "Uganda");
  field.add_option("cd", "DRC");

  assert(
      field.render() ==
      R"(<select id="country" name="country"><option value="ug">Uganda</option><option value="cd">DRC</option></select>)");
}

static void test_select_field_render_selected_from_value()
{
  Field field = Field::select("country");

  field.set_value("cd");
  field.add_option("ug", "Uganda");
  field.add_option("cd", "DRC");

  assert(
      field.render() ==
      R"(<select id="country" name="country"><option value="ug">Uganda</option><option selected value="cd">DRC</option></select>)");
}

static void test_select_field_render_explicit_selected_option()
{
  Field field = Field::select("country");

  field.add_option("ug", "Uganda");
  field.add_option(FieldOption::make("cd", "DRC").set_selected(true));

  assert(
      field.render() ==
      R"(<select id="country" name="country"><option value="ug">Uganda</option><option selected value="cd">DRC</option></select>)");
}

static void test_select_field_render_multiple()
{
  Field field = Field::select("countries");

  field.set_multiple(true);
  field.add_option("ug", "Uganda");
  field.add_option("cd", "DRC");

  assert(
      field.render() ==
      R"(<select id="countries" multiple name="countries"><option value="ug">Uganda</option><option value="cd">DRC</option></select>)");
}

static void test_select_field_set_options()
{
  Field field = Field::select("country");

  std::vector<FieldOption> options;
  options.push_back(FieldOption::make("ug", "Uganda"));
  options.push_back(FieldOption::make("cd", "DRC"));

  field.set_options(options);

  assert(field.has_options());
  assert(field.option_count() == 2);

  assert(
      field.render() ==
      R"(<select id="country" name="country"><option value="ug">Uganda</option><option value="cd">DRC</option></select>)");
}

static void test_select_field_clear_options()
{
  Field field = Field::select("country");

  field.add_option("ug", "Uganda");
  field.add_option("cd", "DRC");

  assert(field.option_count() == 2);

  field.clear_options();

  assert(!field.has_options());
  assert(field.option_count() == 0);

  assert(field.render() == R"(<select id="country" name="country"></select>)");
}

static void test_radio_input_render_without_options()
{
  Field field = Field::radio("role");

  field.set_value("admin");
  field.set_checked(true);

  assert(
      field.render() ==
      R"(<input checked id="role" name="role" type="radio" value="admin">)");
}

static void test_radio_group_render()
{
  Field field = Field::radio("role");

  field.add_option("admin", "Admin");
  field.add_option("editor", "Editor");

  assert(
      field.render() ==
      R"(<div class="radio-group" data-field="role"><div class="radio-option"><input id="role_admin" name="role" type="radio" value="admin"><label for="role_admin">Admin</label></div><div class="radio-option"><input id="role_editor" name="role" type="radio" value="editor"><label for="role_editor">Editor</label></div></div>)");
}

static void test_radio_group_selected_from_value()
{
  Field field = Field::radio("role");

  field.set_value("editor");
  field.add_option("admin", "Admin");
  field.add_option("editor", "Editor");

  assert(
      field.render() ==
      R"(<div class="radio-group" data-field="role"><div class="radio-option"><input id="role_admin" name="role" type="radio" value="admin"><label for="role_admin">Admin</label></div><div class="radio-option"><input checked id="role_editor" name="role" type="radio" value="editor"><label for="role_editor">Editor</label></div></div>)");
}

static void test_radio_group_selected_from_option()
{
  Field field = Field::radio("role");

  field.add_option("admin", "Admin");
  field.add_option(FieldOption::make("editor", "Editor").set_selected(true));

  assert(
      field.render() ==
      R"(<div class="radio-group" data-field="role"><div class="radio-option"><input id="role_admin" name="role" type="radio" value="admin"><label for="role_admin">Admin</label></div><div class="radio-option"><input checked id="role_editor" name="role" type="radio" value="editor"><label for="role_editor">Editor</label></div></div>)");
}

static void test_radio_group_disabled_option()
{
  Field field = Field::radio("role");

  field.add_option("admin", "Admin");
  field.add_option(FieldOption::make("editor", "Editor").set_disabled(true));

  assert(
      field.render() ==
      R"(<div class="radio-group" data-field="role"><div class="radio-option"><input id="role_admin" name="role" type="radio" value="admin"><label for="role_admin">Admin</label></div><div class="radio-option"><input disabled id="role_editor" name="role" type="radio" value="editor"><label for="role_editor">Editor</label></div></div>)");
}

static void test_radio_group_required()
{
  Field field = Field::radio("role");

  field.set_required(true);
  field.add_option("admin", "Admin");

  assert(
      field.render() ==
      R"(<div class="radio-group" data-field="role"><div class="radio-option"><input id="role_admin" name="role" required type="radio" value="admin"><label for="role_admin">Admin</label></div></div>)");
}

static void test_radio_group_safe_option_id()
{
  Field field = Field::radio("plan");

  field.add_option("pro plan", "Pro Plan");

  assert(
      field.render() ==
      R"(<div class="radio-group" data-field="plan"><div class="radio-option"><input id="plan_pro_plan" name="plan" type="radio" value="pro plan"><label for="plan_pro_plan">Pro Plan</label></div></div>)");
}

static void test_field_errors()
{
  Field field = Field::text("name");

  assert(field.valid());
  assert(!field.has_errors());

  field.add_error("Name is required.");

  assert(field.invalid());
  assert(field.has_errors());
  assert(field.error_count() == 1);
  assert(field.errors()[0].field() == "name");
  assert(field.errors()[0].message() == "Name is required.");

  field.clear_errors();

  assert(field.valid());
  assert(field.error_count() == 0);
}

static void test_custom_field_render_throws()
{
  Field field("custom", FieldType::Custom);

  bool thrown = false;

  try
  {
    (void)field.render();
  }
  catch (const FormError &)
  {
    thrown = true;
  }

  assert(thrown);
}

static void test_form_method_to_string()
{
  assert(to_string(FormMethod::Get) == "get");
  assert(to_string(FormMethod::Post) == "post");
}

static void test_form_create()
{
  Form form = Form::post("/login");

  assert(form.action() == "/login");
  assert(form.method() == FormMethod::Post);
  assert(form.has_action());
  assert(form.empty());
  assert(form.valid());
}

static void test_form_add_fields()
{
  Form form = Form::post("/login");

  form.add_email("email");
  form.add_password("password");
  form.add_hidden("_token", "abc123");

  assert(form.has_fields());
  assert(form.size() == 3);
  assert(form.has_field("email"));
  assert(form.has_field("password"));
  assert(form.has_field("_token"));

  const Field *email = form.field("email");
  const Field *password = form.field("password");
  const Field *token = form.field("_token");

  assert(email != nullptr);
  assert(password != nullptr);
  assert(token != nullptr);

  assert(email->type() == FieldType::Email);
  assert(password->type() == FieldType::Password);
  assert(token->value() == "abc123");
}

static void test_form_replace_field()
{
  Form form = Form::post("/profile");

  form.add_text("name");

  Field replacement = Field::email("name");
  replacement.set_value("[hello@example.com](mailto:hello@example.com)");

  form.add_field(replacement);

  assert(form.size() == 1);

  const Field *field = form.field("name");

  assert(field != nullptr);
  assert(field->type() == FieldType::Email);
  assert(field->value() == "[hello@example.com](mailto:hello@example.com)");
}

static void test_form_remove_field()
{
  Form form = Form::post("/profile");

  form.add_text("name");
  assert(form.has_field("name"));

  const bool removed = form.remove_field("name");

  assert(removed);
  assert(!form.has_field("name"));
  assert(form.empty());
}

static void test_form_errors()
{
  Form form = Form::post("/login");

  form.add_email("email");
  form.add_error("email", "Email is required.");

  assert(form.invalid());
  assert(form.has_errors());
  assert(form.error_count() == 1);

  const Field *email = form.field("email");

  assert(email != nullptr);
  assert(email->invalid());
  assert(email->error_count() == 1);

  form.clear_errors();

  assert(form.valid());
  assert(form.error_count() == 0);

  email = form.field("email");
  assert(email != nullptr);
  assert(email->valid());
}

static void test_form_render_open_close()
{
  Form form = Form::post("/login");

  form.set_attr("class", "login-form");

  assert(form.render_open() == R"(<form action="/login" class="login-form" method="post">)");
  assert(form.render_close() == "</form>");
}

static void test_form_render_fields()
{
  Form form = Form::post("/login");

  Field email = Field::email("email");
  email.set_label("Email");

  Field password = Field::password("password");
  password.set_label("Password");

  form.add_field(email);
  form.add_field(password);

  const std::string rendered = form.render_fields();

  assert(rendered.find("<label") != std::string::npos);
  assert(rendered.find("Email") != std::string::npos);
  assert(rendered.find("Password") != std::string::npos);
  assert(rendered.find(R"(type="email")") != std::string::npos);
  assert(rendered.find(R"(type="password")") != std::string::npos);
}

static void test_form_render_errors()
{
  Form form = Form::post("/login");

  form.add_error("email", "Email is required.");

  const std::string rendered = form.render_errors();

  assert(rendered.find(R"(<ul class="form-errors">)") != std::string::npos);
  assert(rendered.find("Email is required.") != std::string::npos);
}

static void test_form_render()
{
  Form form = Form::post("/login");

  form.add_email("email");
  form.add_password("password");

  const std::string rendered = form.render();

  assert(rendered.find(R"(<form action="/login" method="post">)") != std::string::npos);
  assert(rendered.find(R"(type="email")") != std::string::npos);
  assert(rendered.find(R"(type="password")") != std::string::npos);
  assert(rendered.find("</form>") != std::string::npos);
}

int main()
{
  test_validation_error_code_to_string();
  test_validation_error_required();
  test_validation_error_custom();

  test_field_type_to_string();

  test_text_field();
  test_number_field_factory();
  test_radio_field_factory();
  test_select_field_factory();
  test_file_field_factory();

  test_email_field_render();
  test_password_field_render();
  test_password_field_does_not_render_value();
  test_hidden_field_render();
  test_textarea_field_render();

  test_checkbox_field_render_unchecked();
  test_checkbox_field_render_checked();
  test_checkbox_checked_can_be_disabled();

  test_file_field_render();
  test_file_field_render_with_accept();
  test_file_field_render_multiple();
  test_file_field_does_not_render_value();

  test_select_field_options();
  test_select_field_render();
  test_select_field_render_selected_from_value();
  test_select_field_render_explicit_selected_option();
  test_select_field_render_multiple();
  test_select_field_set_options();
  test_select_field_clear_options();

  test_radio_input_render_without_options();
  test_radio_group_render();
  test_radio_group_selected_from_value();
  test_radio_group_selected_from_option();
  test_radio_group_disabled_option();
  test_radio_group_required();
  test_radio_group_safe_option_id();

  test_field_errors();
  test_custom_field_render_throws();

  test_form_method_to_string();
  test_form_create();
  test_form_add_fields();
  test_form_replace_field();
  test_form_remove_field();
  test_form_errors();
  test_form_render_open_close();
  test_form_render_fields();
  test_form_render_errors();
  test_form_render();

  std::cout << "[OK] ui form tests passed\n";
  return 0;
}
