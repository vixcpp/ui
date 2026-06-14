/**
 *
 *  @file View.cpp
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
#include <vix/ui/core/View.hpp>
#include <vix/ui/support/Error.hpp>

#include <utility>

namespace vix::ui
{
  View::View(std::string template_name)
      : data_(std::move(template_name))
  {
  }

  View::View(ViewData data)
      : data_(std::move(data))
  {
  }

  View View::make(std::string template_name)
  {
    return View(std::move(template_name));
  }

  View &View::set_template(std::string template_name)
  {
    data_.template_name = std::move(template_name);
    return *this;
  }

  View &View::set_layout(std::string layout_name)
  {
    data_.layout_name = std::move(layout_name);
    return *this;
  }

  View &View::set_title(std::string title)
  {
    data_.title = std::move(title);
    data_.values["page_title"] = data_.title;
    return *this;
  }

  View &View::set(
      std::string key,
      vix::template_::Value value)
  {
    data_.values[std::move(key)] = std::move(value);
    return *this;
  }

  View &View::merge(vix::template_::Object values)
  {
    for (auto &[key, value] : values)
    {
      data_.values[std::move(key)] = std::move(value);
    }

    return *this;
  }

  bool View::has(const std::string &key) const
  {
    return data_.values.find(key) != data_.values.end();
  }

  const vix::template_::Value *View::get(
      const std::string &key) const noexcept
  {
    const auto it = data_.values.find(key);
    if (it == data_.values.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  vix::template_::Value *View::get(
      const std::string &key) noexcept
  {
    const auto it = data_.values.find(key);
    if (it == data_.values.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  bool View::erase(const std::string &key)
  {
    return data_.values.erase(key) > 0;
  }

  void View::clear_values() noexcept
  {
    data_.values.clear();
  }

  const std::string &View::template_name() const noexcept
  {
    return data_.template_name;
  }

  const std::string &View::layout_name() const noexcept
  {
    return data_.layout_name;
  }

  const std::string &View::title() const noexcept
  {
    return data_.title;
  }

  bool View::has_template() const noexcept
  {
    return data_.has_template();
  }

  bool View::has_layout() const noexcept
  {
    return data_.has_layout();
  }

  bool View::has_title() const noexcept
  {
    return data_.has_title();
  }

  const ViewData &View::data() const noexcept
  {
    return data_;
  }

  ViewData &View::data() noexcept
  {
    return data_;
  }

  const vix::template_::Object &View::values() const noexcept
  {
    return data_.values;
  }

  vix::template_::Object &View::values() noexcept
  {
    return data_.values;
  }

  ViewContext View::context() const
  {
    return ViewContext(data_.values);
  }

  vix::template_::Context View::to_template_context() const
  {
    return vix::template_::Context(data_.values);
  }

  ViewResult View::render(vix::template_::Engine &engine) const
  {
    if (data_.template_name.empty())
    {
      throw ViewError("cannot render UI view without a template name");
    }

    const vix::template_::RenderResult result =
        engine.render(data_.template_name, to_template_context());

    return ViewResult(data_.template_name, result);
  }

} // namespace vix::ui
