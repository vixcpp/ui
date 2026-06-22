/**
 *
 *  @file SafeArea.cpp
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
#include <vix/ui/pwa/SafeArea.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    void append_variable(
        std::string &output,
        std::string_view name,
        std::string_view value)
    {
      if (name.empty() || value.empty())
      {
        return;
      }

      output += "  ";
      output.append(name.data(), name.size());
      output += ": ";
      output.append(value.data(), value.size());
      output += ";\n";
    }

    void append_padding(
        std::string &output,
        std::string_view property,
        std::string_view value)
    {
      if (property.empty() || value.empty())
      {
        return;
      }

      output += "  ";
      output.append(property.data(), property.size());
      output += ": ";
      output.append(value.data(), value.size());
      output += ";\n";
    }
  } // namespace

  std::string_view to_string(SafeAreaEdge edge) noexcept
  {
    switch (edge)
    {
    case SafeAreaEdge::Top:
      return "top";

    case SafeAreaEdge::Right:
      return "right";

    case SafeAreaEdge::Bottom:
      return "bottom";

    case SafeAreaEdge::Left:
      return "left";
    }

    return "top";
  }

  SafeArea SafeArea::make()
  {
    return SafeArea{};
  }

  SafeArea SafeArea::all()
  {
    return SafeArea{};
  }

  SafeArea SafeArea::vertical()
  {
    SafeArea safe_area;
    safe_area.set_right("");
    safe_area.set_left("");
    return safe_area;
  }

  SafeArea &SafeArea::set_root_selector(std::string selector)
  {
    root_selector_ = std::move(selector);
    return *this;
  }

  SafeArea &SafeArea::set_class_name(std::string class_name)
  {
    class_name_ = std::move(class_name);
    return *this;
  }

  SafeArea &SafeArea::set_variable_prefix(std::string prefix)
  {
    variable_prefix_ = std::move(prefix);
    return *this;
  }

  SafeArea &SafeArea::set_top(std::string value)
  {
    top_ = std::move(value);
    return *this;
  }

  SafeArea &SafeArea::set_right(std::string value)
  {
    right_ = std::move(value);
    return *this;
  }

  SafeArea &SafeArea::set_bottom(std::string value)
  {
    bottom_ = std::move(value);
    return *this;
  }

  SafeArea &SafeArea::set_left(std::string value)
  {
    left_ = std::move(value);
    return *this;
  }

  SafeArea &SafeArea::set(
      SafeAreaEdge edge,
      std::string value)
  {
    switch (edge)
    {
    case SafeAreaEdge::Top:
      return set_top(std::move(value));

    case SafeAreaEdge::Right:
      return set_right(std::move(value));

    case SafeAreaEdge::Bottom:
      return set_bottom(std::move(value));

    case SafeAreaEdge::Left:
      return set_left(std::move(value));
    }

    return *this;
  }

  const std::string &SafeArea::root_selector() const noexcept
  {
    return root_selector_;
  }

  const std::string &SafeArea::class_name() const noexcept
  {
    return class_name_;
  }

  const std::string &SafeArea::variable_prefix() const noexcept
  {
    return variable_prefix_;
  }

  const std::string &SafeArea::top() const noexcept
  {
    return top_;
  }

  const std::string &SafeArea::right() const noexcept
  {
    return right_;
  }

  const std::string &SafeArea::bottom() const noexcept
  {
    return bottom_;
  }

  const std::string &SafeArea::left() const noexcept
  {
    return left_;
  }

  const std::string &SafeArea::get(SafeAreaEdge edge) const noexcept
  {
    switch (edge)
    {
    case SafeAreaEdge::Top:
      return top_;

    case SafeAreaEdge::Right:
      return right_;

    case SafeAreaEdge::Bottom:
      return bottom_;

    case SafeAreaEdge::Left:
      return left_;
    }

    return top_;
  }

  bool SafeArea::has_root_selector() const noexcept
  {
    return !root_selector_.empty();
  }

  bool SafeArea::has_class_name() const noexcept
  {
    return !class_name_.empty();
  }

  bool SafeArea::has_variable_prefix() const noexcept
  {
    return !variable_prefix_.empty();
  }

  bool SafeArea::has_top() const noexcept
  {
    return !top_.empty();
  }

  bool SafeArea::has_right() const noexcept
  {
    return !right_.empty();
  }

  bool SafeArea::has_bottom() const noexcept
  {
    return !bottom_.empty();
  }

  bool SafeArea::has_left() const noexcept
  {
    return !left_.empty();
  }

  std::string SafeArea::variable_name(SafeAreaEdge edge) const
  {
    if (variable_prefix_.empty())
    {
      return {};
    }

    return variable_prefix_ + "-" + std::string(to_string(edge));
  }

  std::string SafeArea::variable_ref(SafeAreaEdge edge) const
  {
    const std::string name = variable_name(edge);

    if (name.empty())
    {
      return {};
    }

    return "var(" + name + ")";
  }

  std::string SafeArea::render_variables() const
  {
    if (root_selector_.empty() || variable_prefix_.empty())
    {
      return {};
    }

    std::string output;

    output += root_selector_;
    output += " {\n";

    append_variable(output, variable_name(SafeAreaEdge::Top), top_);
    append_variable(output, variable_name(SafeAreaEdge::Right), right_);
    append_variable(output, variable_name(SafeAreaEdge::Bottom), bottom_);
    append_variable(output, variable_name(SafeAreaEdge::Left), left_);

    output += "}";

    return output;
  }

  std::string SafeArea::render_class() const
  {
    if (class_name_.empty() || variable_prefix_.empty())
    {
      return {};
    }

    std::string output;

    output += ".";
    output += class_name_;
    output += " {\n";

    append_padding(
        output,
        "padding-top",
        has_top() ? variable_ref(SafeAreaEdge::Top) : "");

    append_padding(
        output,
        "padding-right",
        has_right() ? variable_ref(SafeAreaEdge::Right) : "");

    append_padding(
        output,
        "padding-bottom",
        has_bottom() ? variable_ref(SafeAreaEdge::Bottom) : "");

    append_padding(
        output,
        "padding-left",
        has_left() ? variable_ref(SafeAreaEdge::Left) : "");

    output += "}";

    return output;
  }

  std::string SafeArea::render() const
  {
    const std::string variables = render_variables();
    const std::string css_class = render_class();

    if (variables.empty())
    {
      return css_class;
    }

    if (css_class.empty())
    {
      return variables;
    }

    return variables + "\n" + css_class;
  }

} // namespace vix::ui
