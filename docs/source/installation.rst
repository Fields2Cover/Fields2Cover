Installation
------------

The Fields2Cover package has only been tested on Ubuntu and on macOS (Apple Silicon).
If you are able to run it in other operative systems, open an issue/PR and it will be added to this guide


System requirements
^^^^^^^^^^^^^^^^^^^

Some packages are needed before compiling the package:

.. tabs:: os

    .. tab:: Linux

        .. note::
          if your OS is Ubuntu 18.04 or 20.04, you would need to do ``sudo add-apt-repository ppa:ubuntugis/ppa``

        .. code-block:: console

           sudo apt-get update
           sudo apt-get install --no-install-recommends software-properties-common
           sudo apt-get install --no-install-recommends build-essential ca-certificates cmake \
                doxygen g++ git libboost-dev libeigen3-dev libgdal-dev libpython3-dev python3 python3-pip \
                python3-matplotlib python3-tk lcov libgtest-dev libtbb-dev swig libgeos-dev \
                gnuplot libtinyxml2-dev nlohmann-json3-dev
           python3 -m pip install gcovr

        Also, `OR-tools <https://developers.google.com/optimization>`__ for C++ is needed.
        Follow its installation process.

    .. tab:: macOS

        .. code-block:: console

           brew install cmake swig gdal geos or-tools tinyxml2 eigen tbb boost gnuplot googletest


C++
^^^

Clone this repository. Then, from the main folder of the project:

.. tabs:: os

    .. tab:: Linux

        .. code-block:: console

           mkdir -p build;
           cd build;
           cmake ..;
           make -j$(nproc);
           sudo make install;

    .. tab:: macOS

        .. code-block:: console

           mkdir -p build;
           cd build;
           cmake -DCMAKE_PREFIX_PATH="$(brew --prefix)" -DPython_EXECUTABLE="$(which python3)" ..;
           make -j$(sysctl -n hw.ncpu);
           sudo make install;

To add Fields2Cover into your ``CMakeLists.txt``, it is as easy as:

.. code-block:: cmake

   find_package(Fields2Cover REQUIRED)
   target_link_libraries(<your_package> Fields2Cover)


Python
^^^^^^

The python module needs CMake >= 3.18 and Python >= 3.9, and the system
requirements above installed — `OR-tools
<https://developers.google.com/optimization>`__ must be discoverable through
``CMAKE_PREFIX_PATH``. Either way the package is compiled on your machine,
including the SWIG bindings; SWIG, CMake and Ninja are fetched into the
isolated build environment, so no system SWIG is needed.

.. tabs:: pyinstall

    .. tab:: From PyPI

        Released versions are on `PyPI <https://pypi.org/project/fields2cover/>`__:

        .. code-block:: console

           pip install fields2cover

        On macOS:

        .. code-block:: console

           CMAKE_PREFIX_PATH="$(brew --prefix)" pip install fields2cover

    .. tab:: From source

        Clone this repository. Then, from the main folder of the project:

        .. code-block:: console

           pip install .

        On macOS:

        .. code-block:: console

           CMAKE_PREFIX_PATH="$(brew --prefix)" pip install .

        Alternatively, use the manual cmake route, adjusting the
        ``BUILD_PYTHON`` option of the existing build:

        .. code-block:: console

           cd build;
           cmake -DBUILD_PYTHON=ON ..;
           make -j$(nproc);
           sudo make install;

To test if the compilation and installation of the python interface is correct, run:

.. code-block:: console

   python3 -c "import fields2cover as f2c; print(f2c.__version__)"

Or run the tests on the main folder:

.. code-block:: console

   python3 -m pytest tests/python/
