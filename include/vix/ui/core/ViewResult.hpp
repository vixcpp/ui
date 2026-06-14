/**
 *
 *  @file ViewResult.hpp
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
#ifndef VIX_UI_CORE_VIEW_RESULT_HPP
#define VIX_UI_CORE_VIEW_RESULT_HPP

#include <string>

#include <vix/template/RenderResult.hpp>

namespace vix::ui
{
  /**
   * @brief Result of rendering a UI view.
   *
   * ViewResult stores the rendered output and metadata returned by the
   * underlying template rendering layer.
   *
   * It is intentionally close to `vix::template_::RenderResult`, but adds
   * the view/template name so higher-level UI helpers can report which
   * view was rendered.
   */
  struct ViewResult
  {
    /**
     * @brief Template name associated with the rendered view.
     */
    std::string template_name;

    /**
     * @brief Final rendered HTML output.
     */
    std::string output;

    /**
     * @brief Whether the rendered view came from the template cache.
     */
    bool from_cache{false};

    /**
     * @brief Whether HTML escaping was applied during rendering.
     */
    bool escaped{false};

    /**
     * @brief Whether rendering completed successfully.
     */
    bool success{true};

    /**
     * @brief Construct an empty successful view result.
     */
    ViewResult() = default;

    /**
     * @brief Construct a view result from a template render result.
     *
     * @param name Template name.
     * @param result Template render result.
     */
    ViewResult(
        std::string name,
        const vix::template_::RenderResult &result);

    /**
     * @brief Construct a successful view result.
     *
     * @param name Template name.
     * @param html Rendered HTML output.
     * @return Successful view result.
     */
    [[nodiscard]] static ViewResult ok(
        std::string name,
        std::string html);

    /**
     * @brief Construct a failed view result.
     *
     * @param name Template name.
     * @return Failed view result.
     */
    [[nodiscard]] static ViewResult failed(std::string name);

    /**
     * @brief Check whether rendering succeeded.
     *
     * @return True if successful.
     */
    [[nodiscard]] bool is_ok() const noexcept;

    /**
     * @brief Check whether rendering failed.
     *
     * @return True if failed.
     */
    [[nodiscard]] bool is_failed() const noexcept;

    /**
     * @brief Convert the result to bool.
     *
     * @return True if rendering succeeded.
     */
    [[nodiscard]] explicit operator bool() const noexcept;
  };

} // namespace vix::ui

#endif // VIX_UI_CORE_VIEW_RESULT_HPP
