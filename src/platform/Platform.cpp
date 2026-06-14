/**
 *
 *  @file Platform.cpp
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
#include <vix/ui/platform/Platform.hpp>

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] PlatformFamily family_for(PlatformKind kind) noexcept
    {
      switch (kind)
      {
      case PlatformKind::Linux:
      case PlatformKind::Windows:
      case PlatformKind::MacOS:
        return PlatformFamily::Desktop;

      case PlatformKind::Android:
      case PlatformKind::IOS:
        return PlatformFamily::Mobile;

      case PlatformKind::Web:
        return PlatformFamily::Web;

      case PlatformKind::Unknown:
        return PlatformFamily::Unknown;
      }

      return PlatformFamily::Unknown;
    }
  } // namespace

  std::string_view to_string(PlatformKind kind) noexcept
  {
    switch (kind)
    {
    case PlatformKind::Unknown:
      return "unknown";

    case PlatformKind::Linux:
      return "linux";

    case PlatformKind::Windows:
      return "windows";

    case PlatformKind::MacOS:
      return "macos";

    case PlatformKind::Android:
      return "android";

    case PlatformKind::IOS:
      return "ios";

    case PlatformKind::Web:
      return "web";
    }

    return "unknown";
  }

  std::string_view to_string(PlatformFamily family) noexcept
  {
    switch (family)
    {
    case PlatformFamily::Unknown:
      return "unknown";

    case PlatformFamily::Desktop:
      return "desktop";

    case PlatformFamily::Mobile:
      return "mobile";

    case PlatformFamily::Web:
      return "web";
    }

    return "unknown";
  }

  Platform::Platform(PlatformKind kind) noexcept
      : kind_(kind)
  {
  }

  Platform Platform::current() noexcept
  {
#if defined(__ANDROID__)
    return Platform(PlatformKind::Android);
#elif defined(__APPLE__) && defined(__MACH__)
    return Platform(PlatformKind::MacOS);
#elif defined(_WIN32)
    return Platform(PlatformKind::Windows);
#elif defined(__linux__)
    return Platform(PlatformKind::Linux);
#else
    return Platform(PlatformKind::Unknown);
#endif
  }

  Platform Platform::web() noexcept
  {
    return Platform(PlatformKind::Web);
  }

  Platform Platform::linux() noexcept
  {
    return Platform(PlatformKind::Linux);
  }

  Platform Platform::windows() noexcept
  {
    return Platform(PlatformKind::Windows);
  }

  Platform Platform::macos() noexcept
  {
    return Platform(PlatformKind::MacOS);
  }

  Platform Platform::android() noexcept
  {
    return Platform(PlatformKind::Android);
  }

  Platform Platform::ios() noexcept
  {
    return Platform(PlatformKind::IOS);
  }

  PlatformKind Platform::kind() const noexcept
  {
    return kind_;
  }

  PlatformFamily Platform::family() const noexcept
  {
    return family_for(kind_);
  }

  bool Platform::known() const noexcept
  {
    return kind_ != PlatformKind::Unknown;
  }

  bool Platform::unknown() const noexcept
  {
    return kind_ == PlatformKind::Unknown;
  }

  bool Platform::desktop() const noexcept
  {
    return family() == PlatformFamily::Desktop;
  }

  bool Platform::mobile() const noexcept
  {
    return family() == PlatformFamily::Mobile;
  }

  bool Platform::web_platform() const noexcept
  {
    return family() == PlatformFamily::Web;
  }

  bool Platform::is_linux() const noexcept
  {
    return kind_ == PlatformKind::Linux;
  }

  bool Platform::is_windows() const noexcept
  {
    return kind_ == PlatformKind::Windows;
  }

  bool Platform::is_macos() const noexcept
  {
    return kind_ == PlatformKind::MacOS;
  }

  bool Platform::is_android() const noexcept
  {
    return kind_ == PlatformKind::Android;
  }

  bool Platform::is_ios() const noexcept
  {
    return kind_ == PlatformKind::IOS;
  }

  std::string_view Platform::name() const noexcept
  {
    return to_string(kind_);
  }

} // namespace vix::ui
