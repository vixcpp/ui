# Vix UI Roadmap

This roadmap describes the planned evolution of `vix::ui`.

Vix UI is a web-first UI foundation for Vix.cpp applications.
It starts with server-rendered views, HTML helpers, assets, forms, and app shell primitives.
Desktop and mobile support will be added progressively through WebView-based app shells.

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

Focus:

- easier integration with `vix::App`
- easier integration with `vix::Response`
- helpers for rendering `vix::ui::View` directly from routes
- helpers for returning `vix::ui::HtmlResponse`
- conventions for templates, layouts, components and assets
- improved examples using real Vix routes

Possible API direction:

```cpp
app.get("/", [](Request &, Response &res)
{
  auto view = vix::ui::View("home.html")
      .set_title("Home")
      .set("name", "Gaspard");

  res.ui(view);
});
```

Goal:

Make Vix UI useful in normal Vix web applications, not only standalone examples.

## v0.3.0 — Desktop shell MVP

Focus:

- Linux-first desktop shell
- local Vix server launch support
- WebView window integration
- load `http://127.0.0.1:<port>`
- shell lifecycle: start, stop, restart
- window title, size, resizable and fullscreen options
- development mode support
- basic desktop example

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

Focus:

- improve Linux support
- prepare Windows support
- prepare macOS support
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

Focus:

- WebSocket-friendly UI helpers
- partial HTML fragments
- small component update conventions
- server-driven UI update patterns
- helpers for dashboards and internal tools
- flash messages
- simple notification/toast rendering helpers

Goal:

Make Vix UI useful for dynamic dashboards, admin panels, monitoring pages and internal tools.

## v0.6.0 — Forms and validation improvements

Focus:

- richer field types
- select options
- checkbox state
- radio groups
- file input helpers
- form data binding
- old input values
- field-level rendering helpers
- form error summaries
- CSRF helper integration point

Goal:

Make server-rendered forms more practical for real applications.

## v0.7.0 — Asset pipeline helpers

Focus:

- asset versioning
- manifest lookup
- hashed asset paths
- CSS and JS grouping
- preload helpers
- module script support
- production/development asset modes

Goal:

Make static assets easier to manage in Vix UI applications.

## v0.8.0 — Mobile/PWA direction

Focus:

- PWA-oriented helpers
- mobile viewport helpers
- manifest metadata
- installable web app support
- safe-area CSS helpers
- mobile WebView research
- Android shell planning

Goal:

Prepare Vix UI applications to run well on mobile browsers and future mobile WebView shells.

## v1.0.0 — Stable UI foundation

Focus:

- stable public API
- stable module structure
- complete tests
- complete examples
- production-ready docs
- Vix route integration
- asset helpers
- forms
- app shell configuration
- desktop shell baseline

Goal:

Provide a reliable, simple UI foundation for Vix.cpp applications.

## Non-goals

Vix UI is not trying to be:

- a React clone
- a Flutter clone
- a Qt replacement
- a native widget toolkit
- a complex virtual DOM engine
- a heavy frontend framework

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
- make future desktop apps possible
