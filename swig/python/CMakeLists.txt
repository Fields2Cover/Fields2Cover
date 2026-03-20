# Include python
find_package(
  Python REQUIRED
  COMPONENTS
    Interpreter
    Development
)

if(Python_VERSION VERSION_GREATER_EQUAL 3)
  list(APPEND CMAKE_SWIG_FLAGS "-py3;-DPY3")
endif()
if(BUILD_VISUALIZER)
  list(APPEND CMAKE_SWIG_FLAGS "-DFIELDS2COVER_BUILD_VISUALIZER")
endif()

if(BUILD_DOC)
  list(APPEND CMAKE_SWIG_FLAGS "-doxygen")
endif()

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(
  ../${PROJECT_NAME}.i
  PROPERTIES CPLUSPLUS ON
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
  ${Python_INCLUDE_DIRS}
)

# Add swig module
SWIG_ADD_LIBRARY(fields2cover_python
  LANGUAGE python
  SOURCES ../${PROJECT_NAME}.i
)
SWIG_LINK_LIBRARIES(fields2cover_python
  Fields2Cover
  ${Python_LIBRARIES}
  ${PROJ_LIBRARY}
  ${GDAL_LIBRARIES}
)

# Files to install with Python
set(
  PYTHON_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/fields2cover.py
  ${CMAKE_CURRENT_BINARY_DIR}/_fields2cover_python.so
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
  ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
  COPYONLY
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/fields2cover/__init__.py
  COPYONLY
)

file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../src
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fields2cover
)
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../include
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fields2cover
)


# Install Python files directly using CMake
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/fields2cover.py
  $<TARGET_FILE:fields2cover_python>
  DESTINATION lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/dist-packages
)



