/**
 *
 *  @file html_benchmark.cpp
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

#include <string>

#include <vix/ui/html/Html.hpp>
#include <vix/ui/html/HtmlAttrs.hpp>
#include <vix/ui/html/HtmlEscape.hpp>

namespace
{
  std::string sample_text()
  {
    return R"(<div class="card">Gaspard & Vix UI 'live'</div>)";
  }

  vix::ui::HtmlAttrs sample_attrs()
  {
    vix::ui::HtmlAttrs attrs;

    attrs.set("class", "card card-live");
    attrs.set("id", "dashboard-card");
    attrs.set("data-fragment", "stats");
    attrs.set("data-target", "#stats");
    attrs.set("aria-label", "Dashboard stats");
    attrs.boolean("hidden", false);
    attrs.boolean("data-ready", true);

    return attrs;
  }
}

static void BM_HtmlEscapeText(benchmark::State &state)
{
  const std::string input = sample_text();

  for (auto _ : state)
  {
    std::string output = vix::ui::HtmlEscape::text(input);
    benchmark::DoNotOptimize(output);
  }
}

static void BM_HtmlEscapeAttr(benchmark::State &state)
{
  const std::string input = R"(Vix UI "dashboard" & live updates)";

  for (auto _ : state)
  {
    std::string output = vix::ui::HtmlEscape::attr(input);
    benchmark::DoNotOptimize(output);
  }
}

static void BM_HtmlAttrsRender(benchmark::State &state)
{
  const vix::ui::HtmlAttrs attrs = sample_attrs();

  for (auto _ : state)
  {
    std::string output = attrs.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_HtmlAttrsRenderWithLeadingSpace(benchmark::State &state)
{
  const vix::ui::HtmlAttrs attrs = sample_attrs();

  for (auto _ : state)
  {
    std::string output = attrs.render_with_leading_space();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_HtmlOpenTag(benchmark::State &state)
{
  const vix::ui::HtmlAttrs attrs = sample_attrs();

  for (auto _ : state)
  {
    std::string output = vix::ui::Html::open_tag("section", attrs);
    benchmark::DoNotOptimize(output);
  }
}

static void BM_HtmlTagSmall(benchmark::State &state)
{
  const vix::ui::HtmlAttrs attrs = sample_attrs();
  const std::string content = "<strong>128</strong><span>active users</span>";

  for (auto _ : state)
  {
    std::string output = vix::ui::Html::tag("div", content, attrs);
    benchmark::DoNotOptimize(output);
  }
}

static void BM_HtmlTagEscapedText(benchmark::State &state)
{
  const vix::ui::HtmlAttrs attrs = sample_attrs();
  const std::string content = sample_text();

  for (auto _ : state)
  {
    std::string output =
        vix::ui::Html::tag("p", vix::ui::Html::text(content), attrs);

    benchmark::DoNotOptimize(output);
  }
}

static void BM_HtmlVoidTag(benchmark::State &state)
{
  vix::ui::HtmlAttrs attrs;

  attrs.set("rel", "stylesheet");
  attrs.set("href", "/assets/app.css");
  attrs.set("media", "all");

  for (auto _ : state)
  {
    std::string output = vix::ui::Html::void_tag("link", attrs);
    benchmark::DoNotOptimize(output);
  }
}

BENCHMARK(BM_HtmlEscapeText);
BENCHMARK(BM_HtmlEscapeAttr);
BENCHMARK(BM_HtmlAttrsRender);
BENCHMARK(BM_HtmlAttrsRenderWithLeadingSpace);
BENCHMARK(BM_HtmlOpenTag);
BENCHMARK(BM_HtmlTagSmall);
BENCHMARK(BM_HtmlTagEscapedText);
BENCHMARK(BM_HtmlVoidTag);
