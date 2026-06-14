cmake_minimum_required(VERSION 3.20)

# ============================================================================
# Re-entry guard
# Prevent the ui module from being configured/exported twice
# ============================================================================

get_property(_VIX_UI_MODULE_ALREADY_INCLUDED GLOBAL PROPERTY VIX_UI_MODULE_ALREADY_INCLUDED)
if(_VIX_UI_MODULE_ALREADY_INCLUDED)
  return()
endif()
set_property(GLOBAL PROPERTY VIX_UI_MODULE_ALREADY_INCLUDED TRUE)

project(ui
  VERSION 0.1.0
  DESCRIPTION "Web-first UI and app shell primitives for Vix.cpp applications"
  LANGUAGES CXX
)

# ============================================================================
# Options
# ============================================================================

option(UI_BUILD_TESTS "Build ui tests" ON)
option(UI_BUILD_EXAMPLES "Build ui examples" ON)
option(UI_BUILD_BENCHMARKS "Build ui benchmarks" OFF)
option(UI_INSTALL "Enable standalone install/package rules" ON)

if(DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  set(UI_BUILD_TESTS OFF CACHE BOOL "Build ui tests" FORCE)
  set(UI_BUILD_EXAMPLES OFF CACHE BOOL "Build ui examples" FORCE)
  set(UI_BUILD_BENCHMARKS OFF CACHE BOOL "Build ui benchmarks" FORCE)
  set(UI_INSTALL OFF CACHE BOOL "Enable standalone install/package rules" FORCE)
endif()

# ============================================================================
# C++ standard
# ============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Default build type
# ============================================================================

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ============================================================================
# Dependencies for install/export
# ============================================================================

include(GNUInstallDirs)

# ============================================================================
# Dependencies
# ============================================================================

if(NOT TARGET vix::template AND NOT TARGET template)
  find_package(template QUIET CONFIG)
endif()

if(NOT TARGET vix::template AND NOT TARGET template)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../template/CMakeLists.txt")
    add_subdirectory(
      "${CMAKE_CURRENT_SOURCE_DIR}/../template"
      "${CMAKE_CURRENT_BINARY_DIR}/_deps/template"
    )
  endif()
endif()

if(TARGET vix::template)
  set(_VIX_UI_TEMPLATE_TARGET vix::template)
elseif(TARGET template)
  set(_VIX_UI_TEMPLATE_TARGET template)
else()
  message(FATAL_ERROR "[ui] missing required dependency: vix::template")
endif()

# ============================================================================
# Library
# ============================================================================

set(UI_SOURCES
  src/Version.cpp

  src/support/Error.cpp
  src/support/Result.cpp

  src/core/ViewContext.cpp
  src/core/ViewResult.cpp
  src/core/View.cpp

  src/html/HtmlEscape.cpp
  src/html/HtmlAttrs.cpp
  src/html/Html.cpp
  src/html/HtmlResponse.cpp

  src/assets/Asset.cpp
  src/assets/AssetManifest.cpp
  src/assets/AssetManager.cpp

  src/forms/ValidationError.cpp
  src/forms/Field.cpp
  src/forms/Form.cpp

  src/platform/Platform.cpp

  src/shell/ShellConfig.cpp
  src/shell/AppShell.cpp
)

add_library(ui STATIC ${UI_SOURCES})

if(NOT TARGET vix::ui)
  add_library(vix::ui ALIAS ui)
endif()

target_include_directories(ui
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(ui PUBLIC cxx_std_20)

target_link_libraries(ui PUBLIC ${_VIX_UI_TEMPLATE_TARGET})

# ============================================================================
# Warnings
# ============================================================================

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(ui
    PRIVATE
      -Wall
      -Wextra
      -Wpedantic
  )
elseif(MSVC)
  target_compile_options(ui
    PRIVATE
      /W4
  )
endif()

# ============================================================================
# Version macros
# ============================================================================

target_compile_definitions(ui
  PUBLIC
    UI_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    UI_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    UI_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

# ============================================================================
# Examples
# ============================================================================

if(UI_BUILD_EXAMPLES)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
  else()
    message(STATUS "[ui] examples/ missing CMakeLists.txt -> skipping")
  endif()
endif()

# ============================================================================
# Tests
# ============================================================================

if(UI_BUILD_TESTS)
  enable_testing()
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
  else()
    message(STATUS "[ui] tests/ missing CMakeLists.txt -> skipping")
  endif()
endif()

# ============================================================================
# Benchmarks
# ============================================================================

if(UI_BUILD_BENCHMARKS)
  find_package(benchmark QUIET)

  if(benchmark_FOUND)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/CMakeLists.txt")
      add_subdirectory(benchmarks)
    else()
      message(STATUS "[ui] benchmarks/ missing CMakeLists.txt -> skipping")
    endif()
  else()
    message(STATUS "Google Benchmark not found, benchmarks will not be built")
  endif()
endif()

# ============================================================================
# Install for Vix umbrella export
# ============================================================================

if(DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  install(
    TARGETS ui
    EXPORT VixTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endif()

# ============================================================================
# Standalone install/package
# ============================================================================

if(UI_INSTALL AND NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
  include(CMakePackageConfigHelpers)

  install(
    TARGETS ui
    EXPORT uiTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    EXPORT uiTargets
    FILE uiTargets.cmake
    NAMESPACE vix::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ui
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/uiConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ui
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/uiConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/uiConfig.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/uiConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ui
  )
endif()
