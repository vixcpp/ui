/**
 *
 *  @file LiveUpdate.cpp
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
#include <vix/ui/live/LiveUpdate.hpp>

#include <utility>

namespace vix::ui
{
  namespace
  {
    void append_json_string(std::string &output, std::string_view value)
    {
      output.push_back('"');

      for (char ch : value)
      {
        switch (ch)
        {
        case '"':
          output += "\\\"";
          break;

        case '\\':
          output += "\\\\";
          break;

        case '\b':
          output += "\\b";
          break;

        case '\f':
          output += "\\f";
          break;

        case '\n':
          output += "\\n";
          break;

        case '\r':
          output += "\\r";
          break;

        case '\t':
          output += "\\t";
          break;

        default:
          output.push_back(ch);
          break;
        }
      }

      output.push_back('"');
    }

    void append_json_field(
        std::string &output,
        std::string_view name,
        std::string_view value,
        bool &first)
    {
      if (!first)
      {
        output.push_back(',');
      }

      first = false;

      append_json_string(output, name);
      output.push_back(':');
      append_json_string(output, value);
    }
  } // namespace

  std::string_view to_string(LiveUpdateAction action) noexcept
  {
    switch (action)
    {
    case LiveUpdateAction::Replace:
      return "replace";

    case LiveUpdateAction::Append:
      return "append";

    case LiveUpdateAction::Prepend:
      return "prepend";

    case LiveUpdateAction::Before:
      return "before";

    case LiveUpdateAction::After:
      return "after";

    case LiveUpdateAction::Remove:
      return "remove";

    case LiveUpdateAction::None:
      return "none";
    }

    return "none";
  }

  LiveUpdate::LiveUpdate(
      LiveUpdateAction action,
      std::string target,
      Fragment fragment)
      : action_(action),
        target_(std::move(target)),
        fragment_(std::move(fragment))
  {
  }

  LiveUpdate LiveUpdate::replace(
      std::string target,
      Fragment fragment)
  {
    return LiveUpdate(
        LiveUpdateAction::Replace,
        std::move(target),
        std::move(fragment));
  }

  LiveUpdate LiveUpdate::append(
      std::string target,
      Fragment fragment)
  {
    return LiveUpdate(
        LiveUpdateAction::Append,
        std::move(target),
        std::move(fragment));
  }

  LiveUpdate LiveUpdate::prepend(
      std::string target,
      Fragment fragment)
  {
    return LiveUpdate(
        LiveUpdateAction::Prepend,
        std::move(target),
        std::move(fragment));
  }

  LiveUpdate LiveUpdate::before(
      std::string target,
      Fragment fragment)
  {
    return LiveUpdate(
        LiveUpdateAction::Before,
        std::move(target),
        std::move(fragment));
  }

  LiveUpdate LiveUpdate::after(
      std::string target,
      Fragment fragment)
  {
    return LiveUpdate(
        LiveUpdateAction::After,
        std::move(target),
        std::move(fragment));
  }

  LiveUpdate LiveUpdate::remove(std::string target)
  {
    return LiveUpdate(
        LiveUpdateAction::Remove,
        std::move(target),
        Fragment{});
  }

  LiveUpdate LiveUpdate::none()
  {
    return LiveUpdate{};
  }

  LiveUpdate &LiveUpdate::set_action(
      LiveUpdateAction action) noexcept
  {
    action_ = action;
    return *this;
  }

  LiveUpdate &LiveUpdate::set_target(std::string target)
  {
    target_ = std::move(target);
    return *this;
  }

  LiveUpdate &LiveUpdate::set_fragment(Fragment fragment)
  {
    fragment_ = std::move(fragment);
    return *this;
  }

  LiveUpdate &LiveUpdate::set_event(std::string event)
  {
    event_ = std::move(event);
    return *this;
  }

  LiveUpdate &LiveUpdate::set_id(std::string id)
  {
    id_ = std::move(id);
    return *this;
  }

  LiveUpdateAction LiveUpdate::action() const noexcept
  {
    return action_;
  }

  const std::string &LiveUpdate::target() const noexcept
  {
    return target_;
  }

  const Fragment &LiveUpdate::fragment() const noexcept
  {
    return fragment_;
  }

  Fragment &LiveUpdate::fragment() noexcept
  {
    return fragment_;
  }

  const std::string &LiveUpdate::event() const noexcept
  {
    return event_;
  }

  const std::string &LiveUpdate::id() const noexcept
  {
    return id_;
  }

  bool LiveUpdate::has_target() const noexcept
  {
    return !target_.empty();
  }

  bool LiveUpdate::has_fragment() const noexcept
  {
    return fragment_.has_html();
  }

  bool LiveUpdate::has_event() const noexcept
  {
    return !event_.empty();
  }

  bool LiveUpdate::has_id() const noexcept
  {
    return !id_.empty();
  }

  bool LiveUpdate::removes_target() const noexcept
  {
    return action_ == LiveUpdateAction::Remove;
  }

  bool LiveUpdate::none_action() const noexcept
  {
    return action_ == LiveUpdateAction::None;
  }

  std::string LiveUpdate::render() const
  {
    if (removes_target() || none_action())
    {
      return {};
    }

    return fragment_.render();
  }

  std::string LiveUpdate::to_json() const
  {
    std::string output;
    bool first = true;

    output.push_back('{');

    append_json_field(output, "type", "ui.update", first);
    append_json_field(output, "action", to_string(action_), first);

    if (has_target())
    {
      append_json_field(output, "target", target_, first);
    }

    if (has_event())
    {
      append_json_field(output, "event", event_, first);
    }

    if (has_id())
    {
      append_json_field(output, "id", id_, first);
    }

    if (fragment_.has_name())
    {
      append_json_field(output, "fragment", fragment_.name(), first);
    }

    if (!removes_target() && !none_action())
    {
      append_json_field(output, "html", fragment_.render(), first);
    }

    output.push_back('}');

    return output;
  }

} // namespace vix::ui
