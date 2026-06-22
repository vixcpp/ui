/**
 *
 *  @file flash_message_test.cpp
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
#include <iostream>
#include <string>

#include <vix/ui/live/FlashMessage.hpp>

using namespace vix::ui;

static void test_flash_level_to_string()
{
  assert(to_string(FlashLevel::Info) == "info");
  assert(to_string(FlashLevel::Success) == "success");
  assert(to_string(FlashLevel::Warning) == "warning");
  assert(to_string(FlashLevel::Error) == "error");
  assert(to_string(FlashLevel::Neutral) == "neutral");
}

static void test_empty_flash_message()
{
  FlashMessage flash;

  assert(flash.level() == FlashLevel::Info);
  assert(flash.message().empty());
  assert(flash.title().empty());
  assert(!flash.dismissible());
  assert(flash.attrs().empty());

  assert(!flash.has_message());
  assert(!flash.has_title());
  assert(flash.empty());
}

static void test_construct_flash_message()
{
  FlashMessage flash(FlashLevel::Success, "Saved successfully.");

  assert(flash.level() == FlashLevel::Success);
  assert(flash.message() == "Saved successfully.");
  assert(flash.has_message());
  assert(!flash.empty());
}

static void test_info_factory()
{
  FlashMessage flash = FlashMessage::info("Welcome back.");

  assert(flash.level() == FlashLevel::Info);
  assert(flash.message() == "Welcome back.");
}

static void test_success_factory()
{
  FlashMessage flash = FlashMessage::success("Profile saved.");

  assert(flash.level() == FlashLevel::Success);
  assert(flash.message() == "Profile saved.");
}

static void test_warning_factory()
{
  FlashMessage flash = FlashMessage::warning("Storage is almost full.");

  assert(flash.level() == FlashLevel::Warning);
  assert(flash.message() == "Storage is almost full.");
}

static void test_error_factory()
{
  FlashMessage flash = FlashMessage::error("Something went wrong.");

  assert(flash.level() == FlashLevel::Error);
  assert(flash.message() == "Something went wrong.");
}

static void test_neutral_factory()
{
  FlashMessage flash = FlashMessage::neutral("No changes detected.");

  assert(flash.level() == FlashLevel::Neutral);
  assert(flash.message() == "No changes detected.");
}

static void test_setters()
{
  FlashMessage flash;

  flash.set_level(FlashLevel::Warning)
      .set_title("Warning")
      .set_message("Check your configuration.")
      .set_dismissible(true);

  assert(flash.level() == FlashLevel::Warning);
  assert(flash.title() == "Warning");
  assert(flash.message() == "Check your configuration.");
  assert(flash.dismissible());

  assert(flash.has_title());
  assert(flash.has_message());
  assert(!flash.empty());
}

static void test_attrs()
{
  FlashMessage flash = FlashMessage::info("Hello");

  flash.set_attr("id", "flash-main");
  flash.set_bool_attr("hidden", true);

  assert(flash.attrs().has("id"));
  assert(flash.attrs().has("hidden"));
  assert(flash.attrs().size() == 2);
}

static void test_bool_attr_can_be_removed()
{
  FlashMessage flash = FlashMessage::info("Hello");

  flash.set_bool_attr("hidden", true);
  assert(flash.attrs().has("hidden"));

  flash.set_bool_attr("hidden", false);
  assert(!flash.attrs().has("hidden"));
}

static void test_render_info()
{
  FlashMessage flash = FlashMessage::info("Welcome back.");

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-info" data-flash-level="info" role="status"><span class="flash-message">Welcome back.</span></div>)");
}

static void test_render_success()
{
  FlashMessage flash = FlashMessage::success("Saved successfully.");

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-success" data-flash-level="success" role="status"><span class="flash-message">Saved successfully.</span></div>)");
}

static void test_render_warning()
{
  FlashMessage flash = FlashMessage::warning("Storage is almost full.");

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-warning" data-flash-level="warning" role="status"><span class="flash-message">Storage is almost full.</span></div>)");
}

static void test_render_error_uses_alert_role()
{
  FlashMessage flash = FlashMessage::error("Something went wrong.");

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-error" data-flash-level="error" role="alert"><span class="flash-message">Something went wrong.</span></div>)");
}

static void test_render_neutral()
{
  FlashMessage flash = FlashMessage::neutral("No changes detected.");

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-neutral" data-flash-level="neutral" role="status"><span class="flash-message">No changes detected.</span></div>)");
}

static void test_render_with_title()
{
  FlashMessage flash = FlashMessage::success("Profile updated.");

  flash.set_title("Saved");

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-success" data-flash-level="success" role="status"><strong class="flash-title">Saved</strong><span class="flash-message">Profile updated.</span></div>)");
}

static void test_render_dismissible()
{
  FlashMessage flash = FlashMessage::info("You can close this.");

  flash.set_dismissible(true);

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-info" data-dismissible data-flash-level="info" role="status"><span class="flash-message">You can close this.</span></div>)");
}

static void test_render_with_custom_attrs()
{
  FlashMessage flash = FlashMessage::info("Hello");

  flash.set_attr("id", "main-flash");
  flash.set_attr("class", "custom-flash");
  flash.set_bool_attr("hidden", true);

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="custom-flash" data-flash-level="info" hidden id="main-flash" role="status"><span class="flash-message">Hello</span></div>)");
}

static void test_render_escapes_title_and_message()
{
  FlashMessage flash = FlashMessage::error(R"(<failed & "bad">)");

  flash.set_title(R"(Error "main")");

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-error" data-flash-level="error" role="alert"><strong class="flash-title">Error &quot;main&quot;</strong><span class="flash-message">&lt;failed &amp; &quot;bad&quot;&gt;</span></div>)");
}

static void test_render_empty_message()
{
  FlashMessage flash;

  const std::string rendered = flash.render();

  assert(
      rendered ==
      R"(<div class="flash flash-info" data-flash-level="info" role="status"></div>)");
}

int main()
{
  test_flash_level_to_string();

  test_empty_flash_message();
  test_construct_flash_message();

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
  test_render_dismissible();
  test_render_with_custom_attrs();
  test_render_escapes_title_and_message();
  test_render_empty_message();

  std::cout << "[OK] ui flash message tests passed\n";
  return 0;
}
