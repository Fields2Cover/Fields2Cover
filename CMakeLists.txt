
set(FIELDS2COVER_MAJOR_VERSION "1")
set(FIELDS2COVER_MINOR_VERSION "1")
set(FIELDS2COVER_PATCH_VERSION "0")
set(FIELDS2COVER_VERSION_STRING "${FIELDS2COVER_MAJOR_VERSION}.${FIELDS2COVER_MINOR_VERSION}.${FIELDS2COVER_PATCH_VERSION}")
set(FIELDS2COVER_VERSION ${FIELDS2COVER_MAJOR_VERSION}.${FIELDS2COVER_MINOR_VERSION}.${FIELDS2COVER_PATCH_VERSION})

set(PACKAGE_NAME "Fields2Cover")
set(PACKAGE_DESCRIPTION "Fields2Cover is a complete coverage path planning package for autonomous robots")
set(PACKAGE_AUTHOR "Wageningen University")
set(PACKAGE_MAINTAINER "Gonzalo Mier")


option(BUILD_CPP "Build Cpp package" ON)
option(BUILD_TUTORIALS "Build tutorials" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_PYTHON "Build Python SWIG module" OFF)
option(BUILD_DOC "Build Documentation" OFF)



if (BUILD_PYTHON)
  cmake_minimum_required(VERSION 3.12.4)
else()
  cmake_minimum_required(VERSION 3.11)
endif()

if(${CMAKE_VERSION} VERSION_LESS 3.22)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.22)
endif()

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(Fields2Cover
  LANGUAGES CXX
  VERSION "${FIELDS2COVER_VERSION_STRING}"
  DESCRIPTION "${PACKAGE_DESCRIPTION}")

if(BUILD_CPP)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  # set(CMAKE_BUILD_TYPE Testing)
  # set(CMAKE_BUILD_TYPE Coverage)
endif()

if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

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
        endif()
    endmacro()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
FetchContent_Declare(json FETCHCONTENT_UPDATES_DISCONNECTED
  URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_Declare(tinyxml2 FETCHCONTENT_UPDATES_DISCONNECTED
  URL https://github.com/leethomason/tinyxml2/archive/refs/tags/9.0.0.tar.gz)
FetchContent_Declare(steering_functions FETCHCONTENT_UPDATES_DISCONNECTED
  GIT_REPOSITORY https://github.com/Fields2Cover-group/steering_functions.git)
FetchContent_Declare(spline FETCHCONTENT_UPDATES_DISCONNECTED
  GIT_REPOSITORY https://github.com/Fields2Cover-group/spline.git)
FetchContent_Declare(matplotlib_cpp FETCHCONTENT_UPDATES_DISCONNECTED
  GIT_REPOSITORY https://github.com/Fields2Cover-group/matplotlib-cpp.git)

FetchContent_MakeAvailable(json tinyxml2 steering_functions spline)
FetchContent_MakeAvailable(matplotlib_cpp)

find_package(GDAL 3.0 REQUIRED)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")


foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
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

add_library(Fields2Cover SHARED ${fields2cover_src})

target_compile_features(Fields2Cover PUBLIC cxx_std_17) 
target_compile_options(Fields2Cover PRIVATE -pedantic -Wall -Wfatal-errors
  -Werror -Wno-dev -Wextra -Wno-unused-parameter -Wno-sign-compare
  -Wno-attributes -Wl,--no-undefined)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
  include(cmake/Modules/CodeCoverage.cmake)
  target_compile_options(Fields2Cover PRIVATE -g -O0 --coverage -fprofile-arcs -ftest-coverage)
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
else ()
  target_compile_options(Fields2Cover PRIVATE -O2)
endif()



target_include_directories(Fields2Cover PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${EIGEN3_INCLUDE_DIRS}
)


target_link_libraries(Fields2Cover 
  PUBLIC
    ${GDAL_LIBRARIES}
    -lpthread
  PRIVATE 
    nlohmann_json::nlohmann_json
    tinyxml2::tinyxml2
    steering_functions
    spline
    matplotlib_cpp
)

set_target_properties(Fields2Cover PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE PUBLIC_HEADER "include/fields2cover.h")

install(TARGETS Fields2Cover
  EXPORT Fields2Cover-targets 
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
  PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(Fields2CoverConfigVersion.cmake
    VERSION ${PACKAGE_VERSION} COMPATIBILITY AnyNewerVersion)

install(EXPORT Fields2Cover-targets FILE Fields2CoverTargets.cmake
        DESTINATION ${INSTALL_CMAKE_DIR}/cmake/fields2cover)

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(cmake/Fields2CoverConfig.cmake.in
  "Fields2CoverConfig.cmake" @ONLY)
configure_file(cmake/Fields2CoverConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/Fields2CoverConfigVersion.cmake" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfigVersion.cmake"
        DESTINATION ${INSTALL_CMAKE_DIR}/cmake/fields2cover)

export(TARGETS Fields2Cover
  FILE Fields2CoverTargets.cmake)

# Copy public headers during `make install`
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES .git dist .*build.* \\\\.DS_Store .dockerignore)
include(CPack)


#####################################################
#################### tutorials ######################
#####################################################

if (BUILD_TUTORIALS)
    add_subdirectory(tutorials)
endif() 

#####################################################
######################### swig ######################
#####################################################

if (BUILD_PYTHON)
    add_subdirectory(swig)
endif() 

#####################################################
######################### test ######################
#####################################################

if (BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_custom_target(check COMMAND GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND}
      --verbose --test-dir tests/)
  add_subdirectory(tests)
endif()
endif()  # BUILD_CPP

#####################################################
######################### docs ######################
#####################################################


if(BUILD_DOC)
  add_subdirectory(docs)
endif()

