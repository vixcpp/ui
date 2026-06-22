# Vix UI

Web-first UI and app shell primitives for Vix.cpp applications.

`vix::ui` is a small UI foundation layer built on top of the Vix template engine.
It helps Vix.cpp applications describe views, render HTML responses, manage assets, build practical server-rendered forms, create live UI fragments, prepare PWA/mobile metadata, and run server-rendered interfaces inside app shells.

It is not a replacement for `vix::template_`.
It is a higher-level layer that makes template-based web UI development cleaner.

## Version

Current module version:

```txt
0.8.0
```

## Goals

Vix UI follows a simple direction:

- keep Vix web-first
- reuse the existing Vix template engine
- provide small UI primitives
- support server-rendered UI first
- avoid a heavy frontend framework
- prepare desktop/mobile shells through WebView containers
- make dashboards, admin panels, forms, internal tools, and mobile-ready web apps easier to build

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
- `vix::ui::AssetMap`
- `vix::ui::AssetMode`
- `vix::ui::Field`
- `vix::ui::FieldOption`
- `vix::ui::Form`
- `vix::ui::FormData`
- `vix::ui::CsrfToken`
- `vix::ui::ValidationError`
- `vix::ui::Fragment`
- `vix::ui::LiveUpdate`
- `vix::ui::FlashMessage`
- `vix::ui::Toast`
- `vix::ui::Viewport`
- `vix::ui::SafeArea`
- `vix::ui::WebAppManifest`
- `vix::ui::PwaMeta`
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

## Asset pipeline helpers

Vix UI provides small asset pipeline helpers for development and production modes.

They are useful when an application needs stable logical names in C++ while production builds use hashed files.

Typical use cases:

- asset versioning
- manifest lookup
- hashed asset paths
- CSS and JavaScript grouping
- preload helpers
- module script support
- production/development asset modes

Example direction:

```cpp
#include <vix/ui/assets/AssetManager.hpp>
#include <vix/ui/assets/AssetMap.hpp>
#include <vix/ui/assets/AssetMode.hpp>

vix::ui::AssetManager assets("/assets");

assets.add_stylesheet("app_css", "app.css");
assets.add_script("app_js", "app.js", vix::ui::AssetLoading::Deferred);

std::string tags = assets.render();
```

The asset layer only describes and renders asset references.
It does not bundle JavaScript, compile CSS, or serve files directly.

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

## Rich forms

Vix UI forms support richer server-rendered fields:

- text
- email
- password
- number
- hidden
- checkbox
- radio
- textarea
- select
- file

```cpp
#include <vix/ui/forms/Form.hpp>

vix::ui::Form form = vix::ui::Form::post("/products/create");

form.set_csrf("csrf-demo-token");

form.add_field(
    vix::ui::Field::text("title")
        .set_label("Product title")
        .set_required(true));

form.add_field(
    vix::ui::Field::number("price")
        .set_label("Price")
        .set_attr("min", "0")
        .set_attr("step", "0.01"));

vix::ui::Field condition = vix::ui::Field::select("condition")
                               .set_label("Condition")
                               .set_required(true);

condition.add_option("", "Choose condition");
condition.add_option("new", "New");
condition.add_option("used", "Used");
condition.add_option("refurbished", "Refurbished");

form.add_field(condition);

form.add_field(
    vix::ui::Field::file("images")
        .set_label("Product images")
        .set_accept("image/png,image/jpeg,image/webp")
        .set_multiple(true));

std::string html = form.render();
```

When a file field is present, the form automatically renders:

```html
enctype="multipart/form-data"
```

## Form data binding

`FormData` stores old input values or submitted values and can bind them back to a form.

This is useful after validation fails, so users do not lose their input.

```cpp
#include <vix/ui/forms/Form.hpp>
#include <vix/ui/forms/FormData.hpp>

vix::ui::FormData old_input;

old_input.set("name", "Gaspard");
old_input.set("email", "gaspard@example.com");
old_input.set("country", "ug");
old_input.set("newsletter", "1");

vix::ui::Form form = vix::ui::Form::post("/profile/update");

form.add_field(
    vix::ui::Field::text("name")
        .set_label("Name"));

form.add_field(
    vix::ui::Field::email("email")
        .set_label("Email"));

vix::ui::Field country = vix::ui::Field::select("country")
                             .set_label("Country");

country.add_option("ug", "Uganda");
country.add_option("cd", "DRC");
country.add_option("rw", "Rwanda");

form.add_field(country);

form.add_field(
    vix::ui::Field::checkbox("newsletter")
        .set_label("Receive product updates")
        .set_value("1"));

form.bind(old_input);
```

## CSRF helper

`CsrfToken` is a rendering helper only.

It does not generate, sign, rotate, or validate tokens. Applications should do that in their own security layer.

```cpp
#include <vix/ui/forms/CsrfToken.hpp>
#include <vix/ui/forms/Form.hpp>

vix::ui::Form form = vix::ui::Form::post("/account/update");

form.set_csrf(
    vix::ui::CsrfToken::named("_csrf", "secure-token-value"));

std::string csrf_input = form.render_csrf();
```

Rendered output:

```html
<input id="_csrf" name="_csrf" type="hidden" value="secure-token-value" />
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

## PWA viewport helper

`Viewport` renders mobile-friendly viewport metadata.

```cpp
#include <vix/ui/pwa/Viewport.hpp>

