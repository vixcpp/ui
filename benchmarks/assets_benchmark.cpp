/**
 *
 *  @file assets_benchmark.cpp
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

#include <vix/ui/assets/Asset.hpp>
#include <vix/ui/assets/AssetManager.hpp>
#include <vix/ui/assets/AssetManifest.hpp>

namespace
{
  vix::ui::Asset stylesheet_asset()
  {
    return vix::ui::Asset::stylesheet("/assets/app.css")
        .set_integrity("sha384-demo")
        .set_crossorigin("anonymous")
        .set_attr("data-bundle", "main");
  }

  vix::ui::Asset script_asset()
  {
    return vix::ui::Asset::script(
               "/assets/app.js",
               vix::ui::AssetLoading::Deferred)
        .set_integrity("sha384-demo")
        .set_crossorigin("anonymous")
        .set_attr("data-entry", "app");
  }

  vix::ui::Asset image_asset()
  {
    return vix::ui::Asset::image(
               "/assets/logo.png",
               "Vix UI logo")
        .set_attr("class", "logo")
        .set_attr("width", "128")
        .set_attr("height", "128");
  }

  vix::ui::AssetManifest sample_manifest()
  {
    vix::ui::AssetManifest manifest;

    manifest.add(stylesheet_asset());
    manifest.add(script_asset());
    manifest.add(image_asset());
    manifest.add_font("/assets/fonts/inter.woff2");

    return manifest;
  }

  vix::ui::AssetManager sample_manager()
  {
    vix::ui::AssetManager manager("/assets");

    manager.add_stylesheet("app_css", "app.css");
    manager.add_script(
        "app_js",
        "app.js",
        vix::ui::AssetLoading::Deferred);
    manager.add_image("logo", "logo.png", "Vix UI logo");
    manager.add_font("inter", "fonts/inter.woff2");

    if (vix::ui::Asset *css = manager.get("app_css"))
    {
      css->set_integrity("sha384-demo")
          .set_crossorigin("anonymous");
    }

    if (vix::ui::Asset *js = manager.get("app_js"))
    {
      js->set_integrity("sha384-demo")
          .set_crossorigin("anonymous");
    }

    return manager;
  }
}

static void BM_AssetStylesheetToHtml(benchmark::State &state)
{
  const vix::ui::Asset asset = stylesheet_asset();

  for (auto _ : state)
  {
    std::string output = asset.to_html();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetScriptToHtml(benchmark::State &state)
{
  const vix::ui::Asset asset = script_asset();

  for (auto _ : state)
  {
    std::string output = asset.to_html();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetImageToHtml(benchmark::State &state)
{
  const vix::ui::Asset asset = image_asset();

  for (auto _ : state)
  {
    std::string output = asset.to_html();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetFontToHtml(benchmark::State &state)
{
  const vix::ui::Asset asset =
      vix::ui::Asset::font("/assets/fonts/inter.woff2");

  for (auto _ : state)
  {
    std::string output = asset.to_html();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetManifestRender(benchmark::State &state)
{
  const vix::ui::AssetManifest manifest = sample_manifest();

  for (auto _ : state)
  {
    std::string output = manifest.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetManifestRenderStylesheets(benchmark::State &state)
{
  const vix::ui::AssetManifest manifest = sample_manifest();

  for (auto _ : state)
  {
    std::string output = manifest.render_stylesheets();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetManifestRenderScripts(benchmark::State &state)
{
  const vix::ui::AssetManifest manifest = sample_manifest();

  for (auto _ : state)
  {
    std::string output = manifest.render_scripts();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetManagerResolve(benchmark::State &state)
{
  const vix::ui::AssetManager manager("/assets");

  for (auto _ : state)
  {
    std::string output = manager.resolve("css/app.css");
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetManagerManifest(benchmark::State &state)
{
  const vix::ui::AssetManager manager = sample_manager();

  for (auto _ : state)
  {
    vix::ui::AssetManifest manifest = manager.manifest();
    benchmark::DoNotOptimize(manifest.size());
  }
}

static void BM_AssetManagerRender(benchmark::State &state)
{
  const vix::ui::AssetManager manager = sample_manager();

  for (auto _ : state)
  {
    std::string output = manager.render();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetManagerRenderStylesheets(benchmark::State &state)
{
  const vix::ui::AssetManager manager = sample_manager();

  for (auto _ : state)
  {
    std::string output = manager.render_stylesheets();
    benchmark::DoNotOptimize(output);
  }
}

static void BM_AssetManagerRenderScripts(benchmark::State &state)
{
  const vix::ui::AssetManager manager = sample_manager();

  for (auto _ : state)
  {
    std::string output = manager.render_scripts();
    benchmark::DoNotOptimize(output);
  }
}

BENCHMARK(BM_AssetStylesheetToHtml);
BENCHMARK(BM_AssetScriptToHtml);
BENCHMARK(BM_AssetImageToHtml);
BENCHMARK(BM_AssetFontToHtml);

BENCHMARK(BM_AssetManifestRender);
BENCHMARK(BM_AssetManifestRenderStylesheets);
BENCHMARK(BM_AssetManifestRenderScripts);

BENCHMARK(BM_AssetManagerResolve);
BENCHMARK(BM_AssetManagerManifest);
BENCHMARK(BM_AssetManagerRender);
BENCHMARK(BM_AssetManagerRenderStylesheets);
BENCHMARK(BM_AssetManagerRenderScripts);
