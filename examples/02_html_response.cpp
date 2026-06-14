/**
 *
 *  @file 02_html_response.cpp
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
//   vix run examples/02_html_response.cpp

#include <iostream>

#include <vix/ui/html/Html.hpp>
#include <vix/ui/html/HtmlAttrs.hpp>
#include <vix/ui/html/HtmlResponse.hpp>

int main()
{
  vix::ui::HtmlAttrs attrs;
  attrs.set("class", "card");
  attrs.set("id", "welcome");

  const std::string html =
      vix::ui::Html::doctype() + "\n" +
      vix::ui::Html::tag(
          "main",
          vix::ui::Html::tag("h1", vix::ui::Html::text("Vix UI")) +
              vix::ui::Html::tag(
                  "p",
                  vix::ui::Html::text("HTML response helpers are ready.")),
          attrs);

  vix::ui::HtmlResponse response =
      vix::ui::HtmlResponse::html(html, 200);

  std::cout << "Status: " << response.status_code() << "\n";
  std::cout << "Content-Type: " << response.header_content_type() << "\n";
  std::cout << "Body size: " << response.size() << "\n\n";
  std::cout << response.body() << "\n";

  return 0;
}
