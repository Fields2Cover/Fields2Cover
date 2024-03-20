
set(FIELDS2COVER_MAJOR_VERSION "1")
set(FIELDS2COVER_MINOR_VERSION "2")
set(FIELDS2COVER_PATCH_VERSION "1")
set(FIELDS2COVER_VERSION_STRING
  "${FIELDS2COVER_MAJOR_VERSION}.${FIELDS2COVER_MINOR_VERSION}.${FIELDS2COVER_PATCH_VERSION}"
)
set(FIELDS2COVER_VERSION
  ${FIELDS2COVER_MAJOR_VERSION}.${FIELDS2COVER_MINOR_VERSION}.${FIELDS2COVER_PATCH_VERSION}
)

set(PACKAGE_NAME "Fields2Cover")
set(PACKAGE_DESCRIPTION
  "Fields2Cover is a complete coverage path planning package for autonomous robots"
)
set(PACKAGE_AUTHOR "Wageningen University")
set(PACKAGE_MAINTAINER "Gonzalo Mier")
set(PACKAGE_URL "http://fields2cover.github.io")


option(ALLOW_PARALLELIZATION "Allow parallel algorithms" OFF)
option(BUILD_TUTORIALS "Build tutorials" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_PYTHON "Build Python SWIG module" OFF)
option(BUILD_DOC "Build Documentation" OFF)
option(BUILD_SHARED_LIBS "Build shared library(.so)" ON)



if (BUILD_PYTHON)
  cmake_minimum_required(VERSION 3.12.4)
else(BUILD_PYTHON)
  cmake_minimum_required(VERSION 3.11)
endif(BUILD_PYTHON)

if(${CMAKE_VERSION} VERSION_LESS 3.22)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else(${CMAKE_VERSION} VERSION_LESS 3.22)
  cmake_policy(VERSION 3.22)
endif(${CMAKE_VERSION} VERSION_LESS 3.22)

if(POLICY CMP0094)
  cmake_policy(SET CMP0094 NEW)
endif(POLICY CMP0094)
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif(POLICY CMP0135)

project(Fields2Cover
  LANGUAGES CXX
  VERSION "${FIELDS2COVER_VERSION_STRING}"
  DESCRIPTION "${PACKAGE_DESCRIPTION}"
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  # set(CMAKE_BUILD_TYPE Testing)
  # set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif(WIN32)

set(INSTALL_CMAKE_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Fields2Cover)

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

include(FetchContent)
if(${CMAKE_VERSION} VERSION_LESS 3.14)
  macro(FetchContent_MakeAvailable NAME)
    FetchContent_GetProperties(${NAME})
    if(NOT ${NAME}_POPULATED)
      FetchContent_Populate(${NAME})
      add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR})
    endif(NOT ${NAME}_POPULATED)
  endmacro(FetchContent_MakeAvailable)
endif(${CMAKE_VERSION} VERSION_LESS 3.14)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
FetchContent_Declare(json FETCHCONTENT_UPDATES_DISCONNECTED
  URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)
FetchContent_Declare(tinyxml2 FETCHCONTENT_UPDATES_DISCONNECTED
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
  GIT_TAG c2d30872e20621955ca7feb9168bad996d591a19
)
FetchContent_Declare(steering_functions FETCHCONTENT_UPDATES_DISCONNECTED
  GIT_REPOSITORY https://github.com/Fields2Cover/steering_functions.git
  GIT_TAG 33fc010017efa1ef2c8a2d4779fcda94b4b30d20
)
FetchContent_Declare(spline FETCHCONTENT_UPDATES_DISCONNECTED
  GIT_REPOSITORY https://github.com/Fields2Cover/spline.git
  GIT_TAG 1b5d4bad29082997076b264de84ca6d46c2ae6ab
)
FetchContent_Declare(matplotlib_cpp FETCHCONTENT_UPDATES_DISCONNECTED
  GIT_REPOSITORY https://github.com/Fields2Cover/matplotlib-cpp.git
  GIT_TAG 75c15d0c907a4b68bca5ef97032302bd14ccab8e
)

FetchContent_MakeAvailable(
  json
  tinyxml2
  steering_functions
  spline
  matplotlib_cpp
)

find_package(GDAL 3.0 REQUIRED)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files"
)


foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif(NOT IS_ABSOLUTE "${${var}}")
endforeach()

file(GLOB_RECURSE fields2cover_src
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*/*.cpp"
)

list(REMOVE_ITEM fields2cover_src
  "${CMAKE_CURRENT_SOURCE_DIR}/.*"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/.*"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*/.*"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/.*"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*/.*.cpp"
)

