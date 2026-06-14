# ============================================================================
# UI tests
# ============================================================================

function(ui_add_test target source)
  add_executable(${target}
    ${source}
  )

  target_link_libraries(${target}
    PRIVATE
      vix::ui
  )

  add_test(
    NAME ${target}
    COMMAND ${target}
  )
endfunction()

ui_add_test(ui_view_context_test core/view_context_test.cpp)
ui_add_test(ui_view_test core/view_test.cpp)

ui_add_test(ui_html_test html/html_test.cpp)
ui_add_test(ui_html_response_test html/html_response_test.cpp)

ui_add_test(ui_asset_test assets/asset_test.cpp)

ui_add_test(ui_form_test forms/form_test.cpp)

ui_add_test(ui_app_shell_test shell/app_shell_test.cpp)
