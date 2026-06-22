/**
 *
 *  @file toast_test.cpp
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
#include <cassert>
#include <chrono>
#include <iostream>
#include <string>

#include <vix/ui/live/Toast.hpp>

using namespace vix::ui;

static void test_toast_position_to_string()
{
  assert(to_string(ToastPosition::TopRight) == "top-right");
  assert(to_string(ToastPosition::TopLeft) == "top-left");
  assert(to_string(ToastPosition::BottomRight) == "bottom-right");
  assert(to_string(ToastPosition::BottomLeft) == "bottom-left");
  assert(to_string(ToastPosition::TopCenter) == "top-center");
  assert(to_string(ToastPosition::BottomCenter) == "bottom-center");
}

static void test_empty_toast()
{
  Toast toast;

  assert(toast.level() == FlashLevel::Info);
  assert(toast.message().empty());
  assert(toast.title().empty());
  assert(toast.position() == ToastPosition::TopRight);
  assert(toast.dismissible());
  assert(toast.auto_hide());
  assert(toast.timeout() == std::chrono::milliseconds(5000));
  assert(toast.attrs().empty());

  assert(!toast.has_message());
  assert(!toast.has_title());
  assert(toast.empty());
}

static void test_construct_toast()
{
  Toast toast(FlashLevel::Success, "Saved successfully.");

  assert(toast.level() == FlashLevel::Success);
  assert(toast.message() == "Saved successfully.");
  assert(toast.has_message());
  assert(!toast.empty());
}

static void test_info_factory()
{
  Toast toast = Toast::info("Welcome back.");

  assert(toast.level() == FlashLevel::Info);
  assert(toast.message() == "Welcome back.");
}

static void test_success_factory()
{
  Toast toast = Toast::success("Profile saved.");

  assert(toast.level() == FlashLevel::Success);
  assert(toast.message() == "Profile saved.");
}

static void test_warning_factory()
{
  Toast toast = Toast::warning("Storage is almost full.");

  assert(toast.level() == FlashLevel::Warning);
  assert(toast.message() == "Storage is almost full.");
}

static void test_error_factory()
{
  Toast toast = Toast::error("Something went wrong.");

  assert(toast.level() == FlashLevel::Error);
  assert(toast.message() == "Something went wrong.");
}

static void test_neutral_factory()
{
  Toast toast = Toast::neutral("No changes detected.");

  assert(toast.level() == FlashLevel::Neutral);
  assert(toast.message() == "No changes detected.");
}

static void test_setters()
{
  Toast toast;

  toast.set_level(FlashLevel::Warning)
      .set_title("Warning")
      .set_message("Check your configuration.")
      .set_position(ToastPosition::BottomLeft)
      .set_dismissible(false)
      .set_auto_hide(false)
      .set_timeout(std::chrono::milliseconds(2500));

  assert(toast.level() == FlashLevel::Warning);
  assert(toast.title() == "Warning");
  assert(toast.message() == "Check your configuration.");
  assert(toast.position() == ToastPosition::BottomLeft);
  assert(!toast.dismissible());
  assert(!toast.auto_hide());
  assert(toast.timeout() == std::chrono::milliseconds(2500));

  assert(toast.has_title());
  assert(toast.has_message());
  assert(!toast.empty());
}

static void test_attrs()
{
  Toast toast = Toast::info("Hello");

  toast.set_attr("id", "toast-main");
  toast.set_bool_attr("hidden", true);

  assert(toast.attrs().has("id"));
  assert(toast.attrs().has("hidden"));
  assert(toast.attrs().size() == 2);
}

static void test_bool_attr_can_be_removed()
{
  Toast toast = Toast::info("Hello");

  toast.set_bool_attr("hidden", true);
  assert(toast.attrs().has("hidden"));

  toast.set_bool_attr("hidden", false);
  assert(!toast.attrs().has("hidden"));
}

static void test_render_info()
{
  Toast toast = Toast::info("Welcome back.");

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-info toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="info" data-toast-position="top-right" role="status"><span class="toast-message">Welcome back.</span></div>)");
}

static void test_render_success()
{
  Toast toast = Toast::success("Saved successfully.");

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-success toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="success" data-toast-position="top-right" role="status"><span class="toast-message">Saved successfully.</span></div>)");
}

static void test_render_warning()
{
  Toast toast = Toast::warning("Storage is almost full.");

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-warning toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="warning" data-toast-position="top-right" role="status"><span class="toast-message">Storage is almost full.</span></div>)");
}

static void test_render_error_uses_alert_role()
{
  Toast toast = Toast::error("Something went wrong.");

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-error toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="error" data-toast-position="top-right" role="alert"><span class="toast-message">Something went wrong.</span></div>)");
}

static void test_render_neutral()
{
  Toast toast = Toast::neutral("No changes detected.");

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-neutral toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="neutral" data-toast-position="top-right" role="status"><span class="toast-message">No changes detected.</span></div>)");
}

static void test_render_with_title()
{
  Toast toast = Toast::success("Profile updated.");

  toast.set_title("Saved");

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-success toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="success" data-toast-position="top-right" role="status"><strong class="toast-title">Saved</strong><span class="toast-message">Profile updated.</span></div>)");
}

static void test_render_position()
{
  Toast toast = Toast::info("Hello");

  toast.set_position(ToastPosition::BottomCenter);

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-info toast-bottom-center" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="info" data-toast-position="bottom-center" role="status"><span class="toast-message">Hello</span></div>)");
}

static void test_render_not_dismissible()
{
  Toast toast = Toast::info("Cannot close this.");

  toast.set_dismissible(false);

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-info toast-top-right" data-auto-hide data-timeout="5000" data-toast-level="info" data-toast-position="top-right" role="status"><span class="toast-message">Cannot close this.</span></div>)");
}

static void test_render_without_auto_hide()
{
  Toast toast = Toast::info("Persistent toast.");

  toast.set_auto_hide(false);

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-info toast-top-right" data-dismissible data-timeout="5000" data-toast-level="info" data-toast-position="top-right" role="status"><span class="toast-message">Persistent toast.</span></div>)");
}

static void test_render_with_timeout()
{
  Toast toast = Toast::info("Short timeout.");

  toast.set_timeout(std::chrono::milliseconds(1200));

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-info toast-top-right" data-auto-hide data-dismissible data-timeout="1200" data-toast-level="info" data-toast-position="top-right" role="status"><span class="toast-message">Short timeout.</span></div>)");
}

static void test_render_with_custom_attrs()
{
  Toast toast = Toast::info("Hello");

  toast.set_attr("id", "main-toast");
  toast.set_attr("class", "custom-toast");
  toast.set_bool_attr("hidden", true);

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="custom-toast" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="info" data-toast-position="top-right" hidden id="main-toast" role="status"><span class="toast-message">Hello</span></div>)");
}

static void test_render_escapes_title_and_message()
{
  Toast toast = Toast::error(R"(<failed & "bad">)");

  toast.set_title(R"(Error "main")");

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-error toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="error" data-toast-position="top-right" role="alert"><strong class="toast-title">Error &quot;main&quot;</strong><span class="toast-message">&lt;failed &amp; &quot;bad&quot;&gt;</span></div>)");
}

static void test_render_empty_message()
{
  Toast toast;

  const std::string rendered = toast.render();

  assert(
      rendered ==
      R"(<div class="toast toast-info toast-top-right" data-auto-hide data-dismissible data-timeout="5000" data-toast-level="info" data-toast-position="top-right" role="status"></div>)");
}

int main()
{
  test_toast_position_to_string();

  test_empty_toast();
  test_construct_toast();

  test_info_factory();
  test_success_factory();
  test_warning_factory();
  test_error_factory();
  test_neutral_factory();

  test_setters();
  test_attrs();
  test_bool_attr_can_be_removed();

  test_render_info();
  test_render_success();
  test_render_warning();
  test_render_error_uses_alert_role();
  test_render_neutral();

  test_render_with_title();
  test_render_position();
  test_render_not_dismissible();
  test_render_without_auto_hide();
  test_render_with_timeout();
  test_render_with_custom_attrs();
  test_render_escapes_title_and_message();
  test_render_empty_message();

  std::cout << "[OK] ui toast tests passed\n";
  return 0;
}