if(BUILD_SHARED_LIBS)
  add_library(Fields2Cover SHARED ${fields2cover_src})
else(BUILD_SHARED_LIBS)
  add_library(Fields2Cover STATIC ${fields2cover_src})
endif(BUILD_SHARED_LIBS)

target_compile_features(Fields2Cover PUBLIC cxx_std_17)


if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  target_compile_options(Fields2Cover PRIVATE
    -pedantic -Wall -Wfatal-errors
    -Wno-dev -Wextra -Wno-unused-parameter -Wno-sign-compare
    -Wno-attributes -Wl,--no-undefined -Wshadow
  )
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  target_compile_options(Fields2Cover PRIVATE /W4)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "^(Apple)?Clang$")

endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(cmake/Modules/CodeCoverage.cmake)
  APPEND_COVERAGE_COMPILER_FLAGS()
  target_compile_options(Fields2Cover PRIVATE
    -g --coverage -fprofile-arcs -ftest-coverage -fno-inline
  )
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Fields2Cover PRIVATE /O0)
  else(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Fields2Cover PRIVATE -O0)
  endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

else (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    target_compile_options(Fields2Cover PRIVATE /O2)
  else(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    target_compile_options(Fields2Cover PRIVATE -O2)
  endif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")



target_include_directories(Fields2Cover PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${EIGEN3_INCLUDE_DIRS}
)


target_link_libraries(Fields2Cover
  PUBLIC
    ${GDAL_LIBRARIES}
    -lpthread
    -lgeos_c
  PRIVATE
    nlohmann_json::nlohmann_json
    tinyxml2::tinyxml2
    steering_functions
    spline
    matplotlib_cpp
)

if(ALLOW_PARALLELIZATION)
  target_link_libraries(Fields2Cover PRIVATE tbb)
endif(ALLOW_PARALLELIZATION)

set_target_properties(Fields2Cover
  PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  PUBLIC_HEADER "include/fields2cover.h"
)

install(
  TARGETS
    Fields2Cover
    tinyxml2
    steering_functions
    nlohmann_json
  EXPORT Fields2Cover-targets
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
  PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(Fields2CoverConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

if(BUILD_SHARED_LIBS)
  set(type shared)
else(BUILD_SHARED_LIBS)
  set(type static)
endif(BUILD_SHARED_LIBS)

install(
  EXPORT Fields2Cover-targets
  FILE Fields2Cover-${type}-Targets.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}/cmake/fields2cover
)

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(cmake/Fields2CoverConfig.cmake.in
  "Fields2CoverConfig.cmake" @ONLY)
configure_file(cmake/Fields2CoverConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/Fields2CoverConfigVersion.cmake" @ONLY
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfigVersion.cmake"
  DESTINATION
    ${INSTALL_CMAKE_DIR}/cmake/fields2cover
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/package.xml
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}
)

export(
  TARGETS
    Fields2Cover
    tinyxml2
    steering_functions
    nlohmann_json
  FILE
    Fields2Cover-${type}-Targets.cmake
)

# Copy public headers during `make install`
install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE Fields2Cover)

# Packaging support
set(CPACK_PACKAGE_VENDOR "Wageningen University & Research")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PACKAGE_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR ${FIELDS2COVER_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${FIELDS2COVER_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${FIELDS2COVER_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.rst")
set(CPACK_PACKAGE_CONTACT "gonzalo.miermunoz@wur.nl")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Gonzalo Mier<${CPACK_PACKAGE_CONTACT}>")

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES .git dist .*build.* \\\\.DS_Store .dockerignore)
include(CPack)


#####################################################
#################### tutorials ######################
#####################################################

if (BUILD_TUTORIALS)
  add_subdirectory(tutorials)
endif(BUILD_TUTORIALS)

#####################################################
######################### swig ######################
#####################################################

if (BUILD_PYTHON)
  add_subdirectory(swig)
endif(BUILD_PYTHON)

#####################################################
######################### test ######################
#####################################################

if(BUILD_TESTS)
  find_package(GTest REQUIRED)
  include(CTest)
  enable_testing()
  add_custom_target(check COMMAND
    GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --verbose --test-dir tests/
  )
  add_subdirectory(tests)
endif(BUILD_TESTS)

#####################################################
######################### docs ######################
#####################################################


if(BUILD_DOC)
  add_subdirectory(docs)
endif(BUILD_DOC)
