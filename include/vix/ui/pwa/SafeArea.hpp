/**
 *
 *  @file SafeArea.hpp
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
#ifndef VIX_UI_PWA_SAFE_AREA_HPP
#define VIX_UI_PWA_SAFE_AREA_HPP

#include <string>
#include <string_view>

namespace vix::ui
{
  /**
   * @brief Safe-area inset edge.
   *
   * SafeAreaEdge identifies one mobile safe-area inset direction.
   */
  enum class SafeAreaEdge
  {
    Top = 0,
    Right,
    Bottom,
    Left
  };

  /**
   * @brief Convert a safe-area edge to a stable string.
   *
   * @param edge Safe-area edge.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(SafeAreaEdge edge) noexcept;

  /**
   * @brief Mobile safe-area CSS helper.
   *
   * SafeArea generates small CSS helpers for mobile browsers and
   * installable web apps that need to account for screen cutouts, rounded
   * corners, browser UI, and device safe-area insets.
   *
   * It does not inspect the current device and does not require a browser
   * runtime. It only generates CSS strings based on the standard
   * env(safe-area-inset-*) variables.
   */
  class SafeArea
  {
  public:
    /**
     * @brief Construct a default safe-area helper.
     */
    SafeArea() = default;

    /**
     * @brief Create a default safe-area helper.
     *
     * @return Safe-area helper.
     */
    [[nodiscard]] static SafeArea make();

    /**
     * @brief Create a safe-area helper with all default insets enabled.
     *
     * @return Safe-area helper.
     */
    [[nodiscard]] static SafeArea all();

    /**
     * @brief Create a safe-area helper focused on vertical insets.
     *
     * Top and bottom keep their default values, left and right are cleared.
     *
     * @return Vertical safe-area helper.
     */
    [[nodiscard]] static SafeArea vertical();

    /**
     * @brief Set the CSS selector used for root variables.
     *
     * @param selector Root selector.
     * @return This safe-area helper.
     */
    SafeArea &set_root_selector(std::string selector);

    /**
     * @brief Set the CSS class name used for padding helpers.
     *
     * @param class_name CSS class name without the leading dot.
     * @return This safe-area helper.
     */
    SafeArea &set_class_name(std::string class_name);

    /**
     * @brief Set the CSS variable prefix.
     *
     * Example:
     *
     * @code
     * --vix-safe-area
     * @endcode
     *
     * @param prefix CSS variable prefix.
     * @return This safe-area helper.
     */
    SafeArea &set_variable_prefix(std::string prefix);

    /**
     * @brief Set the top safe-area inset value.
     *
     * @param value CSS value.
     * @return This safe-area helper.
     */
    SafeArea &set_top(std::string value);

    /**
     * @brief Set the right safe-area inset value.
     *
     * @param value CSS value.
     * @return This safe-area helper.
     */
    SafeArea &set_right(std::string value);

    /**
     * @brief Set the bottom safe-area inset value.
     *
     * @param value CSS value.
     * @return This safe-area helper.
     */
    SafeArea &set_bottom(std::string value);

    /**
     * @brief Set the left safe-area inset value.
     *
     * @param value CSS value.
     * @return This safe-area helper.
     */
    SafeArea &set_left(std::string value);

    /**
     * @brief Set one safe-area inset value.
     *
     * @param edge Safe-area edge.
     * @param value CSS value.
     * @return This safe-area helper.
     */
    SafeArea &set(SafeAreaEdge edge, std::string value);

    /**
     * @brief Get the root selector.
     *
     * @return Root selector.
     */
    [[nodiscard]] const std::string &root_selector() const noexcept;

    /**
     * @brief Get the padding helper class name.
     *
     * @return CSS class name.
     */
    [[nodiscard]] const std::string &class_name() const noexcept;

    /**
     * @brief Get the CSS variable prefix.
     *
     * @return CSS variable prefix.
     */
    [[nodiscard]] const std::string &variable_prefix() const noexcept;

    /**
     * @brief Get the top inset value.
     *
     * @return Top inset value.
     */
    [[nodiscard]] const std::string &top() const noexcept;

    /**
     * @brief Get the right inset value.
     *
     * @return Right inset value.
     */
    [[nodiscard]] const std::string &right() const noexcept;

    /**
     * @brief Get the bottom inset value.
     *
     * @return Bottom inset value.
     */
    [[nodiscard]] const std::string &bottom() const noexcept;

    /**
     * @brief Get the left inset value.
     *
     * @return Left inset value.
     */
    [[nodiscard]] const std::string &left() const noexcept;

    /**
     * @brief Get one safe-area inset value.
     *
     * @param edge Safe-area edge.
     * @return Inset value.
     */
    [[nodiscard]] const std::string &get(SafeAreaEdge edge) const noexcept;

    /**
     * @brief Check whether the root selector is set.
     *
     * @return True if root selector is not empty.
     */
    [[nodiscard]] bool has_root_selector() const noexcept;

    /**
     * @brief Check whether the class name is set.
     *
     * @return True if class name is not empty.
     */
    [[nodiscard]] bool has_class_name() const noexcept;

    /**
     * @brief Check whether the variable prefix is set.
     *
     * @return True if variable prefix is not empty.
     */
    [[nodiscard]] bool has_variable_prefix() const noexcept;

    /**
     * @brief Check whether the top inset is set.
     *
     * @return True if top is not empty.
     */
    [[nodiscard]] bool has_top() const noexcept;

    /**
     * @brief Check whether the right inset is set.
     *
     * @return True if right is not empty.
     */
    [[nodiscard]] bool has_right() const noexcept;

    /**
     * @brief Check whether the bottom inset is set.
     *
     * @return True if bottom is not empty.
     */
    [[nodiscard]] bool has_bottom() const noexcept;

    /**
     * @brief Check whether the left inset is set.
     *
     * @return True if left is not empty.
     */
    [[nodiscard]] bool has_left() const noexcept;

    /**
     * @brief Build the CSS variable name for one edge.
     *
     * @param edge Safe-area edge.
     * @return CSS variable name.
     */
    [[nodiscard]] std::string variable_name(SafeAreaEdge edge) const;

    /**
     * @brief Build the CSS variable reference for one edge.
     *
     * @param edge Safe-area edge.
     * @return CSS var() expression.
     */
    [[nodiscard]] std::string variable_ref(SafeAreaEdge edge) const;

    /**
     * @brief Render safe-area CSS variables.
     *
     * @return CSS variable block.
     */
    [[nodiscard]] std::string render_variables() const;

    /**
     * @brief Render the padding helper class.
     *
     * @return CSS class block.
     */
    [[nodiscard]] std::string render_class() const;

    /**
     * @brief Render variables and helper class together.
     *
     * @return Complete safe-area CSS helper.
     */
    [[nodiscard]] std::string render() const;

  private:
    /**
     * @brief Root selector for CSS variables.
     */
    std::string root_selector_{":root"};

    /**
     * @brief CSS class name for padding helpers.
     */
    std::string class_name_{"vix-safe-area"};

    /**
     * @brief CSS variable prefix.
     */
    std::string variable_prefix_{"--vix-safe-area"};

    /**
     * @brief Top safe-area inset value.
     */
    std::string top_{"env(safe-area-inset-top)"};

    /**
     * @brief Right safe-area inset value.
     */
    std::string right_{"env(safe-area-inset-right)"};

    /**
     * @brief Bottom safe-area inset value.
     */
    std::string bottom_{"env(safe-area-inset-bottom)"};

    /**
     * @brief Left safe-area inset value.
     */
    std::string left_{"env(safe-area-inset-left)"};
  };

} // namespace vix::ui

#endif // VIX_UI_PWA_SAFE_AREA_HPP
