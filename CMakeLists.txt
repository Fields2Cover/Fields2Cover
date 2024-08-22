
cmake_minimum_required(VERSION 3.12.4...3.22)

project(Fields2Cover
  VERSION 2.0.0
  DESCRIPTION "Fields2Cover is a complete coverage path planning package for autonomous robots"
  HOMEPAGE_URL "http://fields2cover.github.io"
  LANGUAGES CXX
)



option(ALLOW_PARALLELIZATION "Allow parallel algorithms" ON)
option(BUILD_TUTORIALS "Build tutorials" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_PYTHON "Build Python SWIG module" ON)
option(BUILD_DOC "Build Documentation" OFF)
option(BUILD_SHARED_LIBS "Build shared library(.so)" ON)
option(USE_ORTOOLS_RELEASE "Get or-tools from release tarball" OFF)
option(USE_ORTOOLS_FETCH_SRC "Get or-tools from source" OFF)
option(USE_ORTOOLS_VENDOR "Get or-tools from ortools_vendor" OFF)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



if(POLICY CMP0094)
  cmake_policy(SET CMP0094 NEW)
endif(POLICY CMP0094)
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif(POLICY CMP0135)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  # set(CMAKE_BUILD_TYPE Testing)
  # set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)


list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")


# Dependencies
find_package(TinyXML2 REQUIRED)
find_package(Threads REQUIRED)
find_package(Gnuplot)
find_package(GDAL 3.0 REQUIRED)
find_package(Eigen3 REQUIRED)
find_library(MATH_LIBRARY m)


set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)


include(cmake/F2CUtils.cmake)
f2c_declare_dependencies()


file(GLOB_RECURSE fields2cover_src
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(Fields2Cover ${fields2cover_src})


target_include_directories(Fields2Cover
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Fields2Cover
  PUBLIC
    GDAL::GDAL
    ${CMAKE_THREAD_LIBS_INIT}
    -lgeos_c
    ${MATH_LIBRARY}
  PRIVATE
    ortools::ortools
    nlohmann_json::nlohmann_json
    tinyxml2::tinyxml2
    steering_functions
    matplot
)

if(ALLOW_PARALLELIZATION)
  target_link_libraries(Fields2Cover PRIVATE tbb)
endif(ALLOW_PARALLELIZATION)




# Compiler options
include(cmake/F2CCompilerOptions.cmake)
f2c_set_compiler_options(Fields2Cover)


include(GNUInstallDirs)
set(F2C_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Fields2Cover)

install(TARGETS Fields2Cover steering_functions matplot
  EXPORT Fields2CoverTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Fields2CoverConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfig.cmake
  INSTALL_DESTINATION ${F2C_INSTALL_CMAKEDIR}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/Fields2CoverConfigVersion.cmake
  DESTINATION ${F2C_INSTALL_CMAKEDIR}
)

install(EXPORT Fields2CoverTargets
  FILE Fields2CoverTargets.cmake
  NAMESPACE Fields2Cover::
  DESTINATION ${F2C_INSTALL_CMAKEDIR}
)


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

if(BUILD_TESTS AND GNUPLOT_FOUND)
  find_package(GTest REQUIRED)
  include(CTest)
  enable_testing()
  add_custom_target(check COMMAND
    GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --verbose --test-dir tests/
  )
  add_subdirectory(tests)
endif(BUILD_TESTS AND GNUPLOT_FOUND)

#####################################################
######################### docs ######################
#####################################################


if(BUILD_DOC)
  add_subdirectory(docs)
endif(BUILD_DOC)

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

