cmake_minimum_required(VERSION 3.20)

project(vix_example_ui LANGUAGES CXX)

if (NOT TARGET vix::ui)
  find_package(Vix CONFIG REQUIRED)
endif()

set(_vix_ui_examples
  01_basic_view
  02_html_response
  03_assets
  04_forms
  05_shell_config_v04
  06_fragment
  07_live_update
  08_flash_and_toast
  09_rich_forms
  10_form_binding
  11_asset_pipeline
  12_asset_manifest
  13_pwa_helpers
  14_web_app_manifest
  desktop_shell_basic
  desktop_shell_readiness
  desktop_shell_with_server
)

foreach (_example IN LISTS _vix_ui_examples)
  add_executable("ui_${_example}" "${_example}.cpp")
  target_compile_features("ui_${_example}" PRIVATE cxx_std_20)
  target_link_libraries("ui_${_example}" PRIVATE vix::ui)
endforeach()

# This example directly uses the public template API in addition to vix::ui.
target_link_libraries(ui_01_basic_view PRIVATE vix::template)

# Desktop shell examples require a platform shell or a configured local server.
enable_testing()
foreach (_example IN ITEMS
    01_basic_view
    02_html_response
    03_assets
    04_forms
    05_shell_config_v04
    06_fragment
    07_live_update
    08_flash_and_toast
    09_rich_forms
    10_form_binding
    11_asset_pipeline
    12_asset_manifest
    13_pwa_helpers
    14_web_app_manifest)
  add_test(NAME "ui.${_example}" COMMAND "ui_${_example}")
endforeach()
