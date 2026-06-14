/**
 *
 *  @file HtmlAttrs.cpp
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
#include <vix/ui/html/HtmlAttrs.hpp>
#include <vix/ui/html/HtmlEscape.hpp>
#include <vix/ui/support/Error.hpp>

#include <cctype>
#include <utility>

namespace vix::ui
{
  HtmlAttrs::HtmlAttrs(std::map<std::string, std::string> attrs)
  {
    for (auto &[name, value] : attrs)
    {
      set(std::move(name), std::move(value));
    }
  }

  HtmlAttrs HtmlAttrs::make()
  {
    return HtmlAttrs{};
  }

  HtmlAttrs &HtmlAttrs::set(std::string name, std::string value)
  {
    if (!is_valid_name(name))
    {
      throw HtmlError("invalid HTML attribute name: " + name);
    }

    attrs_[std::move(name)] = HtmlAttr{
        .value = std::move(value),
        .boolean = false};

    return *this;
  }

  HtmlAttrs &HtmlAttrs::set_if(
      bool condition,
      std::string name,
      std::string value)
  {
    if (condition)
    {
      set(std::move(name), std::move(value));
    }

    return *this;
  }

  HtmlAttrs &HtmlAttrs::boolean(std::string name, bool enabled)
  {
    if (!is_valid_name(name))
    {
      throw HtmlError("invalid HTML attribute name: " + name);
    }

    if (!enabled)
    {
      attrs_.erase(name);
      return *this;
    }

    attrs_[std::move(name)] = HtmlAttr{
        .value = {},
        .boolean = true};

    return *this;
  }

  bool HtmlAttrs::has(const std::string &name) const
  {
    return attrs_.find(name) != attrs_.end();
  }

  const HtmlAttr *HtmlAttrs::get(const std::string &name) const noexcept
  {
    const auto it = attrs_.find(name);
    if (it == attrs_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  HtmlAttr *HtmlAttrs::get(const std::string &name) noexcept
  {
    const auto it = attrs_.find(name);
    if (it == attrs_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  bool HtmlAttrs::erase(const std::string &name)
  {
    return attrs_.erase(name) > 0;
  }

  void HtmlAttrs::clear() noexcept
  {
    attrs_.clear();
  }

  bool HtmlAttrs::empty() const noexcept
  {
    return attrs_.empty();
  }

  std::size_t HtmlAttrs::size() const noexcept
  {
    return attrs_.size();
  }

  const std::map<std::string, HtmlAttr> &HtmlAttrs::values() const noexcept
  {
    return attrs_;
  }

  std::map<std::string, HtmlAttr> &HtmlAttrs::values() noexcept
  {
    return attrs_;
  }

  std::string HtmlAttrs::render() const
  {
    std::string output;
    bool first = true;

    for (const auto &[name, attr] : attrs_)
    {
      if (!first)
      {
        output.push_back(' ');
      }

      first = false;

      output += name;

      if (!attr.boolean)
      {
        output += "=\"";
        output += HtmlEscape::attr(attr.value);
        output += "\"";
      }
    }

    return output;
  }

  std::string HtmlAttrs::render_with_leading_space() const
  {
    if (attrs_.empty())
    {
      return {};
    }

    return " " + render();
  }

  bool HtmlAttrs::is_valid_name(std::string_view name) noexcept
  {
    if (name.empty())
    {
      return false;
    }

    const unsigned char first = static_cast<unsigned char>(name.front());

    if (!std::isalpha(first) && name.front() != '_' && name.front() != ':')
    {
      return false;
    }

    for (char ch : name)
    {
      const unsigned char c = static_cast<unsigned char>(ch);

      if (std::isalnum(c) ||
          ch == '_' ||
          ch == '-' ||
          ch == ':' ||
          ch == '.')
      {
        continue;
      }

      return false;
    }

    return true;
  }

} // namespace vix::ui
