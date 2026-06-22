/**

* @file 10_form_binding.cpp
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
// Run:
//   vix run examples/10_form_binding.cpp

#include <iostream>

#include <vix/ui/all.hpp>

int main()
{
  vix::ui::FormData old_input;

  old_input.set("name", "Gaspard");
  old_input.set("email", "[gaspard@example.com](mailto:gaspard@example.com)");
  old_input.set("country", "ug");
  old_input.set("role", "admin");
  old_input.set("newsletter", "1");
  old_input.set("bio", "Building server-rendered UI with Vix.cpp.");

  vix::ui::Form form = vix::ui::Form::post("/profile/update");

  form.set_attr("class", "profile-form")
      .set_csrf(vix::ui::CsrfToken::named("_csrf", "csrf-demo-token"));

  form.add_field(
      vix::ui::Field::text("name")
          .set_label("Name")
          .set_required(true));

  form.add_field(
      vix::ui::Field::email("email")
          .set_label("Email")
          .set_required(true));

  form.add_field(
      vix::ui::Field::password("password")
          .set_label("Password")
          .set_placeholder("Leave empty to keep current password"));

  vix::ui::Field country = vix::ui::Field::select("country")
                               .set_label("Country")
                               .set_required(true);

  country.add_option("ug", "Uganda");
  country.add_option("cd", "DRC");
  country.add_option("rw", "Rwanda");
  country.add_option("ke", "Kenya");

  form.add_field(country);

  vix::ui::Field role = vix::ui::Field::radio("role")
                            .set_label("Role")
                            .set_required(true);

  role.add_option("admin", "Admin");
  role.add_option("editor", "Editor");
  role.add_option("viewer", "Viewer");

  form.add_field(role);

  form.add_field(
      vix::ui::Field::checkbox("newsletter")
          .set_label("Receive product updates")
          .set_value("1"));

  form.add_field(
      vix::ui::Field::textarea("bio")
          .set_label("Bio")
          .set_attr("rows", "4"));

  form.bind(old_input);

  form.add_error("email", "This email address is already used.");

  std::cout << form.render() << '\n';

  return 0;
}
