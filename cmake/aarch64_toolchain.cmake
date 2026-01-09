set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Cross compiler
set(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)

# Search paths for arm64 libraries
set(CMAKE_FIND_ROOT_PATH /usr /usr/lib/aarch64-linux-gnu)
list(APPEND CMAKE_PREFIX_PATH /usr/lib/aarch64-linux-gnu/cmake)

# Search modes
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Python 3.12 paths for arm64 cross-compilation
set(Python_EXECUTABLE "/usr/bin/python3" CACHE FILEPATH "Host Python for SWIG" FORCE)
set(Python_INCLUDE_DIRS "/usr/include/python3.12" CACHE PATH "ARM Python Include" FORCE)
set(Python_LIBRARIES "/usr/lib/aarch64-linux-gnu/libpython3.12.so" CACHE FILEPATH "ARM Python Lib" FORCE)

set(PYTHON_LIBRARY "/usr/lib/aarch64-linux-gnu/libpython3.12.so" CACHE FILEPATH "ARM Python Lib" FORCE)
set(PYTHON_LIBRARIES "/usr/lib/aarch64-linux-gnu/libpython3.12.so" CACHE FILEPATH "ARM Python Libs" FORCE)
set(PYTHON_INCLUDE_DIR "/usr/include/python3.12" CACHE PATH "ARM Python Include" FORCE)
set(PYTHON_INCLUDE_DIRS "/usr/include/python3.12" CACHE PATH "ARM Python Includes" FORCE)

set(Python3_EXECUTABLE "/usr/bin/python3" CACHE FILEPATH "Host Python3" FORCE)
set(Python3_LIBRARY "/usr/lib/aarch64-linux-gnu/libpython3.12.so" CACHE FILEPATH "ARM Python3 Lib" FORCE)
set(Python3_LIBRARIES "/usr/lib/aarch64-linux-gnu/libpython3.12.so" CACHE FILEPATH "ARM Python3 Libs" FORCE)
set(Python3_INCLUDE_DIR "/usr/include/python3.12" CACHE PATH "ARM Python3 Include" FORCE)
set(Python3_INCLUDE_DIRS "/usr/include/python3.12" CACHE PATH "ARM Python3 Includes" FORCE)
