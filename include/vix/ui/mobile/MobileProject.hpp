/**
 *
 *  @file MobileProject.hpp
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
#ifndef VIX_UI_MOBILE_MOBILE_PROJECT_HPP
#define VIX_UI_MOBILE_MOBILE_PROJECT_HPP

#include <vix/ui/mobile/MobileConfig.hpp>

namespace vix::ui
{
  /**
   * @brief Common mobile project boundary.
   *
   * MobileProject owns the platform-independent configuration shared by
   * mobile project generators. It is a value boundary, not a polymorphic
   * generation interface: platform-specific generation belongs to their
   * respective backends.
   */
  class MobileProject
  {
  public:
    /**
     * @brief Construct a mobile project with the default configuration.
     */
    MobileProject() = default;

    /**
     * @brief Construct a mobile project from a configuration.
     *
     * The configuration is owned by value, so later changes to the argument
     * do not affect this project.
     *
     * @param config Mobile application configuration.
     */
    explicit MobileProject(MobileConfig config);

    /**
     * @brief Access the mobile project configuration.
     *
     * @return Immutable mobile configuration.
     */
    [[nodiscard]] const MobileConfig &config() const noexcept;

    /**
     * @brief Access the mobile project configuration.
     *
     * @return Mutable mobile configuration.
     */
    [[nodiscard]] MobileConfig &config() noexcept;

    /**
     * @brief Validate the mobile project configuration.
     *
     * @return Successful result when the mobile configuration is valid.
     */
    [[nodiscard]] Result<void> validate() const;

  private:
    MobileConfig config_;
  };

} // namespace vix::ui

#endif // VIX_UI_MOBILE_MOBILE_PROJECT_HPP
