Installation
------------

The Fields2Cover package has only been tested on Ubuntu 20.04.
If you are able to run it in other operative systems, open an issue/PR and it will be added to this guide


Requirements on Linux
^^^^^^^^^^^^^^^^^^^^^^

Some packages are needed before compiling the package:

.. code-block:: console

   sudo add-apt-repository -y ppa:ubuntugis/ppa
   sudo apt-get -y update
   sudo apt-get install -y --no-install-recommends build-essential ca-certificates cmake \
        doxygen g++ git libeigen3-dev libgdal-dev libpython3-dev python3 python3-pip \
        python3-matplotlib lcov libgtest-dev 
   python -m pip install gcovr


Compilation
^^^^^^^^^^^^

First, clone this repository. 
Then, from the main folder of the project:

.. code-block:: console

   cd build;
   cmake -DCMAKE_BUILD_TYPE=Release ..;
   make -j$(nproc);
   sudo make install;


Add it to your projects
^^^^^^^^^^^^^^^^^^^^^^^^

To add Fields2Cover into your CMakeLists.txt, it is as easy as:

.. code-block:: console
   
   find_package(Fields2Cover REQUIRED)
   target_link_libraries(<<<your_package>>> Fields2Cover)




