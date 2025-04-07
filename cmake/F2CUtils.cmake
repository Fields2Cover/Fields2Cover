function(f2c_declare_dependencies)

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


  set(ORTOOLS_TARGET "")
  if(USE_ORTOOLS_FETCH_SRC)
    message(STATUS "or-tools -- Downloading and building from source")
    FetchContent_Declare(ortools
      GIT_REPOSITORY https://github.com/google/or-tools.git
      GIT_TAG v9.9
    )
    FetchContent_MakeAvailable(ortools)
    set(ORTOOLS_TARGET "ortools")
  elseif(USE_ORTOOLS_VENDOR)
    find_package(ortools_vendor REQUIRED)
  else()
    find_package(ortools_vendor QUIET)
    find_package(ortools CONFIG QUIET)
    if(NOT ortools_FOUND)
      message(STATUS "or-tools -- Downloading and installing from release tarball")
      if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
        message(STATUS "Target architecture is AMD64")
        FetchContent_Declare(ortools FETCHCONTENT_UPDATES_DISCONNECTED
          URL https://github.com/google/or-tools/releases/download/v9.9/or-tools_amd64_ubuntu-22.04_cpp_v9.9.3963.tar.gz
          URL_HASH SHA256=a611133f4e9b75661c637347ebadff79539807cf8966eb9c176c2c560aad0a84
        )
      elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        message(STATUS "Target architecture is ARM64")
        FetchContent_Declare(ortools FETCHCONTENT_UPDATES_DISCONNECTED
          URL https://github.com/google/or-tools/releases/download/v9.9/or-tools_arm64_debian-11_cpp_v9.9.3963.tar.gz
          URL_HASH SHA256=f308a06d89dce060f74e6fec4936b43f4bdf4874d18c131798697756200f4e7a
        )
      else()
        message(FATAL_ERROR "Unknown/Unhandled target architecture: ${CMAKE_SYSTEM_PROCESSOR}")
      endif()
      #NOTE: FetchContent_GetProperties variables only available in called scope
      FetchContent_GetProperties(ortools)
      if(NOT ortools_POPULATED)
        FetchContent_Populate(ortools)
      endif()
      list(INSERT CMAKE_PREFIX_PATH 0 "${ortools_SOURCE_DIR}")
      find_package(ortools CONFIG REQUIRED)
      if(NOT ortools_FOUND)
        message(FATAL_ERROR "Failed to find ortools in release tarball")
      endif()

      #NOTE: in CMAKE 3.21 introduces IMPORTED_RUNTIME_ARTIFACTS
      # https://cmake.org/cmake/help/v3.21/command/install.html#imported-runtime-artifacts
      # Which would allow creation of an ortools target with IMPORTED_LOCATION
      # and then install with IMPORTED_RUNTIME_ARTIFACTS
      # Just brute force install the contents of the directories for now.
      include(GNUInstallDirs)
      install(
        DIRECTORY "${ortools_SOURCE_DIR}/"
        DESTINATION ${CMAKE_INSTALL_PREFIX} #opt/f2c_ortools/
      )
    endif(NOT ortools_FOUND)
  endif()

  FetchContent_Declare(steering_functions FETCHCONTENT_UPDATES_DISCONNECTED
    GIT_REPOSITORY https://github.com/Fields2Cover/steering_functions.git
    GIT_TAG 13e3f5658144b3832fb1eb31a0e2f5a3cbf57db9
  )
  FetchContent_Declare(matplot FETCHCONTENT_UPDATES_DISCONNECTED
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus.git
    GIT_TAG 5d01eb3695b07634a2b6642fd423740dea9b026c
  )

  FetchContent_Declare(json FETCHCONTENT_UPDATES_DISCONNECTED
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG 4424a0fcc1c7fa640b5c87d26776d99150dacd10
  )

  FetchContent_MakeAvailable(steering_functions matplot json)
endfunction()
