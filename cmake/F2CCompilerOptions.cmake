
function(f2c_set_compiler_options target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /WX)
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Werror -pedantic
      -Wno-unused-parameter -Wno-sign-compare
    )
  endif()

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    include(cmake/Modules/CodeCoverage.cmake)
    APPEND_COVERAGE_COMPILER_FLAGS()
    if(MSVC)
      target_compile_options(${target} PRIVATE /Od /Zi)
    else()
      target_compile_options(${target} PRIVATE -g --coverage -fprofile-arcs -ftest-coverage -fno-inline -O0)
      target_link_options(${target} PRIVATE --coverage)
      target_link_libraries(${target} PRIVATE gcov)

    endif()
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
      target_compile_options(${target} PRIVATE /O2)
    else()
      target_compile_options(${target} PRIVATE -O2)
    endif()
  endif()
endfunction()
