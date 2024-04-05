============
Fields2Cover
============

  **Robust and efficient coverage paths for autonomous agricultural vehicles**

.. image:: ./figures/logo_fields2cover.jpeg
    :align: center


`The github repository is here <https://github.com/Fields2Cover/Fields2Cover>`__.


The Coverage Path Planning problem (CPP) aims to create a path to cover an area with one or several vehicles.


.. image:: ./figures/diagram-F2C.jpg
   :align: center
   :width: 500px


Although the development of this project is focused on offline planning of agricultural vehicles,
the library accepts pull requests from other types of coverage planners.


F2C version 2.0
===============

The version 2.0 of Fields2Cover comes with support for non-convex fields and fields with obstacles!! This has been huge request from the community and finally it's here.

A quick list of new additions to the library:

#. :ref:`Decomposition algorithms<tutorial-decomp>`:

   * Trapezoidal and Boustrophedon decomposition. Those algorithms are useful to split any concave field into several convex sub-fields.

#. :ref:`Route planner<tutorial-route>`:

   * Route optimizer using `OR-tools <https://developers.google.com/optimization>`__ can be used to order the swaths, instead of just using a known pattern.

   * Support for a point used as the start and end points of the route.

#. Path planner:

   * Support for routes provided by the route optimizer.

#. Swath generator:

   * Speed improvement on swath generation.

   * New cost function: ``NSwathModified``. It uses an approximation to compute the number of swaths, reducing computation costs.

#. Many bugs fixed.

For a little guide about the migration, please check: :ref:`migration-guide`.


Citing
======

Please cite `the following paper <https://ieeexplore.ieee.org/document/10050562>`__ when using Fields2Cover for your research:

.. code-block:: bibtex

  @article{Mier_Fields2Cover_An_open-source_2023,
    author={Mier, Gonzalo and Valente, João and de Bruin, Sytze},
    journal={IEEE Robotics and Automation Letters},
    title={Fields2Cover: An Open-Source Coverage Path Planning Library for Unmanned Agricultural Vehicles},
    year={2023},
    volume={8},
    number={4},
    pages={2166-2172},
    doi={10.1109/LRA.2023.3248439}
  }



More information
================

.. toctree::
   :maxdepth: 2

   self
   source/installation.rst
   source/tutorials.rst
   source/migration_to_v2.rst
   source/faq.rst
   api/f2c_library.rst



Contribute
==========

If you find any issue/bug/proposal, open an issue and we will try to solve/discuss it.

Pull requests are more than welcome. For major changes, please open an issue first to discuss what you would like to change. Please make sure to update tests as appropriate.


License
=======

Fields2Cover project is under `BSD-3 license <https://tldrlegal.com/license/bsd-3-clause-license-%28revised%29>`__.






