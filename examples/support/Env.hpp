/**
 *
 *  @file Env.hpp
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
#ifndef VIX_UI_EXAMPLES_SUPPORT_ENV_HPP
#define VIX_UI_EXAMPLES_SUPPORT_ENV_HPP

#include <cstdlib>
#include <string>

namespace vix::ui::examples
{
  [[nodiscard]] inline std::string env_or_empty(const char *name)
  {
    if (name == nullptr)
    {
      return {};
    }

#if defined(_WIN32)
    char *buffer = nullptr;
    std::size_t length = 0;

    if (_dupenv_s(&buffer, &length, name) != 0 || buffer == nullptr)
    {
      return {};
    }

    std::string value(buffer);
    std::free(buffer);

    return value;
#else
    const char *value = std::getenv(name);

    if (value == nullptr)
    {
      return {};
    }

    return std::string(value);
#endif
  }

} // namespace vix::ui::examples

#endif // VIX_UI_EXAMPLES_SUPPORT_ENV_HPP
