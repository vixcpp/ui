# Vix UI Roadmap

This roadmap describes the planned evolution of `vix::ui`.

Vix UI is a web-first UI foundation for Vix.cpp applications.
It starts with server-rendered views, HTML helpers, asset helpers, forms, live UI fragments, PWA/mobile helpers, and app shell primitives.

Desktop and mobile support are added progressively through WebView-based app shells.

## v0.1.0 — UI foundation

Status: done.

Focus:

- core view abstraction
- template-compatible view context
- rendered view result
- HTML escaping helpers
- HTML attribute helpers
- small HTML generation helpers
- transport-neutral HTML response
- asset descriptors
- asset manifests
- asset manager
- basic form and field helpers
- validation error model
- platform descriptor
- shell configuration
- lightweight app shell state
- examples
- unit tests

Goal:

Create the first stable UI module foundation without introducing native desktop or mobile complexity too early.

## v0.2.0 — Vix web integration

Status: done.

Focus:

- integration with the Vix HTTP response wrapper
- `ResponseWrapper::ui(const vix::ui::HtmlResponse &)`
- `ResponseWrapper::ui(const vix::ui::View &)`
- rendering `vix::ui::View` through the configured template engine
- returning `vix::ui::HtmlResponse` through normal HTTP responses
- automatic `Content-Type: text/html; charset=utf-8`
- `X-Content-Type-Options: nosniff` for UI HTML responses
- support for normal HTTP status handling
- support for `204 No Content` and `304 Not Modified`
- improved examples using real Vix response flow

Example:

```cpp
app.get("/", [](Request &, ResponseWrapper &res)
{
  auto view = vix::ui::View("home.html")
      .set_title("Home")
      .set("name", "Gaspard");

  res.ui(view);
});
```

Goal:

Make Vix UI work directly inside normal Vix web applications, not only standalone examples.

## v0.3.0 — Desktop shell MVP

Status: done.

Focus:

- Linux-first desktop shell
- local Vix server launch support
- WebView window integration
- load `http://127.0.0.1:<port>`
- shell lifecycle: start, stop, restart
- window title, size, resizable, fullscreen, and devtools options
- basic desktop examples

Goal:

Run a Vix web application as a desktop app without rewriting the UI.

Model:

```txt
Vix app
  -> HTTP server
  -> templates / vix::ui
  -> local URL
  -> desktop WebView window
```

## v0.4.0 — Desktop shell expansion

Status: done.

Focus:

- improved Linux shell support
- backend abstraction
- descriptor shell backend
- Linux WebView shell backend
- shell error reporting
- better platform detection
- local server readiness checks
- configurable startup timeout
- desktop app metadata
- icon path support
- basic packaging direction

Goal:

Move from proof of concept to a more usable desktop app shell.

## v0.5.0 — Live UI helpers

Status: done.

Focus:

- WebSocket-friendly UI helpers
- partial HTML fragments
- small component update conventions
- server-driven UI update patterns
- helpers for dashboards and internal tools
- flash messages
- toast rendering helpers

Goal:

Make Vix UI useful for dynamic dashboards, admin panels, monitoring pages, and internal tools.

## v0.6.0 — Forms and validation improvements

Status: done.

Focus:

- richer field types
- select options
- checkbox state
- radio groups
- textarea helpers
- file input helpers
- form data binding
- old input values
- field-level rendering helpers
- form error summaries
- CSRF helper integration point

Goal:

Make server-rendered forms more practical for real applications.

## v0.7.0 — Asset pipeline helpers

Status: done.

Focus:

- asset versioning
- manifest lookup
- hashed asset paths
- CSS and JS grouping
- preload helpers
- module script support
- production/development asset modes
- `AssetMap`
- `AssetMode`
- improved asset examples

Goal:

Make static assets easier to manage in Vix UI applications.

## v0.8.0 — Mobile/PWA direction

Status: done.

Focus:

- PWA-oriented helpers
- mobile viewport helpers
- safe-area CSS helpers
- web app manifest metadata
- installable web app support
- mobile meta tag helpers
- future mobile WebView preparation
- Android shell planning direction

Added primitives:

- `vix::ui::Viewport`
- `vix::ui::SafeArea`
- `vix::ui::WebAppManifest`
- `vix::ui::PwaMeta`

Goal:

Prepare Vix UI applications to run well on mobile browsers and future mobile WebView shells.

## v1.0.0 — Stable UI foundation

Status: planned.

Focus:

- stable public API
- stable module structure
- complete tests
- complete examples
- production-ready docs
- stable Vix response integration
- stable view rendering flow
- stable asset helpers
- stable form helpers
- stable live UI helpers
- stable PWA helpers
- stable app shell configuration
- desktop shell baseline
- clear migration notes from `0.x`

Goal:

Provide a reliable, simple UI foundation for Vix.cpp applications.

## Future direction after v1.0.0

Possible future work:

- deeper route-level helpers
- cleaner controller examples
- template layout conventions
- desktop packaging helpers
- Windows WebView backend
- macOS WebView backend
- Android WebView shell research
- service worker helper direction
- installable app examples
- dashboard starter examples
- admin panel starter examples

These should only be added if they keep the API simple.

## Non-goals

Vix UI is not trying to be:

- a React clone
- a Flutter clone
- a Qt replacement
- a native widget toolkit
- a complex virtual DOM engine
- a heavy frontend framework
- a JavaScript bundler
- a CSS framework
- a full desktop packaging system

The direction is:

```txt
server-rendered UI first
WebView app shell later
native UI only if truly needed
```

## Design principles

- keep the API small
- build on top of Vix templates
- avoid duplicate rendering engines
- prefer explicit data flow
- keep desktop/mobile as shells, not as a new UI system
- make simple web apps easy
- make dashboards and admin panels practical
- make future desktop apps possible
- make mobile/PWA readiness simple
- avoid adding complexity before it is truly needed
