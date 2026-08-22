Installation
------------

The Fields2Cover package has only been tested on Ubuntu.
If you are able to run it in other operative systems, open an issue/PR and it will be added to this guide


Requirements on Linux
^^^^^^^^^^^^^^^^^^^^^

Some packages are needed before compiling the package:

.. note::
  if your OS is Ubuntu 18.04 or 20.04, you would need to do ``sudo add-apt-repository ppa:ubuntugis/ppa``

.. code-block:: console

   sudo apt-get update
   sudo apt-get install --no-install-recommends software-properties-common
   sudo apt-get install --no-install-recommends build-essential ca-certificates cmake \
        doxygen g++ git libeigen3-dev libgdal-dev libpython3-dev python3 python3-pip \
        python3-matplotlib python3-tk lcov libgtest-dev libtbb-dev swig libgeos-dev \
        gnuplot libtinyxml2-dev nlohmann-json3-dev
   python3 -m pip install gcovr



Compilation
^^^^^^^^^^^

First, clone this repository.
Then, from the main folder of the project:

.. code-block:: console

   mkdir -p build;
   cd build;
   cmake ..;
   make -j$(nproc);
   sudo make install;


Add it to your projects
^^^^^^^^^^^^^^^^^^^^^^^

To add Fields2Cover into your CMakeLists.txt, it is as easy as:

.. code-block:: console

   find_package(Fields2Cover REQUIRED)
   target_link_libraries(<<<your_package>>> Fields2Cover)


Compilation with python interface
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As without the interface, clone this repository.
Then, from the main folder of the project:

With the system dependencies from the previous section installed (plus
``libgeos-dev``, ``libtbb-dev``, ``libtinyxml2-dev``, ``libeigen3-dev`` and
or-tools discoverable through ``CMAKE_PREFIX_PATH``), the python module can be
built and installed straight from the repository with pip:

.. code-block:: console

   pip install .

This compiles the library and the SWIG bindings and installs a
``fields2cover`` package into the active python environment. The manual cmake
route below is still available.

.. code-block:: console

   cd build;
   cmake -DBUILD_PYTHON=ON ..;
   make -j$(nproc);
   sudo make install;

To test if the compilation and installation of the python interface is correct, run on python:

.. code-block:: python

  import fields2cover






