/**
 *
 *  @file 04_forms.cpp
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
// Run:
//   vix run examples/04_forms.cpp

#include <iostream>

#include <vix/ui/forms/Field.hpp>
#include <vix/ui/forms/Form.hpp>

int main()
{
  vix::ui::Form form = vix::ui::Form::post("/login");

  form.set_attr("class", "login-form");

  form.add_hidden("_token", "csrf-demo-token");

  vix::ui::Field email = vix::ui::Field::email("email");
  email.set_label("Email")
      .set_placeholder("you@example.com")
      .set_required();

  vix::ui::Field password = vix::ui::Field::password("password");
  password.set_label("Password")
      .set_placeholder("Your password")
      .set_required();

  vix::ui::Field remember = vix::ui::Field::checkbox("remember");
  remember.set_label("Remember me")
      .set_bool_attr("checked", true);

  form.add_field(email);
  form.add_field(password);
  form.add_field(remember);

  form.add_error("email", "Email is required.");

  std::cout << form.render() << "\n";

  return 0;
}
