# Vix UI

Web-first UI and app shell primitives for Vix.cpp applications.

`vix::ui` is a small UI foundation layer built on top of the Vix template engine.
It helps Vix.cpp applications describe views, render HTML responses, manage assets, build forms, create live UI fragments, and run server-rendered interfaces inside app shells.

It is not a replacement for `vix::template_`.
It is a higher-level layer that makes template-based web UI development cleaner.

## Version

Current module version:

```txt
0.5.0
```

## Goals

Vix UI follows a simple direction:

- keep Vix web-first
- reuse the existing Vix template engine
- provide small UI primitives
- support server-rendered UI first
- avoid a heavy frontend framework
- prepare desktop/mobile shells through WebView containers
- make dashboards, admin panels and internal tools easier to build

The first target is server-rendered web UI.

The same UI foundation can then be used by desktop or mobile shells through WebView-based app containers.

## What it provides

Current primitives:

- `vix::ui::View`
- `vix::ui::ViewContext`
- `vix::ui::ViewResult`
- `vix::ui::Html`
- `vix::ui::HtmlAttrs`
- `vix::ui::HtmlEscape`
- `vix::ui::HtmlResponse`
- `vix::ui::Asset`
- `vix::ui::AssetManifest`
- `vix::ui::AssetManager`
- `vix::ui::Field`
- `vix::ui::Form`
- `vix::ui::ValidationError`
- `vix::ui::Fragment`
- `vix::ui::LiveUpdate`
- `vix::ui::FlashMessage`
- `vix::ui::Toast`
- `vix::ui::Platform`
- `vix::ui::ShellConfig`
- `vix::ui::AppShell`
- `vix::ui::ServerProcess`
- `vix::ui::ServerReadiness`

## Basic view example

```cpp
#include <iostream>
#include <memory>

#include <vix/template/Engine.hpp>
#include <vix/template/StringLoader.hpp>
#include <vix/ui/core/View.hpp>

int main()
{
  auto loader = std::make_shared<vix::template_::StringLoader>();

  loader->set(
      "home.html",
      "<h1>Hello {{ name }}</h1>"
      "<p>{{ page_title }}</p>");

  vix::template_::Engine engine(loader);

  vix::ui::View view("home.html");
  view.set_title("Vix UI");
  view.set("name", "Gaspard");

  const vix::ui::ViewResult result = view.render(engine);

  if (!result.success)
  {
    return 1;
  }

  std::cout << result.output << "\n";
  return 0;
}
```

## HTML helpers

```cpp
#include <vix/ui/html/Html.hpp>
#include <vix/ui/html/HtmlAttrs.hpp>

vix::ui::HtmlAttrs attrs;
attrs.set("class", "card");

std::string html = vix::ui::Html::tag(
    "div",
    vix::ui::Html::text("Hello <Vix>"),
    attrs);
```

Output:

```html
<div class="card">Hello &lt;Vix&gt;</div>
```

## HTML response

```cpp
#include <vix/ui/html/HtmlResponse.hpp>

vix::ui::HtmlResponse response =
    vix::ui::HtmlResponse::html("<h1>Hello</h1>", 200);

response.header_content_type(); // text/html; charset=utf-8
response.body();                // <h1>Hello</h1>
response.status_code();         // 200
```

`HtmlResponse` is transport-neutral.

It does not depend on a specific HTTP response type. Higher-level Vix integrations can copy its body, status, and content type into a real HTTP response.

## Assets

```cpp
#include <vix/ui/assets/AssetManager.hpp>

vix::ui::AssetManager assets("/assets");

assets.add_stylesheet("app_css", "app.css");
assets.add_script("app_js", "app.js", vix::ui::AssetLoading::Deferred);
assets.add_font("inter", "fonts/inter.woff2");

std::string html = assets.render();
```

Example output:

```html
<link href="/assets/app.css" media="all" rel="stylesheet" />
<script defer src="/assets/app.js"></script>
<link
  as="font"
  crossorigin="anonymous"
  href="/assets/fonts/inter.woff2"
  rel="preload"
/>
```

## Forms

```cpp
#include <vix/ui/forms/Form.hpp>

vix::ui::Form form = vix::ui::Form::post("/login");

form.add_email("email");
form.add_password("password");
form.add_hidden("_token", "abc123");

std::string html = form.render();
```

Validation errors can be attached to the form and to individual fields:

```cpp
form.add_error("email", "Email is required.");
```

## Live UI fragments

`Fragment` represents a small server-rendered HTML piece.

It can be returned from a route, embedded in a template, or sent through a WebSocket-friendly update payload.

```cpp
#include <vix/ui/live/Fragment.hpp>

vix::ui::Fragment stats =
    vix::ui::Fragment::make("dashboard-stats")
        .set_target("#stats-card")
        .set_html("<strong>128</strong><span>active users</span>");

std::string html = stats.render();
std::string wrapped = stats.render_wrapped();
```

## Live updates

`LiveUpdate` describes how a client should apply a server-rendered fragment.

Supported actions:

- `replace`
- `append`
- `prepend`
- `before`
- `after`
- `remove`
- `none`

