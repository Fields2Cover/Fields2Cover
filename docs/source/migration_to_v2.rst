.. _migration-guide:

Migration guide to version 2.0
==============================

F2C v2 has some breaking changes, which requires a little modifications to our code using F2C v1. Fortunately, this guide shows those changes so we can start using v2 as soon as possible.

New classes and new functionalities
-----------------------------------

To be able to add new functionalities to the library, some classes are added:

* ``f2c::types::Graph`` and ``f2c::types::Graph2D`` implement simple logic for graphs

* ``f2c::types::Route`` on version 2 has  :doc:`a new logic <tutorials/basic_types.rst>`.

* ``f2c::types::SwathsByCells`` is a class now, instead of just an alias to ``std::vector<Swaths>``.

* ``f2c::rp::RoutePlannerBase`` class does the route planning without given known patterns.

* ``f2c::decomp::DecompositionBase``, ``f2c::decomp::TrapezoidalDecomp`` and ``f2c::decomp::BoustrophedonDecomp`` provides functions to do cell decomposition.

* ``f2c::pp::PathPlanning`` is able to connect ``f2c::types::Route`` smoothly, providing proper coverage paths for any kind of fields.

* ``f2c::obj::NSwathModified`` is a new objective function. It's an approximation to ``f2c::obj::NSwath``, being able to compute faster the number of swaths.


Renaming functions
------------------

On v1, functions inherited from GDAL maintained the same name as in GDAL. Due to that, many functions didn't have a name following camel case convention. Here it is a list with a few examples:

* ``f2c::types::Geometry::Distance`` -> ``f2c::types::Geometry::distance``

* ``f2c::types::Geometry::Difference`` -> ``f2c::types::Geometry::difference``

* ``f2c::types::Geometry::Within`` -> ``f2c::types::Geometry::within``

* ``f2c::types::Geometry::Union`` -> ``f2c::types::Geometry::union``

* ``f2c::types::Swath::StartPoint`` -> ``f2c::types::Swath::startPoint``

* ``f2c::types::Swath::EndPoint`` -> ``f2c::types::Swath::endPoint``

Other functions had names that could be improved:

* ``f2c::types::Geometry::getArea()`` -> ``f2c::types::Geometry::area()``

* ``f2c::types::Geometry::getLength()`` -> ``f2c::types::Geometry::length()``

* ``f2c::types::Point::getSemiLongCurve()`` -> ``f2c::types::Point::createSemiLongLine()``

* ``f2c::pp::PathPlanning::searchBestPath`` -> ``f2c::pp::PathPlanning::planPath``


.. warning::
    The input parameters in function ``f2c::Random::generateRandField(int n_sides, double area)`` has been reorder to ``f2c::Random::generateRandField(double area, int n_sides)``.

    This change is made for keeping coherence with other similar functions, but can make our program crash if we are not careful.


Renaming files
--------------

This section is only refered to objective functions.
Previously, they were mixed in the same folder, independently of the module they belong to.

Now, they are sorted by subfolders.

For example, file ``fields2cover/objectives/n_swath.h`` is moved to ``fields2cover/objectives/sg_obj/n_swath.h``


Class attributes are private now
--------------------------------

On v1, you can access any attribute of any class because everything was public. Following good practices, access to attributes is done by getters and setters now, and all the attributes are private:

* ``f2c::types::Robot``

   * ``width`` -> ``getWidth()``/``setWidth()``

   * ``op_width`` -> ``getCovWidth()``/``setCovWidth()``

   * ``cruise_speed`` -> ``getCruiseVel()``/``setCruiseVel()``

   * ``max_icc`` -> ``getMaxCurv()``/``setMaxCurv()``

   * ``linear_curv_change`` -> ``getMaxDiffCurv()``/``setMaxDiffCurv()``

* ``f2c::types::Swath``

   * ``id`` -> ``getId()``/``setId()``

   * ``path`` -> ``getPath()``/``setPath()``

   * ``width`` -> ``getWidth()``/``setWidth()``


* ``f2c::types::Path``

   * ``states`` -> ``getState()``/``setState()``, ``getStates()``/``setStates()``

   * ``task_time`` -> ``getTaskTime()``

* ``f2c::types::Field``

   * ``id`` -> ``getId()``/``setId()``

   * ``coord_sys`` -> ``getCRS()``/``setCRS()``

   * ``field`` -> ``getField()``/``setField()``

   * ``ref_point`` -> ``getRefPoint()``/``setRefPoint()``


Visualizer
----------

The library used for plotting has been changed from `lava/matplotlib-cpp <https://github.com/lava/matplotlib-cpp>`_ to `alandefreitas/matplotplusplus <https://github.com/alandefreitas/matplotplusplus>`_. This means that the resulting plots will be different.

The main changes on the user side are:

* ``f2c::Visualizer::figure()`` does not accept arguments.

* ``f2c::Visualizer::save(std::string)`` requires the extension of the file on the name i.e. `.png`.











