/**
 *
 *  @file Version.cpp
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
#include <vix/ui/Version.hpp>

namespace vix::ui
{
  int version_major() noexcept
  {
    return VERSION_MAJOR;
  }

  int version_minor() noexcept
  {
    return VERSION_MINOR;
  }

  int version_patch() noexcept
  {
    return VERSION_PATCH;
  }

  std::string_view version() noexcept
  {
    return VERSION;
  }

} // namespace vix::ui