vix::ui::Viewport viewport =
    vix::ui::Viewport::mobile_app();

std::string html = viewport.render();
```

Example output:

```html
<meta
  content="width=device-width, initial-scale=1, viewport-fit=cover"
  name="viewport"
/>
```

## Safe-area CSS helper

`SafeArea` renders CSS variables and classes for mobile safe-area insets.

```cpp
#include <vix/ui/pwa/SafeArea.hpp>

vix::ui::SafeArea safe_area =
    vix::ui::SafeArea::vertical();

std::string css = safe_area.render();
```

Example output:

```css
:root {
  --vix-safe-area-top: env(safe-area-inset-top);
  --vix-safe-area-bottom: env(safe-area-inset-bottom);
}
.vix-safe-area {
  padding-top: var(--vix-safe-area-top);
  padding-bottom: var(--vix-safe-area-bottom);
}
```

## Web app manifest

`WebAppManifest` builds a deterministic manifest JSON document for installable web apps.

```cpp
#include <vix/ui/pwa/WebAppManifest.hpp>

vix::ui::WebAppManifest manifest =
    vix::ui::WebAppManifest::app("Vix Mobile", "Vix");

manifest.set_description("A mobile-ready Vix UI application")
    .set_start_url("/")
    .set_scope("/")
    .set_id("com.vix.mobile")
    .set_lang("en")
    .set_display(vix::ui::WebAppDisplayMode::Standalone)
    .set_orientation(vix::ui::WebAppOrientation::Portrait)
    .set_background_color("#ffffff")
    .set_theme_color("#111111");

manifest.add_icon(
    "/icons/icon-192.png",
    "192x192",
    "image/png");

manifest.add_icon(
    "/icons/icon-512.png",
    "512x512",
    "image/png",
    "any maskable");

std::string json = manifest.render();
```

Example output:

```json
{
  "name": "Vix Mobile",
  "short_name": "Vix",
  "description": "A mobile-ready Vix UI application",
  "start_url": "/",
  "scope": "/",
  "id": "com.vix.mobile",
  "lang": "en",
  "display": "standalone",
  "orientation": "portrait",
  "background_color": "#ffffff",
  "theme_color": "#111111",
  "icons": [
    {
      "src": "/icons/icon-192.png",
      "sizes": "192x192",
      "type": "image/png"
    },
    {
      "src": "/icons/icon-512.png",
      "sizes": "512x512",
      "type": "image/png",
      "purpose": "any maskable"
    }
  ]
}
```

## PWA meta tags

`PwaMeta` renders the HTML metadata commonly needed by mobile-friendly pages and installable web apps.

```cpp
#include <vix/ui/pwa/PwaMeta.hpp>

vix::ui::PwaMeta meta =
    vix::ui::PwaMeta::mobile_app("Vix Mobile", "#111111");

meta.set_color_scheme("light dark")
    .set_format_detection("telephone=no");

std::string html = meta.render();
```

Example output:

```html
<meta
  content="width=device-width, initial-scale=1, viewport-fit=cover"
  name="viewport"
/>
<link href="/manifest.webmanifest" rel="manifest" />
<meta content="#111111" name="theme-color" />
<meta content="Vix Mobile" name="application-name" />
<meta content="yes" name="apple-mobile-web-app-capable" />
<meta content="Vix Mobile" name="apple-mobile-web-app-title" />
<meta
  content="black-translucent"
  name="apple-mobile-web-app-status-bar-style"
/>
<meta content="light dark" name="color-scheme" />
<meta content="telephone=no" name="format-detection" />
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
      .set_app_version("0.8.0")
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
    AssetMap.hpp
    AssetMode.hpp

  forms/
    CsrfToken.hpp
    Field.hpp
    FieldOption.hpp
    Form.hpp
    FormData.hpp
    ValidationError.hpp

  live/
    Fragment.hpp
    LiveUpdate.hpp
    FlashMessage.hpp
    Toast.hpp

  pwa/
    Viewport.hpp
    SafeArea.hpp
    WebAppManifest.hpp
    PwaMeta.hpp

  shell/
    AppShell.hpp
    ShellConfig.hpp
    ServerProcess.hpp
    ServerReadiness.hpp
    backends/
      DescriptorShellBackend.hpp
      LinuxWebViewShellBackend.hpp
      ShellBackend.hpp

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
vix run examples/05_shell_config_v04.cpp
vix run examples/06_fragment.cpp
vix run examples/07_live_update.cpp
vix run examples/08_flash_and_toast.cpp
vix run examples/09_rich_forms.cpp
vix run examples/10_form_binding.cpp
vix run examples/11_asset_pipeline.cpp
vix run examples/12_asset_manifest.cpp
vix run examples/13_pwa_helpers.cpp
vix run examples/14_web_app_manifest.cpp
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
- asset descriptors
- asset manifests
- asset pipeline helpers
- form helpers
- form data binding
- old input values
- CSRF rendering helpers
- live UI fragments
- WebSocket-friendly update payloads
- flash and toast rendering helpers
- mobile viewport helpers
- safe-area CSS helpers
- web app manifest metadata
- PWA meta tag rendering
- platform/app shell primitives
- server process helpers
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
