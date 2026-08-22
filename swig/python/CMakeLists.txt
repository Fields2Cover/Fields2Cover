# Python bindings (SWIG). Consumed two ways:
#   * by scikit-build-core (pip/uv build) -- SKBUILD is defined, install
#     destinations are relative to the wheel's platlib root;
#   * by a plain `cmake --install` from a source checkout -- the module is
#     installed straight into the interpreter's site-packages.
# Only rules tagged COMPONENT python end up in the wheel.

if(CMAKE_VERSION VERSION_LESS 3.18)
  message(FATAL_ERROR "BUILD_PYTHON requires CMake >= 3.18 (FindPython Development.Module)")
endif()

# Development.Module (not Development): don't require libpython, link the
# extension module against Python::Module, which handles
# "-undefined dynamic_lookup" on macOS and no libpython on Linux/manylinux.
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
# UseSWIG ignores target INCLUDE_DIRECTORIES for the swig -I flags unless
# opted in; without this, swig fails to resolve the %include headers below.
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
      "Where `cmake --install` puts the fields2cover python package (default: the interpreter's site-packages)")
  # Outside a wheel, Fields2Cover may be a shared library installed to
  # CMAKE_INSTALL_PREFIX/lib; let the extension find it there.
  # CMAKE_INSTALL_PREFIX may be relative (CI passes ../instdir); a relative
  # RPATH would be resolved against the process' cwd, so make it absolute.
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
