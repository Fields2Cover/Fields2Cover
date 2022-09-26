Fields2Cover
============




  **Robust and efficient coverage paths for autonomous agricultural vehicles**
  
|badge1| |badge2| |badge3| |license| 


.. image:: docs/figures/logo_fields2cover.jpeg
   :align: center


`The extended documentation is here <https://fields2cover.github.io/>`__.



The coverage path planning problem (CPP) aims to create a path to cover an area with one or several vehicles.
CPP has many application as cleaning robots, surveillance, de-mining or agriculture. 
Each of the application has its own requirements.
For example, in cleaning robots environment is usually unknown and the objective is improve the probability of cover the area.
On the other hand, CPP in agriculture knows the area to cover, but the coverage has to be complete.




Fields2Cover library provides several algorithms to plan the coverage path on convex fields:

.. image:: docs/figures/Tutorial_6_1_Dubins.png
   :align: center


Although the development of this project is focused on offline planning of agricultural vehicles, the library accepts pull requests from other types of coverage planners.


Why?
-----

There are many CPP papers out there, but almost no code about their implementation.
Due to this, research about this topic is slow as you need to implement every algorithm you want to compare with.

Fields2Cover provides a flexible structure and several algorithms to compare with, so any developer can research about this topic easily.


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
        python3-matplotlib lcov libgtest-dev libtbb-dev
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


Compilation with python interface
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As without the interface, clone this repository. 

Install the packages required as:

.. code-block:: console
   
   sudo apt install swig python3-pytest


Then, from the main folder of the project:

.. code-block:: console

   cd build;
   cmake -DBUILD_PYTHON=ON ..;
   make -j$(nproc);
   sudo make install;

To test if the compilation and installation of the python interface is correct, run on python:

.. code-block:: python

  import fields2cover
  
Or run on the main folder:

.. code-block:: console

  pytest-3 tests/python/


Stability
---------

Fields2Cover is still in early development, so feedback and bug reports are appreciated.  At the moment, it should expected that breaking changes may be introduced as new functionality is added, bugs are fixed, and APIs are streamlined.


Contribute
-----------

If you find any issue/bug/proposal, open an issue and we will try to solve/discuss it.

Pull requests are more than welcome. For major changes, please open an issue first to discuss what you would like to change. Please make sure to update tests as appropriate.


License
--------

Fields2Cover project is under `BSD-3 license <https://tldrlegal.com/license/bsd-3-clause-license-%28revised%29>`__.



TODO
-------

- Create SWIG interface with Lua/R
- Support Route planners with metaheuristics
- Support non-convex fields
- Consider the start and end point of the vehicle
- ...





Credits and more info
----------

This library is only possible thanks to `GDAL <https://gdal.org/index.html>`_ who provides the basic types of this library. 
Other great libraries that made Fields2Cover possible are `hbanzhaf/steering_functions <https://github.com/hbanzhaf/steering_functions>`_, `nlohmann/json <https://github.com/nlohmann/json/>`_, `leethomason/tinyxml2 <https://github.com/leethomason/tinyxml2>`_, `ttk592/spline <https://github.com/ttk592/spline>`_ and `lava/matplotlib-cpp <https://github.com/lava/matplotlib-cpp>`_



This code repository is part of the project Fields2Cover which is (partly) financed by the Dutch Research Council (NWO).


.. |badge1| image:: https://github.com/Fields2Cover/Fields2Cover/actions/workflows/build.yml/badge.svg
.. |badge2| image:: http://githubbadges.com/star.svg?user=fields2cover&repo=fields2cover&style=flat&color=fff&background=007ec6
.. |badge3| image:: http://githubbadges.com/fork.svg?user=fields2cover&repo=fields2cover&style=flat&color=fff&background=007ec6
.. |license| image:: https://img.shields.io/badge/license-BDS3-orange.svg
   :target: https://gitlab.com/Fields2Cover/Fields2Cover/blob/main/LICENSE



