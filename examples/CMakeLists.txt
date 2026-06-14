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
