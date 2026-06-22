/**
 *
 *  @file live_benchmark.cpp
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
#include <benchmark/benchmark.h>

#include <chrono>
#include <string>

#include <vix/ui/live/FlashMessage.hpp>
#include <vix/ui/live/Fragment.hpp>
#include <vix/ui/live/LiveUpdate.hpp>
#include <vix/ui/live/Toast.hpp>

namespace
{
  vix::ui::Fragment stats_fragment()
  {
    return vix::ui::Fragment::make("dashboard-stats")
        .set_target("#stats-card")
        .set_attr("class", "stats-fragment")
        .set_html(
            "<div class=\"stat-card\">"
            "<strong>128</strong>"
            "<span>active users</span>"
            "</div>");
  }

  vix::ui::Fragment table_row_fragment()
  {
    return vix::ui::Fragment::make("user-row-42")
        .set_target("#user-42")
        .set_html(
            "<tr id=\"user-42\">"
            "<td>Gaspard</td>"
            "<td>online</td>"
            "<td>admin</td>"
            "</tr>");
  }

  vix::ui::LiveUpdate replace_update()
  {
    return vix::ui::LiveUpdate::replace(
               "#stats-card",
               stats_fragment())
        .set_event("dashboard.stats.updated")
        .set_id("update-42");
  }

  vix::ui::LiveUpdate append_update()
  {
    return vix::ui::LiveUpdate::append(
               "#users-table",
               table_row_fragment())
        .set_event("users.appended")
        .set_id("user-42");
  }

  vix::ui::FlashMessage success_flash()
  {
    return vix::ui::FlashMessage::success("Profile updated successfully.")
        .set_title("Saved")
        .set_dismissible(true)
        .set_attr("id", "flash-profile");
  }

  vix::ui::FlashMessage error_flash()
  {
    return vix::ui::FlashMessage::error("Failed to save profile.")
        .set_title("Error")
        .set_dismissible(true)
        .set_attr("id", "flash-error");
  }

  vix::ui::Toast info_toast()
  {
    return vix::ui::Toast::info("Background sync completed.")
        .set_title("Sync")
        .set_position(vix::ui::ToastPosition::BottomRight)
        .set_timeout(std::chrono::milliseconds(3000));
  }

  vix::ui::Toast error_toast()
  {
    return vix::ui::Toast::error("Connection lost.")
        .set_title("Network")
        .set_position(vix::ui::ToastPosition::TopRight)
        .set_auto_hide(false);
  }
}

static void BM_FragmentRender(benchmark::State &state)
{
  const vix::ui::Fragment fragment = stats_fragment();

  for (auto _ : state)
  {
    std::string output = fragment.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FragmentRenderWrapped(benchmark::State &state)
{
  const vix::ui::Fragment fragment = stats_fragment();

  for (auto _ : state)
  {
    std::string output = fragment.render_wrapped();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FragmentRenderWrappedCustomTag(benchmark::State &state)
{
  const vix::ui::Fragment fragment = table_row_fragment();

  for (auto _ : state)
  {
    std::string output = fragment.render_wrapped("tbody");
    benchmark::DoNotOptimize(output);
  }
}

static void BM_LiveUpdateRenderReplace(benchmark::State &state)
{
  const vix::ui::LiveUpdate update = replace_update();

  for (auto _ : state)
  {
    std::string output = update.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_LiveUpdateToJsonReplace(benchmark::State &state)
{
  const vix::ui::LiveUpdate update = replace_update();

  for (auto _ : state)
  {
    std::string output = update.to_json();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_LiveUpdateToJsonAppend(benchmark::State &state)
{
  const vix::ui::LiveUpdate update = append_update();

  for (auto _ : state)
  {
    std::string output = update.to_json();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_LiveUpdateToJsonRemove(benchmark::State &state)
{
  const vix::ui::LiveUpdate update =
      vix::ui::LiveUpdate::remove("#old-toast")
          .set_event("toast.remove")
          .set_id("toast-old");

  for (auto _ : state)
  {
    std::string output = update.to_json();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FlashMessageRenderSuccess(benchmark::State &state)
{
  const vix::ui::FlashMessage flash = success_flash();

  for (auto _ : state)
  {
    std::string output = flash.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_FlashMessageRenderError(benchmark::State &state)
{
  const vix::ui::FlashMessage flash = error_flash();

  for (auto _ : state)
  {
    std::string output = flash.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_ToastRenderInfo(benchmark::State &state)
{
  const vix::ui::Toast toast = info_toast();

  for (auto _ : state)
  {
    std::string output = toast.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_ToastRenderError(benchmark::State &state)
{
  const vix::ui::Toast toast = error_toast();

  for (auto _ : state)
  {
    std::string output = toast.render();
    benchmark::DoNotOptimize(output);
  }
}

BENCHMARK(BM_FragmentRender);
BENCHMARK(BM_FragmentRenderWrapped);
BENCHMARK(BM_FragmentRenderWrappedCustomTag);

BENCHMARK(BM_LiveUpdateRenderReplace);
BENCHMARK(BM_LiveUpdateToJsonReplace);
BENCHMARK(BM_LiveUpdateToJsonAppend);
BENCHMARK(BM_LiveUpdateToJsonRemove);

BENCHMARK(BM_FlashMessageRenderSuccess);
BENCHMARK(BM_FlashMessageRenderError);

BENCHMARK(BM_ToastRenderInfo);
BENCHMARK(BM_ToastRenderError);
