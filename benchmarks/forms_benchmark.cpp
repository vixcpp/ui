/**
 *
 *  @file forms_benchmark.cpp
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
#include <benchmark/benchmark.h>

#include <string>

#include <vix/ui/forms/Field.hpp>
#include <vix/ui/forms/Form.hpp>
#include <vix/ui/forms/ValidationError.hpp>

namespace
{
  vix::ui::Field text_field()
  {
    return vix::ui::Field::text("username")
        .set_label("Username")
        .set_placeholder("Enter username")
        .set_value("gaspard")
        .set_required()
        .set_attr("class", "form-control")
        .set_attr("autocomplete", "username");
  }

  vix::ui::Field email_field()
  {
    return vix::ui::Field::email("email")
        .set_label("Email")
        .set_placeholder("you@example.com")
        .set_value("hello@example.com")
        .set_required()
        .set_attr("class", "form-control")
        .set_attr("autocomplete", "email");
  }

  vix::ui::Field password_field()
  {
    return vix::ui::Field::password("password")
        .set_label("Password")
        .set_placeholder("Your password")
        .set_required()
        .set_attr("class", "form-control")
        .set_attr("autocomplete", "current-password");
  }

  vix::ui::Field textarea_field()
  {
    return vix::ui::Field::textarea("bio")
        .set_label("Bio")
        .set_placeholder("Write something about yourself")
        .set_value("C++ developer building with Vix UI.")
        .set_attr("class", "form-control")
        .set_attr("rows", "5");
  }

  vix::ui::Field checkbox_field()
  {
    return vix::ui::Field::checkbox("remember")
        .set_label("Remember me")
        .set_bool_attr("checked", true)
        .set_attr("class", "form-check-input");
  }

  vix::ui::Form login_form()
  {
    vix::ui::Form form = vix::ui::Form::post("/login");

    form.set_attr("class", "login-form");
    form.add_hidden("_token", "csrf-demo-token");
    form.add_field(email_field());
    form.add_field(password_field());
    form.add_field(checkbox_field());

    return form;
  }

  vix::ui::Form profile_form()
  {
    vix::ui::Form form = vix::ui::Form::post("/profile");

    form.set_attr("class", "profile-form");
    form.add_hidden("_token", "csrf-demo-token");
    form.add_field(text_field());
    form.add_field(email_field());
    form.add_field(textarea_field());

    return form;
  }

  vix::ui::Form form_with_errors()
  {
    vix::ui::Form form = profile_form();

    form.add_error("username", "Username is required.");
    form.add_error("email", "Email is invalid.");
    form.add_error("bio", "Bio is too long.");

    return form;
  }
}

static void BM_FieldRenderText(benchmark::State &state)
{
  const vix::ui::Field field = text_field();

  for (auto _ : state)
  {
    std::string output = field.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FieldRenderEmail(benchmark::State &state)
{
  const vix::ui::Field field = email_field();

  for (auto _ : state)
  {
    std::string output = field.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FieldRenderPassword(benchmark::State &state)
{
  const vix::ui::Field field = password_field();

  for (auto _ : state)
  {
    std::string output = field.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FieldRenderTextarea(benchmark::State &state)
{
  const vix::ui::Field field = textarea_field();

  for (auto _ : state)
  {
    std::string output = field.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FieldRenderCheckbox(benchmark::State &state)
{
  const vix::ui::Field field = checkbox_field();

  for (auto _ : state)
  {
    std::string output = field.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FormRenderOpen(benchmark::State &state)
{
  const vix::ui::Form form = login_form();

  for (auto _ : state)
  {
    std::string output = form.render_open();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FormRenderFields(benchmark::State &state)
{
  const vix::ui::Form form = login_form();

  for (auto _ : state)
  {
    std::string output = form.render_fields();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FormRenderLogin(benchmark::State &state)
{
  const vix::ui::Form form = login_form();

  for (auto _ : state)
  {
    std::string output = form.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FormRenderProfile(benchmark::State &state)
{
  const vix::ui::Form form = profile_form();

  for (auto _ : state)
  {
    std::string output = form.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FormRenderErrorsOnly(benchmark::State &state)
{
  const vix::ui::Form form = form_with_errors();

  for (auto _ : state)
  {
    std::string output = form.render_errors();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FormRenderWithErrors(benchmark::State &state)
{
  const vix::ui::Form form = form_with_errors();

  for (auto _ : state)
  {
    std::string output = form.render();
    benchmark::DoNotOptimize(output);
  }
}

BENCHMARK(BM_FieldRenderText);
BENCHMARK(BM_FieldRenderEmail);
BENCHMARK(BM_FieldRenderPassword);
BENCHMARK(BM_FieldRenderTextarea);
BENCHMARK(BM_FieldRenderCheckbox);

BENCHMARK(BM_FormRenderOpen);
BENCHMARK(BM_FormRenderFields);
BENCHMARK(BM_FormRenderLogin);
BENCHMARK(BM_FormRenderProfile);
BENCHMARK(BM_FormRenderErrorsOnly);
BENCHMARK(BM_FormRenderWithErrors);
