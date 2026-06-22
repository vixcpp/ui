# ============================================================================
# UI examples
# ============================================================================

add_executable(ui_example_basic_view
  01_basic_view.cpp
)

target_link_libraries(ui_example_basic_view
  PRIVATE
    vix::ui
)

add_executable(ui_example_html_response
  02_html_response.cpp
)

target_link_libraries(ui_example_html_response
  PRIVATE
    vix::ui
)

add_executable(ui_example_assets
  03_assets.cpp
)

target_link_libraries(ui_example_assets
  PRIVATE
    vix::ui
)

add_executable(ui_example_desktop_shell_basic
  desktop_shell_basic.cpp
)

target_link_libraries(ui_example_desktop_shell_basic
  PRIVATE
    vix::ui
)

add_executable(ui_example_desktop_shell_with_server
  desktop_shell_with_server.cpp
)

target_link_libraries(ui_example_desktop_shell_with_server
  PRIVATE
    vix::ui
)

add_executable(ui_example_forms
  04_forms.cpp
)

target_link_libraries(ui_example_forms
  PRIVATE
    vix::ui
)

add_executable(ui_example_shell_config_v04
  05_shell_config_v04.cpp
)

target_link_libraries(ui_example_shell_config_v04
  PRIVATE
    vix::ui
)

add_executable(ui_example_fragment
  06_fragment.cpp
)

target_link_libraries(ui_example_fragment
  PRIVATE
    vix::ui
)

add_executable(ui_example_live_update
  07_live_update.cpp
)

target_link_libraries(ui_example_live_update
  PRIVATE
    vix::ui
)

add_executable(ui_example_flash_and_toast
  08_flash_and_toast.cpp
)

target_link_libraries(ui_example_flash_and_toast
  PRIVATE
    vix::ui
)
