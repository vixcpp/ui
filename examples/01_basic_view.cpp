/**
 *
 *  @file 01_basic_view.cpp
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
//   vix run examples/01_basic_view.cpp

#include <iostream>
#include <memory>

#include <vix/template/Engine.hpp>
#include <vix/template/StringLoader.hpp>
#include <vix/ui/core/View.hpp>

int main()
{
  auto loader = std::make_shared<vix::template_::StringLoader>();

  loader->set(
      "home.html",
      "<!doctype html>\n"
      "<html>\n"
      "<head>\n"
      "  <title>{{ page_title }}</title>\n"
      "</head>\n"
      "<body>\n"
      "  <h1>Hello {{ name }}</h1>\n"
      "  <p>Status: {{ status }}</p>\n"
      "</body>\n"
      "</html>");

  vix::template_::Engine engine(loader);

  vix::ui::View view("home.html");
  view.set_title("Vix UI Basic View");
  view.set("name", "Gaspard");
  view.set("status", "running");

  const vix::ui::ViewResult result = view.render(engine);

  if (!result.success)
  {
    std::cerr << "failed to render view\n";
    return 1;
  }

  std::cout << result.output << "\n";
  return 0;
}
