# ============================================================================
# UI benchmarks
# ============================================================================

function(ui_add_benchmark target source)
  add_executable(${target}
    ${source}
  )

  target_compile_features(${target}
    PRIVATE
      cxx_std_20
  )

  target_link_libraries(${target}
    PRIVATE
      vix::ui
      benchmark::benchmark
      benchmark::benchmark_main
  )

  if(VIX_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(${target}
      PRIVATE
        -fno-omit-frame-pointer
        -fsanitize=address,undefined
    )

    target_link_options(${target}
      PRIVATE
        -fsanitize=address,undefined
    )
  endif()
endfunction()

ui_add_benchmark(ui_html_benchmark html_benchmark.cpp)
ui_add_benchmark(ui_assets_benchmark assets_benchmark.cpp)
ui_add_benchmark(ui_forms_benchmark forms_benchmark.cpp)
ui_add_benchmark(ui_live_benchmark live_benchmark.cpp)
ui_add_benchmark(ui_view_context_benchmark view_context_benchmark.cpp)
