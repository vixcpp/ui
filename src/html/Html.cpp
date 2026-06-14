/**
 *
 *  @file Html.cpp
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
#include <vix/ui/html/Html.hpp>
#include <vix/ui/html/HtmlEscape.hpp>
#include <vix/ui/support/Error.hpp>

#include <cctype>

namespace vix::ui
{
  namespace
  {
    void validate_tag_name(std::string_view name)
    {
      if (!Html::is_valid_tag_name(name))
      {
        throw HtmlError("invalid HTML tag name");
      }
    }

    void append_view(std::string &output, std::string_view value)
    {
      output.append(value.data(), value.size());
    }
  } // namespace

  std::string Html::escape(std::string_view input)
  {
    return HtmlEscape::text(input);
  }

  std::string Html::text(std::string_view input)
  {
    return HtmlEscape::text(input);
  }

  std::string Html::raw(std::string_view input)
  {
    return std::string(input);
  }

  std::string Html::doctype()
  {
    return "<!doctype html>";
  }

  std::string Html::open_tag(
      std::string_view name,
      const HtmlAttrs &attrs)
  {
    validate_tag_name(name);

    std::string output;
    output.reserve(name.size() + attrs.render().size() + 3);

    output.push_back('<');
    append_view(output, name);
    output += attrs.render_with_leading_space();
    output.push_back('>');

    return output;
  }

  std::string Html::close_tag(std::string_view name)
  {
    validate_tag_name(name);

    std::string output;
    output.reserve(name.size() + 3);

    output += "</";
    append_view(output, name);
    output.push_back('>');

    return output;
  }

  std::string Html::tag(
      std::string_view name,
      std::string_view content,
      const HtmlAttrs &attrs)
  {
    validate_tag_name(name);

    std::string output;
    const std::string rendered_attrs = attrs.render_with_leading_space();

    output.reserve(
        name.size() * 2 +
        content.size() +
        rendered_attrs.size() +
        5);

    output.push_back('<');
    append_view(output, name);
    output += rendered_attrs;
    output.push_back('>');
    append_view(output, content);
    output += "</";
    append_view(output, name);
    output.push_back('>');

    return output;
  }

  std::string Html::void_tag(
      std::string_view name,
      const HtmlAttrs &attrs)
  {
    validate_tag_name(name);

    std::string output;
    const std::string rendered_attrs = attrs.render_with_leading_space();

    output.reserve(name.size() + rendered_attrs.size() + 2);

    output.push_back('<');
    append_view(output, name);
    output += rendered_attrs;
    output.push_back('>');

    return output;
  }

  bool Html::is_valid_tag_name(std::string_view name) noexcept
  {
    if (name.empty())
    {
      return false;
    }

    const unsigned char first = static_cast<unsigned char>(name.front());

    if (!std::isalpha(first))
    {
      return false;
    }

    for (char ch : name)
    {
      const unsigned char c = static_cast<unsigned char>(ch);

      if (std::isalnum(c) ||
          ch == '-' ||
          ch == ':')
      {
        continue;
      }

      return false;
    }

    return true;
  }

} // namespace vix::ui
