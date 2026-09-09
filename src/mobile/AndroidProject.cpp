/**
 *
 *  @file AndroidProject.cpp
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
#include <vix/ui/mobile/AndroidProject.hpp>

#include <cctype>
#include <fstream>
#include <sstream>
#include <utility>

namespace vix::ui
{
  namespace
  {
    [[nodiscard]] std::string safe_project_name(const std::string &name)
    {
      if (name.empty())
      {
        return "VixMobile";
      }

      std::string result;

      for (char character : name)
      {
        const unsigned char value = static_cast<unsigned char>(character);

        if (std::isalnum(value))
        {
          result.push_back(character);
        }
        else if (character == ' ' || character == '-' || character == '_')
        {
          result.push_back('_');
        }
      }

      if (result.empty())
      {
        return "VixMobile";
      }

      if (std::isdigit(static_cast<unsigned char>(result.front())))
      {
        result.insert(result.begin(), 'V');
      }

      return result;
    }

    [[nodiscard]] std::string gradle_string(std::string_view value)
    {
      std::string result;
      result.reserve(value.size());

      for (char character : value)
      {
        if (character == '\\' || character == '\'')
        {
          result.push_back('\\');
        }

        result.push_back(character);
      }

      return result;
    }

    [[nodiscard]] std::string render_settings_gradle(const AndroidProject &project)
    {
      std::ostringstream output;

      output
          << "pluginManagement {\n"
          << "    repositories {\n"
          << "        google()\n"
          << "        mavenCentral()\n"
          << "        gradlePluginPortal()\n"
          << "    }\n"
          << "}\n\n"
          << "dependencyResolutionManagement {\n"
          << "    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)\n"
          << "    repositories {\n"
          << "        google()\n"
          << "        mavenCentral()\n"
          << "    }\n"
          << "}\n\n"
          << "rootProject.name = '" << safe_project_name(project.config().name())
          << "'\n"
          << "include ':app'\n";

      return output.str();
    }

    [[nodiscard]] std::string render_root_build_gradle(
        const AndroidProject &project)
    {
      return "plugins {\n"
             "    id 'com.android.application' version '" +
             gradle_string(project.android_gradle_plugin_version()) +
             "' apply false\n"
             "}\n";
    }

    [[nodiscard]] std::string render_gradle_properties()
    {
      return "org.gradle.jvmargs=-Xmx2048m -Dfile.encoding=UTF-8\n"
             "android.useAndroidX=false\n";
    }

    [[nodiscard]] std::string render_app_build_gradle(
        const AndroidProject &project)
    {
      const MobileConfig &config = project.config();
      std::ostringstream output;

      output
          << "plugins {\n"
          << "    id 'com.android.application'\n"
          << "}\n\n"
          << "android {\n"
          << "    namespace '" << gradle_string(config.app_id()) << "'\n"
          << "    compileSdk " << project.compile_sdk() << "\n\n"
          << "    defaultConfig {\n"
          << "        applicationId '" << gradle_string(config.app_id()) << "'\n"
          << "        minSdk " << project.min_sdk() << "\n"
          << "        targetSdk " << project.target_sdk() << "\n"
          << "        versionCode " << project.version_code() << "\n"
          << "        versionName '" << gradle_string(config.version()) << "'\n"
          << "    }\n"
          << "}\n";

      return output.str();
    }

    [[nodiscard]] Result<void> write_text_file(
        const std::filesystem::path &path,
        const std::string &content)
    {
      std::ofstream output(path, std::ios::binary | std::ios::trunc);

      if (!output.is_open())
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "cannot write Android project file: " + path.string());
      }

      output << content;

      if (!output.good())
      {
        return Result<void>::fail(
            ErrorCode::RuntimeError,
            "failed while writing Android project file: " + path.string());
      }

      return Result<void>::ok();
    }
  } // namespace

  AndroidProject::AndroidProject(MobileProject project)
      : MobileProject(std::move(project))
  {
  }

  AndroidProject &AndroidProject::set_min_sdk(int min_sdk) noexcept
  {
    min_sdk_ = min_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_target_sdk(int target_sdk) noexcept
  {
    target_sdk_ = target_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_compile_sdk(int compile_sdk) noexcept
  {
    compile_sdk_ = compile_sdk;
    return *this;
  }

  AndroidProject &AndroidProject::set_version_code(int version_code) noexcept
  {
    version_code_ = version_code;
    return *this;
  }

  AndroidProject &AndroidProject::set_android_gradle_plugin_version(
      std::string version)
  {
    android_gradle_plugin_version_ = std::move(version);
    return *this;
  }

  int AndroidProject::min_sdk() const noexcept
  {
    return min_sdk_;
  }

  int AndroidProject::target_sdk() const noexcept
  {
    return target_sdk_;
  }

  int AndroidProject::compile_sdk() const noexcept
  {
    return compile_sdk_;
  }

  int AndroidProject::version_code() const noexcept
  {
    return version_code_;
  }

  const std::string &AndroidProject::android_gradle_plugin_version() const noexcept
  {
    return android_gradle_plugin_version_;
  }

  Result<void> AndroidProject::generate(
      const std::filesystem::path &directory) const
  {
    Result<void> validation = validate();
    if (validation.is_failed())
    {
      return validation;
    }

    if (directory.empty())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project output directory must not be empty");
    }

    std::error_code error;
    std::filesystem::create_directories(directory / "app", error);
    if (error)
    {
      return Result<void>::fail(
          ErrorCode::RuntimeError,
          "cannot create Android project directory: " +
              directory.string() + ": " + error.message());
    }

    const std::pair<std::filesystem::path, std::string> files[]{
        {directory / "settings.gradle", render_settings_gradle(*this)},
        {directory / "build.gradle", render_root_build_gradle(*this)},
        {directory / "gradle.properties", render_gradle_properties()},
        {directory / "app" / "build.gradle", render_app_build_gradle(*this)}};

    for (const auto &[path, content] : files)
    {
      Result<void> write_result = write_text_file(path, content);
      if (write_result.is_failed())
      {
        return write_result;
      }
    }

    return Result<void>::ok();
  }

  Result<void> AndroidProject::validate() const
  {
    Result<void> mobile_validation = MobileProject::validate();
    if (mobile_validation.is_failed())
    {
      return mobile_validation;
    }

    if (min_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project min SDK must be greater than zero");
    }

    if (target_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project target SDK must be greater than zero");
    }

    if (compile_sdk_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project compile SDK must be greater than zero");
    }

    if (version_code_ <= 0)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project version code must be greater than zero");
    }

    if (min_sdk_ > target_sdk_)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project min SDK must not exceed target SDK");
    }

    if (target_sdk_ > compile_sdk_)
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project target SDK must not exceed compile SDK");
    }

    if (android_gradle_plugin_version_.empty())
    {
      return Result<void>::fail(
          ErrorCode::ConfigError,
          "Android project Android Gradle Plugin version must not be empty");
    }

    return Result<void>::ok();
  }

} // namespace vix::ui
