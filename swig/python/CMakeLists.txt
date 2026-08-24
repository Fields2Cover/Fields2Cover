# SWIG python bindings, built either by scikit-build-core (SKBUILD defined,
# wheel layout) or by a plain cmake --install (module goes to site-packages).
# Only install rules tagged COMPONENT python end up in the wheel.

if(CMAKE_VERSION VERSION_LESS 3.18)
  message(FATAL_ERROR "BUILD_PYTHON requires CMake >= 3.18 (FindPython Development.Module)")
endif()

# Development.Module: link Python::Module without libpython (required for
# manylinux wheels; handles -undefined dynamic_lookup on macOS).
find_package(Python 3.9 REQUIRED COMPONENTS Interpreter Development.Module)

if(BUILD_DOC)
  list(APPEND CMAKE_SWIG_FLAGS "-doxygen")
endif()

set_source_files_properties(../${PROJECT_NAME}.i PROPERTIES CPLUSPLUS ON)

swig_add_library(fields2cover_python
  LANGUAGE python
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
  SOURCES ../${PROJECT_NAME}.i
)
target_include_directories(fields2cover_python PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)
# Pass the target's include directories to swig -I (UseSWIG opt-in).
set_property(TARGET fields2cover_python PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)
target_link_libraries(fields2cover_python PRIVATE
  Fields2Cover
  Python::Module
)

set(F2C_PYTHON_PACKAGE_DIR fields2cover)
if(DEFINED SKBUILD)
  set(F2C_PYTHON_INSTALL_DIR ${F2C_PYTHON_PACKAGE_DIR})
else()
  set(F2C_PYTHON_INSTALL_DIR "${Python_SITEARCH}/${F2C_PYTHON_PACKAGE_DIR}" CACHE PATH
      "Install directory of the fields2cover python package (default: site-packages)")
  # RPATH to <prefix>/lib where libFields2Cover.so is installed. The prefix may
  # be relative (CI passes ../instdir), so resolve it against the build dir.
  get_filename_component(F2C_ABS_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" ABSOLUTE
                         BASE_DIR "${CMAKE_BINARY_DIR}")
  set_target_properties(fields2cover_python PROPERTIES
    INSTALL_RPATH "${F2C_ABS_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
  )
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
  @ONLY
)

install(TARGETS fields2cover_python
  LIBRARY DESTINATION ${F2C_PYTHON_INSTALL_DIR}
  COMPONENT python
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
  ${CMAKE_CURRENT_BINARY_DIR}/fields2cover.py
  DESTINATION ${F2C_PYTHON_INSTALL_DIR}
  COMPONENT python
)
