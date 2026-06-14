# Vix UI

Web-first UI and app shell primitives for Vix.cpp applications.

`vix::ui` is a small UI foundation layer built on top of the Vix template engine.
It helps Vix.cpp applications describe views, render HTML responses, manage assets, build simple forms, and prepare future browser, desktop, and mobile app shells.

It is not a replacement for `vix::template_`.
It is a higher-level layer that makes template-based web UI development cleaner.

## Goals

Vix UI starts with a simple direction:

- keep Vix web-first
- reuse the existing Vix template engine
- provide small UI primitives
- avoid a heavy frontend framework
- prepare desktop/mobile shells without forcing native UI complexity early

The first target is server-rendered web UI.

Later, the same web UI foundation can be used by desktop or mobile shells through WebView-based app containers.

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
- `vix::ui::Platform`
- `vix::ui::ShellConfig`
- `vix::ui::AppShell`

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
      "<p>{{ page_title }}</p>"
  );

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
    attrs
);
```

Output:

```html
<div class="card">Hello &lt;Vix&gt;</div>
```

## HTML response

```cpp
#include <vix/ui/html/HtmlResponse.hpp>

vix::ui::HtmlResponse response = vix::ui::HtmlResponse::html("<h1>Hello</h1>", 200);

response.header_content_type(); // text/html; charset=utf-8
response.body();                // <h1>Hello</h1>
response.status_code();         // 200
```

`HtmlResponse` is transport-neutral.
It does not depend on a specific HTTP response type. Higher-level Vix integrations can copy its body, status and content type into a real HTTP response.

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

## App shell

`AppShell` is currently a lightweight shell descriptor.

It stores configuration, validates the shell settings, tracks running/stopped state, and exposes the effective target URL.

```cpp
#include <vix/ui/shell/AppShell.hpp>
#include <vix/ui/shell/ShellConfig.hpp>

vix::ui::ShellConfig config;

config.set_name("Vix Admin")
      .set_title("Vix Admin")
      .set_host("127.0.0.1")
      .set_port(8080)
      .set_width(1280)
      .set_height(720);

vix::ui::AppShell shell(config);

auto result = shell.start();

if (result.is_ok())
{
  // shell.running() == true
}
```

Native desktop/mobile WebView startup can be added later on top of this public surface.

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

  shell/
    AppShell.hpp
    ShellConfig.hpp

  platform/
    Platform.hpp

  support/
    Error.hpp
    Result.hpp
```

## Build

From the repository root:

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
- platform/app shell primitives

This keeps the architecture simple and avoids building a heavy UI framework too early.

## License

MIT License.

Copyright 2025, Gaspard Kirira.
