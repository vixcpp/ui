
/**
 *
 *  @file LiveUpdate.hpp
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
#ifndef VIX_UI_LIVE_LIVE_UPDATE_HPP
#define VIX_UI_LIVE_LIVE_UPDATE_HPP

#include <string>
#include <string_view>

#include <vix/ui/live/Fragment.hpp>

namespace vix::ui
{
  /**
   * @brief Supported live UI update operations.
   *
   * LiveUpdateAction describes how a client should apply a server-rendered
   * fragment to the current document.
   */
  enum class LiveUpdateAction
  {
    Replace = 0,
    Append,
    Prepend,
    Before,
    After,
    Remove,
    None
  };

  /**
   * @brief Convert a live update action to a stable string.
   *
   * @param action Live update action.
   * @return String representation.
   */
  [[nodiscard]] std::string_view to_string(
      LiveUpdateAction action) noexcept;

  /**
   * @brief WebSocket-friendly UI update payload.
   *
   * LiveUpdate represents a small server-driven UI instruction:
   * - which target should be updated
   * - which action should be applied
   * - which HTML fragment should be used
   *
   * It does not require WebSocket directly. The result can be returned from
   * a route, sent through a WebSocket, logged, tested, or consumed by any
   * client-side adapter.
   */
  class LiveUpdate
  {
  public:
    /**
     * @brief Construct an empty live update.
     */
    LiveUpdate() = default;

    /**
     * @brief Construct a live update.
     *
     * @param action Update action.
     * @param target CSS-like target selector.
     * @param fragment HTML fragment.
     */
    LiveUpdate(
        LiveUpdateAction action,
        std::string target,
        Fragment fragment = {});

    /**
     * @brief Create a replace update.
     *
     * @param target Target selector.
     * @param fragment Fragment used as replacement.
     * @return Live update.
     */
    [[nodiscard]] static LiveUpdate replace(
        std::string target,
        Fragment fragment);

    /**
     * @brief Create an append update.
     *
     * @param target Target selector.
     * @param fragment Fragment appended inside the target.
     * @return Live update.
     */
    [[nodiscard]] static LiveUpdate append(
        std::string target,
        Fragment fragment);

    /**
     * @brief Create a prepend update.
     *
     * @param target Target selector.
     * @param fragment Fragment prepended inside the target.
     * @return Live update.
     */
    [[nodiscard]] static LiveUpdate prepend(
        std::string target,
        Fragment fragment);

    /**
     * @brief Create a before update.
     *
     * @param target Target selector.
     * @param fragment Fragment inserted before the target.
     * @return Live update.
     */
    [[nodiscard]] static LiveUpdate before(
        std::string target,
        Fragment fragment);

    /**
     * @brief Create an after update.
     *
     * @param target Target selector.
     * @param fragment Fragment inserted after the target.
     * @return Live update.
     */
    [[nodiscard]] static LiveUpdate after(
        std::string target,
        Fragment fragment);

    /**
     * @brief Create a remove update.
     *
     * @param target Target selector.
     * @return Live update.
     */
    [[nodiscard]] static LiveUpdate remove(std::string target);

    /**
     * @brief Create a no-op update.
     *
     * @return Live update.
     */
    [[nodiscard]] static LiveUpdate none();

    /**
     * @brief Set the update action.
     *
     * @param action Update action.
     * @return This update.
     */
    LiveUpdate &set_action(LiveUpdateAction action) noexcept;

    /**
     * @brief Set the update target selector.
     *
     * @param target Target selector.
     * @return This update.
     */
    LiveUpdate &set_target(std::string target);

    /**
     * @brief Set the update fragment.
     *
     * @param fragment Fragment payload.
     * @return This update.
     */
    LiveUpdate &set_fragment(Fragment fragment);

    /**
     * @brief Set the update event name.
     *
     * The event name is optional metadata useful for WebSocket clients.
     *
     * @param event Event name.
     * @return This update.
     */
    LiveUpdate &set_event(std::string event);

    /**
     * @brief Set an optional request or correlation id.
     *
     * @param id Correlation id.
     * @return This update.
     */
    LiveUpdate &set_id(std::string id);

    /**
     * @brief Get the update action.
     *
     * @return Update action.
     */
    [[nodiscard]] LiveUpdateAction action() const noexcept;

    /**
     * @brief Get the target selector.
     *
     * @return Target selector.
     */
    [[nodiscard]] const std::string &target() const noexcept;

    /**
     * @brief Get the fragment.
     *
     * @return Fragment payload.
     */
    [[nodiscard]] const Fragment &fragment() const noexcept;

    /**
     * @brief Get the mutable fragment.
     *
     * @return Fragment payload.
     */
    [[nodiscard]] Fragment &fragment() noexcept;

    /**
     * @brief Get the optional event name.
     *
     * @return Event name.
     */
    [[nodiscard]] const std::string &event() const noexcept;

    /**
     * @brief Get the optional correlation id.
     *
     * @return Correlation id.
     */
    [[nodiscard]] const std::string &id() const noexcept;

    /**
     * @brief Check whether the update has a target selector.
     *
     * @return True if target is not empty.
     */
    [[nodiscard]] bool has_target() const noexcept;

    /**
     * @brief Check whether the update has a fragment with HTML.
     *
     * @return True if the fragment contains HTML.
     */
    [[nodiscard]] bool has_fragment() const noexcept;

    /**
     * @brief Check whether the update has an event name.
     *
     * @return True if event is not empty.
     */
    [[nodiscard]] bool has_event() const noexcept;

    /**
     * @brief Check whether the update has a correlation id.
     *
     * @return True if id is not empty.
     */
    [[nodiscard]] bool has_id() const noexcept;

    /**
     * @brief Check whether the update removes a target.
     *
     * @return True if the action is Remove.
     */
    [[nodiscard]] bool removes_target() const noexcept;

    /**
     * @brief Check whether the update is a no-op.
     *
     * @return True if the action is None.
     */
    [[nodiscard]] bool none_action() const noexcept;

    /**
     * @brief Render the fragment HTML.
     *
     * For Remove and None actions, this returns an empty string.
     *
     * @return Rendered HTML.
     */
    [[nodiscard]] std::string render() const;

    /**
     * @brief Serialize the update to a small JSON payload.
     *
     * The output is dependency-free and intentionally simple so it can be
     * sent through a WebSocket or returned from an endpoint.
     *
     * @return JSON payload.
     */
    [[nodiscard]] std::string to_json() const;

  private:
    /**
     * @brief Update action.
     */
    LiveUpdateAction action_{LiveUpdateAction::None};

    /**
     * @brief CSS-like target selector.
     */
    std::string target_;

    /**
     * @brief Fragment payload.
     */
    Fragment fragment_;

    /**
     * @brief Optional event name.
     */
    std::string event_;

    /**
     * @brief Optional correlation id.
     */
    std::string id_;
  };

} // namespace vix::ui

#endif // VIX_UI_LIVE_LIVE_UPDATE_HPP
