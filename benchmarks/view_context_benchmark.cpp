/**
 *
 *  @file view_context_benchmark.cpp
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

#include <vix/template/Context.hpp>
#include <vix/template/Value.hpp>
#include <vix/ui/core/View.hpp>
#include <vix/ui/core/ViewContext.hpp>

namespace
{
  vix::template_::Object sample_values()
  {
    vix::template_::Object values;

    values["name"] = "Gaspard";
    values["role"] = "builder";
    values["status"] = "online";
    values["users"] = 128;
    values["active"] = true;
    values["version"] = "0.5.0";

    return values;
  }

  vix::ui::ViewContext sample_context()
  {
    vix::ui::ViewContext context;

    context.set("name", "Gaspard");
    context.set("role", "builder");
    context.set("status", "online");
    context.set("users", 128);
    context.set("active", true);
    context.set("version", "0.5.0");

    return context;
  }

  vix::ui::View sample_view()
  {
    vix::ui::View view("dashboard.html");

    view.set_title("Dashboard");
    view.set("name", "Gaspard");
    view.set("role", "builder");
    view.set("status", "online");
    view.set("users", 128);
    view.set("active", true);

    return view;
  }
}

static void BM_ViewContextSetOneValue(benchmark::State &state)
{
  for (auto _ : state)
  {
    vix::ui::ViewContext context;

    context.set("name", "Gaspard");

    benchmark::DoNotOptimize(context.size());
  }
}

static void BM_ViewContextSetMultipleValues(benchmark::State &state)
{
  for (auto _ : state)
  {
    vix::ui::ViewContext context;

    context.set("name", "Gaspard");
    context.set("role", "builder");
    context.set("status", "online");
    context.set("users", 128);
    context.set("active", true);
    context.set("version", "0.5.0");

    benchmark::DoNotOptimize(context.size());
  }
}

static void BM_ViewContextConstructFromObject(benchmark::State &state)
{
  const vix::template_::Object values = sample_values();

  for (auto _ : state)
  {
    vix::ui::ViewContext context(values);

    benchmark::DoNotOptimize(context.size());
  }
}

static void BM_ViewContextGetValue(benchmark::State &state)
{
  const vix::ui::ViewContext context = sample_context();

  for (auto _ : state)
  {
    const vix::template_::Value *value = context.get("name");

    benchmark::DoNotOptimize(value);
  }
}

static void BM_ViewContextGetOrValue(benchmark::State &state)
{
  const vix::ui::ViewContext context = sample_context();

  for (auto _ : state)
  {
    vix::template_::Value value = context.get_or("missing", "fallback");

    benchmark::DoNotOptimize(value);
  }
}

static void BM_ViewContextToTemplateContext(benchmark::State &state)
{
  const vix::ui::ViewContext context = sample_context();

  for (auto _ : state)
  {
    vix::template_::Context template_context =
        context.to_template_context();

    benchmark::DoNotOptimize(template_context.get("name"));
  }
}

static void BM_ViewSetOneValue(benchmark::State &state)
{
  for (auto _ : state)
  {
    vix::ui::View view("dashboard.html");

    view.set("name", "Gaspard");

    benchmark::DoNotOptimize(view.values().size());
  }
}

static void BM_ViewSetMultipleValues(benchmark::State &state)
{
  for (auto _ : state)
  {
    vix::ui::View view("dashboard.html");

    view.set_title("Dashboard");
    view.set("name", "Gaspard");
    view.set("role", "builder");
    view.set("status", "online");
    view.set("users", 128);
    view.set("active", true);

    benchmark::DoNotOptimize(view.values().size());
  }
}

static void BM_ViewMergeValues(benchmark::State &state)
{
  const vix::template_::Object values = sample_values();

  for (auto _ : state)
  {
    vix::ui::View view("dashboard.html");

    view.merge(values);

    benchmark::DoNotOptimize(view.values().size());
  }
}

static void BM_ViewContextFromView(benchmark::State &state)
{
  const vix::ui::View view = sample_view();

  for (auto _ : state)
  {
    vix::ui::ViewContext context = view.context();

    benchmark::DoNotOptimize(context.size());
  }
}

static void BM_ViewToTemplateContext(benchmark::State &state)
{
  const vix::ui::View view = sample_view();

  for (auto _ : state)
  {
    vix::template_::Context context = view.to_template_context();

    benchmark::DoNotOptimize(context.get("name"));
  }
}

BENCHMARK(BM_ViewContextSetOneValue);
BENCHMARK(BM_ViewContextSetMultipleValues);
BENCHMARK(BM_ViewContextConstructFromObject);
BENCHMARK(BM_ViewContextGetValue);
BENCHMARK(BM_ViewContextGetOrValue);
BENCHMARK(BM_ViewContextToTemplateContext);

BENCHMARK(BM_ViewSetOneValue);
BENCHMARK(BM_ViewSetMultipleValues);
BENCHMARK(BM_ViewMergeValues);
BENCHMARK(BM_ViewContextFromView);
BENCHMARK(BM_ViewToTemplateContext);