```cpp
#include <vix/ui/live/Fragment.hpp>
#include <vix/ui/live/LiveUpdate.hpp>

vix::ui::Fragment row =
    vix::ui::Fragment::make("user-row-42")
        .set_html("<tr id=\"user-42\"><td>Gaspard</td><td>online</td></tr>");

vix::ui::LiveUpdate update =
    vix::ui::LiveUpdate::replace("#user-42", row)
        .set_event("users.updated")
        .set_id("update-42");

std::string payload = update.to_json();
```

Example payload:

```json
{
  "type": "ui.update",
  "action": "replace",
  "target": "#user-42",
  "event": "users.updated",
  "id": "update-42",
  "fragment": "user-row-42",
  "html": "<tr id=\"user-42\"><td>Gaspard</td><td>online</td></tr>"
}
```

## Flash messages

```cpp
#include <vix/ui/live/FlashMessage.hpp>

vix::ui::FlashMessage flash =
    vix::ui::FlashMessage::success("Profile updated successfully.")
        .set_title("Saved")
        .set_dismissible(true);

std::string html = flash.render();
```

## Toasts

```cpp
#include <chrono>
#include <vix/ui/live/Toast.hpp>

vix::ui::Toast toast =
    vix::ui::Toast::info("Background sync completed.")
        .set_title("Sync")
        .set_position(vix::ui::ToastPosition::BottomRight)
        .set_timeout(std::chrono::milliseconds(3000));

std::string html = toast.render();
```

## App shell

`AppShell` is the public shell facade for Vix UI applications.

It stores configuration, validates shell settings, can start a local server process, wait for server readiness, and open the configured target URL through the selected backend.

```cpp
#include <chrono>

#include <vix/ui/shell/AppShell.hpp>
#include <vix/ui/shell/ShellConfig.hpp>

vix::ui::ShellConfig config;

config.set_name("Vix Admin")
      .set_title("Vix Admin")
      .set_app_id("com.vix.admin")
      .set_app_version("0.5.0")
      .set_vendor("Vix.cpp")
      .set_host("127.0.0.1")
      .set_port(8080)
      .set_readiness_url("http://127.0.0.1:8080/health")
      .set_startup_timeout(std::chrono::milliseconds(5000))
      .set_wait_for_server(true)
      .set_width(1280)
      .set_height(720);

vix::ui::AppShell shell(config);

auto result = shell.start();

if (result.is_ok())
{
  // shell started
}
```

When Linux WebView support is enabled, the Linux backend can open the target URL inside a desktop WebView shell.

## Module structure

```txt
include/vix/ui/
  Version.hpp
  all.hpp

  core/
    View.hpp
    ViewData.hpp
    ViewContext.hpp
    ViewResult.hpp

  html/
    Html.hpp
    HtmlResponse.hpp
    HtmlAttrs.hpp
    HtmlEscape.hpp

  assets/
    Asset.hpp
    AssetManager.hpp
    AssetManifest.hpp

  forms/
    Form.hpp
    Field.hpp
    ValidationError.hpp

  live/
    Fragment.hpp
    LiveUpdate.hpp
    FlashMessage.hpp
    Toast.hpp

  shell/
    AppShell.hpp
    ShellConfig.hpp
    ServerProcess.hpp
    ServerReadiness.hpp

  platform/
    Platform.hpp

  support/
    Error.hpp
    Result.hpp
```

## Build

From the module directory or repository workflow:

```bash
vix build
```

Run tests:

```bash
vix tests
```

Run examples:

```bash
vix run examples/01_basic_view.cpp
vix run examples/02_html_response.cpp
vix run examples/03_assets.cpp
vix run examples/04_forms.cpp
vix run examples/06_fragment.cpp
vix run examples/07_live_update.cpp
vix run examples/08_flash_and_toast.cpp
```

Build benchmarks with CMake:

```bash
vix build --build-target all -v -DUI_BUILD_BENCHMARKS=ON
```

Run benchmarks:

```bash
./build-ninja/benchmarks/ui_html_benchmark
./build-ninja/benchmarks/ui_assets_benchmark
./build-ninja/benchmarks/ui_forms_benchmark
./build-ninja/benchmarks/ui_live_benchmark
./build-ninja/benchmarks/ui_view_context_benchmark
```

## Design direction

Vix UI is intentionally small.

The template engine remains responsible for:

- template loading
- parsing
- rendering
- escaping
- caching
- includes
- layouts
- blocks

The UI module is responsible for:

- view metadata
- view contexts
- HTML helpers
- HTML response data
- asset helpers
- form helpers
- live UI fragments
- WebSocket-friendly update payloads
- flash and toast rendering helpers
- platform/app shell primitives
- server readiness helpers

This keeps the architecture simple and avoids building a heavy UI framework too early.

## Non-goals

Vix UI is not trying to be:

- a React clone
- a Flutter clone
- a Qt replacement
- a native widget toolkit
- a complex virtual DOM engine
- a heavy frontend framework

The direction is:

- server-rendered UI first
- WebView app shell later
- native UI only if truly needed

## License

MIT License.

Copyright 2025, Gaspard Kirira.
