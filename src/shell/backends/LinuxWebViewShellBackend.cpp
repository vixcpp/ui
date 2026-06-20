/**
 *
 *  @file LinuxWebViewShellBackend.cpp
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
#include <vix/ui/shell/backends/LinuxWebViewShellBackend.hpp>

#include <utility>

#if defined(VIX_UI_ENABLE_LINUX_WEBVIEW)
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#endif

namespace vix::ui
{
  LinuxWebViewShellBackend::LinuxWebViewShellBackend(
      const LinuxWebViewShellBackend &)
  {
    reset_runtime_state();
  }

  LinuxWebViewShellBackend &LinuxWebViewShellBackend::operator=(
      const LinuxWebViewShellBackend &)
  {
    reset_runtime_state();
    return *this;
  }

  LinuxWebViewShellBackend::~LinuxWebViewShellBackend()
  {
    (void)stop();
  }

  std::unique_ptr<ShellBackend> LinuxWebViewShellBackend::clone() const
  {
    return std::make_unique<LinuxWebViewShellBackend>(*this);
  }

  std::string_view LinuxWebViewShellBackend::name() const noexcept
  {
    return "linux_webview";
  }

  Result<void> LinuxWebViewShellBackend::start(const ShellConfig &config)
  {
    if (running_)
    {
      return Result<void>::ok();
    }

    if (config.effective_url().empty())
    {
      return Result<void>::fail(
          ErrorCode::ShellError,
          "Linux WebView shell backend requires a target URL");
    }

#if defined(VIX_UI_ENABLE_LINUX_WEBVIEW)
    int argc = 0;
    char **argv = nullptr;

    if (!gtk_init_check(&argc, &argv))
    {
      return Result<void>::fail(
          ErrorCode::RuntimeError,
          "failed to initialize GTK for Linux WebView shell");
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *webview = webkit_web_view_new();

    window_ = window;
    webview_ = webview;
    running_ = true;

    gtk_window_set_title(
        GTK_WINDOW(window),
        config.title().c_str());

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        config.width(),
        config.height());

    gtk_window_set_resizable(
        GTK_WINDOW(window),
        config.resizable() ? TRUE : FALSE);

    if (config.fullscreen())
    {
      gtk_window_fullscreen(GTK_WINDOW(window));
    }

    if (config.devtools())
    {
      WebKitSettings *settings =
          webkit_web_view_get_settings(WEBKIT_WEB_VIEW(webview));

      webkit_settings_set_enable_developer_extras(settings, TRUE);
    }

    gtk_container_add(GTK_CONTAINER(window), webview);

    g_signal_connect(
        window,
        "destroy",
        G_CALLBACK(+[](GtkWidget *, gpointer data)
                   {
                     auto *self =
                         static_cast<LinuxWebViewShellBackend *>(data);

                     self->running_ = false;
                     self->reset_runtime_state();

                     gtk_main_quit();
                   }),
        this);

    const std::string url = config.effective_url();

    webkit_web_view_load_uri(
        WEBKIT_WEB_VIEW(webview),
        url.c_str());

    gtk_widget_show_all(window);

    gtk_main();

    running_ = false;
    reset_runtime_state();

    return Result<void>::ok();
#else
    return Result<void>::fail(
        ErrorCode::RuntimeError,
        "Linux WebView shell backend is not enabled in this build");
#endif
  }

  Result<void> LinuxWebViewShellBackend::stop()
  {
    if (!running_)
    {
      reset_runtime_state();
      return Result<void>::ok();
    }

#if defined(VIX_UI_ENABLE_LINUX_WEBVIEW)
    if (window_ != nullptr)
    {
      gtk_widget_destroy(GTK_WIDGET(window_));
    }

    running_ = false;
    reset_runtime_state();

    return Result<void>::ok();
#else
    running_ = false;
    reset_runtime_state();

    return Result<void>::ok();
#endif
  }

  bool LinuxWebViewShellBackend::running() const noexcept
  {
    return running_;
  }

  std::string LinuxWebViewShellBackend::status() const
  {
    return running_ ? "running" : "stopped";
  }

  void LinuxWebViewShellBackend::reset_runtime_state() noexcept
  {
    window_ = nullptr;
    webview_ = nullptr;
  }

} // namespace vix::ui
