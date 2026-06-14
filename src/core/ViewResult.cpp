/**
 *
 *  @file ViewResult.cpp
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
#include <vix/ui/core/ViewResult.hpp>

#include <utility>

namespace vix::ui
{
  ViewResult::ViewResult(
      std::string name,
      const vix::template_::RenderResult &result)
      : template_name(std::move(name)),
        output(result.output),
        from_cache(result.from_cache),
        escaped(result.escaped),
        success(result.success)
  {
  }

  ViewResult ViewResult::ok(
      std::string name,
      std::string html)
  {
    ViewResult result;
    result.template_name = std::move(name);
    result.output = std::move(html);
    result.success = true;
    return result;
  }

  ViewResult ViewResult::failed(std::string name)
  {
    ViewResult result;
    result.template_name = std::move(name);
    result.success = false;
    return result;
  }

  bool ViewResult::is_ok() const noexcept
  {
    return success;
  }

  bool ViewResult::is_failed() const noexcept
  {
    return !success;
  }

  ViewResult::operator bool() const noexcept
  {
    return is_ok();
  }

} // namespace vix::ui
