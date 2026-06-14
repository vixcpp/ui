/**
 *
 *  @file ViewContext.cpp
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
#include <vix/ui/core/ViewContext.hpp>

#include <utility>

namespace vix::ui
{
  ViewContext::ViewContext(vix::template_::Object values)
      : values_(std::move(values))
  {
  }

  ViewContext &ViewContext::set(
      std::string key,
      vix::template_::Value value)
  {
    values_[std::move(key)] = std::move(value);
    return *this;
  }

  bool ViewContext::has(const std::string &key) const
  {
    return values_.find(key) != values_.end();
  }

  const vix::template_::Value *ViewContext::get(
      const std::string &key) const noexcept
  {
    const auto it = values_.find(key);
    if (it == values_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  vix::template_::Value *ViewContext::get(
      const std::string &key) noexcept
  {
    const auto it = values_.find(key);
    if (it == values_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  vix::template_::Value ViewContext::get_or(
      const std::string &key,
      vix::template_::Value fallback) const
  {
    const auto it = values_.find(key);
    if (it == values_.end())
    {
      return fallback;
    }

    return it->second;
  }

  bool ViewContext::erase(const std::string &key)
  {
    return values_.erase(key) > 0;
  }

  void ViewContext::clear() noexcept
  {
    values_.clear();
  }

  bool ViewContext::empty() const noexcept
  {
    return values_.empty();
  }

  std::size_t ViewContext::size() const noexcept
  {
    return values_.size();
  }

  const vix::template_::Object &ViewContext::values() const noexcept
  {
    return values_;
  }

  vix::template_::Object &ViewContext::values() noexcept
  {
    return values_;
  }

  vix::template_::Context ViewContext::to_template_context() const
  {
    return vix::template_::Context(values_);
  }

} // namespace vix::ui
