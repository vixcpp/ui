/**
 *
 * @file 09_rich_forms.cpp
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
//   vix run examples/09_rich_forms.cpp

#include <iostream>
#include <vix/ui/all.hpp>

int main()
{
  vix::ui::Form form = vix::ui::Form::post("/products/create");

  form.set_attr("class", "product-form")
      .set_csrf("csrf-demo-token");

  form.add_field(
      vix::ui::Field::text("title")
          .set_label("Product title")
          .set_placeholder("Example: iPhone 13 Pro")
          .set_required(true));

  form.add_field(
      vix::ui::Field::number("price")
          .set_label("Price")
          .set_placeholder("Example: 350")
          .set_required(true)
          .set_attr("min", "0")
          .set_attr("step", "0.01"));

  vix::ui::Field condition = vix::ui::Field::select("condition")
                                 .set_label("Condition")
                                 .set_required(true);

  condition.add_option("", "Choose condition");
  condition.add_option("new", "New");
  condition.add_option("used", "Used");
  condition.add_option("refurbished", "Refurbished");

  form.add_field(condition);

  vix::ui::Field visibility = vix::ui::Field::radio("visibility")
                                  .set_label("Visibility")
                                  .set_value("public")
                                  .set_required(true);

  visibility.add_option("public", "Public");
  visibility.add_option("private", "Private");
  visibility.add_option("draft", "Draft");

  form.add_field(visibility);

  form.add_field(
      vix::ui::Field::checkbox("featured")
          .set_label("Mark as featured")
          .set_value("1")
          .set_checked(true));

  form.add_field(
      vix::ui::Field::file("images")
          .set_label("Product images")
          .set_accept("image/png,image/jpeg,image/webp")
          .set_multiple(true));

  form.add_field(
      vix::ui::Field::textarea("description")
          .set_label("Description")
          .set_placeholder("Write a short product description")
          .set_attr("rows", "5"));

  std::cout << form.render() << '\n';

  return 0;
}
