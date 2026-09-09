/**
 *
 *  @file MobileProject.cpp
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
#include <vix/ui/mobile/MobileProject.hpp>

#include <utility>

namespace vix::ui
{
  MobileProject::MobileProject(MobileConfig config)
      : config_(std::move(config))
  {
  }

  const MobileConfig &MobileProject::config() const noexcept
  {
    return config_;
  }

  MobileConfig &MobileProject::config() noexcept
  {
    return config_;
  }

  Result<void> MobileProject::validate() const
  {
    return config_.validate();
  }

} // namespace vix::ui
