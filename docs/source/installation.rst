Installation
------------

The Fields2Cover package has only been tested on Ubuntu 18.04, 20.04 and 22.04.
If you are able to run it in other operative systems, open an issue/PR and it will be added to this guide


Requirements on Linux
^^^^^^^^^^^^^^^^^^^^^^

Some packages are needed before compiling the package:

.. note::
  if your OS is Ubuntu 18.04 or 20.04, you would need to do ``sudo add-apt-repository ppa:ubuntugis/ppa``

.. code-block:: console

   sudo apt-get update
   sudo apt-get install --no-install-recommends software-properties-common
   sudo apt-get install --no-install-recommends build-essential ca-certificates cmake \
        doxygen g++ git libeigen3-dev libgdal-dev libpython3-dev python3 python3-pip \
        python3-matplotlib python3-tk lcov libgtest-dev libtbb-dev swig libgeos-dev
   python3 -m pip install gcovr


Compilation
^^^^^^^^^^^^

First, clone this repository.
Then, from the main folder of the project:

.. code-block:: console

   mkdir -p build;
   cd build;
   cmake ..;
   make -j$(nproc);
   sudo make install;


Add it to your projects
^^^^^^^^^^^^^^^^^^^^^^^^

To add Fields2Cover into your CMakeLists.txt, it is as easy as:

.. code-block:: console

   find_package(Fields2Cover REQUIRED)
   target_link_libraries(<<<your_package>>> Fields2Cover)


Compilation with python interface
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As without the interface, clone this repository.
Then, from the main folder of the project:

.. code-block:: console

   cd build;
   cmake -DBUILD_PYTHON=ON ..;
   make -j$(nproc);
   sudo make install;

To test if the compilation and installation of the python interface is correct, run on python:

.. code-block:: python

  import fields2cover



ROS Compatibility
^^^^^^^^^^^^^^^^^

We also provide a `ROS 1 and ROS 2 interface <https://github.com/Fields2Cover/fields2cover_ros>`__. On it, you can have a nice visualization of your field and the paths generated:

.. image:: ../figures/demo_image.png
  :align: center
  :width: 1000px

For being able to use the package from ROS, from `catkin_ws/`:

.. code-block:: console

  git clone https://github.com/Fields2Cover/Fields2Cover src/fields2cover
  git clone https://github.com/Fields2Cover/fields2cover_ros src/fields2cover_ros
  rosdep install -r --ignore-src --from-paths .
  # If using ROS 1:
  catkin_make_isolated
  # If using ROS 2:
  colcon build




