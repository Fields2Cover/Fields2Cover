# Include python
find_package(
  Python REQUIRED
  COMPONENTS
    Interpreter
    Development
)

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/fields2cover)
file(MAKE_DIRECTORY ${CMAKE_SWIG_OUTDIR})

set(CMAKE_SWIG_FLAGS "")
if(Python_VERSION VERSION_GREATER_EQUAL 3)
  list(APPEND CMAKE_SWIG_FLAGS "-py3;-DPY3")
endif()

set(CMAKE_SWIG_FLAGS
    -I${PROJECT_SOURCE_DIR}/include
)

if(BUILD_DOC)
  list(APPEND CMAKE_SWIG_FLAGS "-doxygen")
endif()

set_source_files_properties(
  ../${PROJECT_NAME}.i
  PROPERTIES CPLUSPLUS ON SWIG_FLAGS ""
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
  ${Python_INCLUDE_DIRS}
)

# Add swig module
SWIG_ADD_LIBRARY(fields2cover
  TYPE MODULE
  LANGUAGE python
  SOURCES ../Fields2Cover.i
)

target_include_directories(fields2cover PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(fields2cover PROPERTIES
  OUTPUT_NAME "fields2cover_core"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR}
)

target_link_libraries(fields2cover
  PRIVATE
  Fields2Cover
  Python::Python
)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/fields2cover")

# Files to install with Python
set(
  PYTHON_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/fields2cover/_fields2cover_core.so
)

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

set(PYPROJECT_IN ${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml)
set(PYPROJECT_OUT ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml)
configure_file(${PYPROJECT_IN} ${PYPROJECT_OUT} COPYONLY)

set(MANIFEST_IN ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.in)
set(MANIFEST_OUT ${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in)
configure_file(
  ${MANIFEST_IN}
  ${MANIFEST_OUT}
  COPYONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE
  ${CMAKE_CURRENT_BINARY_DIR}/LICENSE
  COPYONLY
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../README.rst
  ${CMAKE_CURRENT_BINARY_DIR}/README.rst
  COPYONLY
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/fields2cover/__init__.py
  COPYONLY
)

execute_process(
  COMMAND ${Python_EXECUTABLE} -c
    "import sysconfig; print(sysconfig.get_path('purelib'))"
  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
  OUTPUT_STRIP_TRAILING_WHITESPACE
)


install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/fields2cover/fields2cover_core.py
    DESTINATION ${PYTHON_SITE_PACKAGES}/fields2cover
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/fields2cover/__init__.py
    DESTINATION ${PYTHON_SITE_PACKAGES}/fields2cover
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/fields2cover/_fields2cover_core.so
    DESTINATION ${PYTHON_SITE_PACKAGES}/fields2cover
)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/../../libFields2Cover.so
#    DESTINATION ${PYTHON_SITE_PACKAGES}/fields2cover
#)





