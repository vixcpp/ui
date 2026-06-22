/**
 *
 *  @file FormData.cpp
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
#include <vix/ui/forms/FormData.hpp>

#include <algorithm>
#include <utility>

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] std::string make_key(std::string_view name)
    {
      return std::string(name);
    }
  } // namespace

  FormData::FormData(std::map<std::string, std::string> values)
  {
    for (auto &[name, value] : values)
    {
      set(std::move(name), std::move(value));
    }
  }

  FormData FormData::make()
  {
    return FormData{};
  }

  FormData FormData::from(std::map<std::string, std::string> values)
  {
    return FormData(std::move(values));
  }

  FormData &FormData::set(std::string name, std::string value)
  {
    values_[std::move(name)] = {std::move(value)};
    return *this;
  }

  FormData &FormData::add(std::string name, std::string value)
  {
    values_[std::move(name)].push_back(std::move(value));
    return *this;
  }

  FormData &FormData::set_all(
      std::string name,
      std::vector<std::string> values)
  {
    values_[std::move(name)] = std::move(values);
    return *this;
  }

  bool FormData::has(std::string_view name) const noexcept
  {
    return values_.find(make_key(name)) != values_.end();
  }

  bool FormData::contains(
      std::string_view name,
      std::string_view value) const noexcept
  {
    const auto *items = get_all(name);

    if (items == nullptr)
    {
      return false;
    }

    return std::find(
               items->begin(),
               items->end(),
               value) != items->end();
  }

  const std::string *FormData::get(
      std::string_view name) const noexcept
  {
    const auto it = values_.find(make_key(name));

    if (it == values_.end() || it->second.empty())
    {
      return nullptr;
    }

    return &it->second.front();
  }

  std::string FormData::get_or(
      std::string_view name,
      std::string fallback) const
  {
    const std::string *value = get(name);

    if (value == nullptr)
    {
      return fallback;
    }

    return *value;
  }

  const std::vector<std::string> *FormData::get_all(
      std::string_view name) const noexcept
  {
    const auto it = values_.find(make_key(name));

    if (it == values_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  std::vector<std::string> FormData::get_all_or_empty(
      std::string_view name) const
  {
    const auto *items = get_all(name);

    if (items == nullptr)
    {
      return {};
    }

    return *items;
  }

  bool FormData::erase(std::string_view name)
  {
    return values_.erase(make_key(name)) > 0;
  }

  void FormData::clear() noexcept
  {
    values_.clear();
  }

  bool FormData::empty() const noexcept
  {
    return values_.empty();
  }

  std::size_t FormData::size() const noexcept
  {
    return values_.size();
  }

  std::size_t FormData::value_count(
      std::string_view name) const noexcept
  {
    const auto *items = get_all(name);

    if (items == nullptr)
    {
      return 0;
    }

    return items->size();
  }

  const std::map<std::string, std::vector<std::string>> &
  FormData::values() const noexcept
  {
    return values_;
  }

  std::map<std::string, std::vector<std::string>> &
  FormData::values() noexcept
  {
    return values_;
  }

} // namespace vix::ui
