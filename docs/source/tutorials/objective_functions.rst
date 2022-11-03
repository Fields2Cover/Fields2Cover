Part 2: Objective functions
===========================

One of the main problems in the Coverage Path Planning problem is to define the objective function.
The objective function defines how good a path is.
By default, the objective functions are defined as minimization problems.

Fields2Cover defines different objective functions for each module: HG (Headland Generator), SG (Swath Generator), RP (Route Planner) and PP (Path Planner).




HG objective functions
--------------------------

Remaining area
^^^^^^^^^^^^^^

Compute the percentage of main field over the total field. The cost is a value between [0, 1], and it is defined as a maximization problem.


.. code-block:: cpp

    F2CCells total_field(F2CCell(F2CLinearRing(
      {F2CPoint(-2,-2), F2CPoint(6,-2), F2CPoint(6,6), F2CPoint(-2,6), F2CPoint(-2,-2)})));
    F2CCells field(F2CCell(F2CLinearRing(
      {F2CPoint(0,0), F2CPoint(4,0), F2CPoint(4,4), F2CPoint(0,4), F2CPoint(0,0)})));


    f2c::obj::RemArea rem_area;
    std::cout << "The remaining area is "
      << rem_area.computeCost(total_field, field) << std::endl;
    std::cout << "The remaining area with sign is "
      << rem_area.computeCostWithMinimizingSign(total_field, field) <<std::endl;

|  *The remaining area is 0.25*
|  *The remaining area with sign is: -0.25*


SG objective functions
--------------------------

Field coverage
^^^^^^^^^^^^^^

Compute the percentage of the field covered by the swaths. The cost is a value between [0, 1], and it is defined as a maximization problem.

.. note::
   Right now, it only works with same width swaths.

.. code-block:: cpp

   double width {2.0};
   F2CSwath swath1(F2CLineString({F2CPoint(0.0, 1.0), F2CPoint(4.0, 1.0)}), width);
   F2CSwath swath2(F2CLineString({F2CPoint(0.0, 3.0), F2CPoint(4.0, 3.0)}), width);
   F2CSwath swath3(F2CLineString({F2CPoint(0.0, 2.0), F2CPoint(4.0, 2.0)}), width);

   f2c::obj::FieldCoverage field_cov;

   std::cout << "The field coverage with swath1 is "
     << field_cov.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
     << field_cov.computeCost(field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

| *The field coverage with swath1 is 0.5 and with all of the swaths 1*

If you want to create an algorithm that reduce the objective function, use ``computeCostWithMinimizingSign()`` function instead, as it consider if it is a maximization or minimization problem:


.. code-block:: cpp

   std::cout << "The field coverage with sign for all of the swaths is "
     << field_cov.computeCostWithMinimizingSign(
     field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

*The field coverage with sign for all of the swaths is -1*


Number of swaths
^^^^^^^^^^^^^^^^

Compute the number of swaths needed to cover the field. The idea is the slowest part of covering a field is turning between swaths. If the number of swaths are reduced, the number of turns too, and consequently, the time needed to cover the field.

.. code-block:: cpp

  f2c::obj::NSwath n_swaths;

  std::cout << "The number of swaths with swath1 is "
     << n_swaths.computeCost(F2CSwaths({swath1})) << " and with all of the swaths "
     << n_swaths.computeCost(field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

*The number of swaths with swath1 is 1 and with all of the swaths 3*

.. note::
  As the number of swaths do not depend on the field, the parameter can be omitted.


Overlap
^^^^^^^

Compute percentage of the overlapping area in relation with the area of the field.

.. code-block:: cpp

   f2c::obj::Overlaps overlaps;

   std::cout << "The field overlapping with swath1 is "
     << overlaps.computeCost(field, F2CSwaths({swath1})) << " and with all of the swaths "
     << overlaps.computeCost(field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

*The field overlapping with swath1 is 0 and with all of the swaths 0.5*


Swath Length
^^^^^^^^^^^^

Compute the sum of the path length of each swath.

.. code-block:: cpp

   f2c::obj::SwathLength swath_length;

   std::cout << "The swath length with swath1 is "
     << swath_length.computeCost(F2CSwaths({swath1})) << " and with all of the swaths "
     << swath_length.computeCost(field, F2CSwaths({swath1, swath2, swath3})) <<std::endl;

*The swath length with swath1 is 4 and with all of the swaths 12*


RP objective functions
------------------------

Distance with turns
^^^^^^^^^^^^^^^^^^^

Compute the complete distance of the path, including turns. This objective function actually computes each turn needed, so we will need to define the way to compute the turns.

.. code-block:: cpp
  :linenos:

  F2CSwaths swaths_path({
    F2CSwath(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0)})),
    F2CSwath(F2CLineString({F2CPoint(1.0, 1.0), F2CPoint(1.0, 0.0)}))});
  F2CRobot robot(3.0, 39.0);
  robot.setMinRadius(0.5);

  f2c::obj::CompleteTurnPathObj<f2c::pp::DubinsCurves> complete_length(robot);

  std::cout << "The complete length is: " << complete_length.computeCost(swaths_path) <<
    " =~= " << 1 + 1 + M_PI/2.0 << std::endl;

*The complete length is: 3.57166 =~= 3.5708*

On line 7, we define the cost function with the class to compute the turns. In this case, ``f2c::pp::DubinsCurves``.

Direct distance without turns
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Compute an approximation of the distance of the path, replacing turns by straight lines.
This is faster than computing the turns and doesn't require to provide a class to compute the turns.

.. code-block:: cpp

  f2c::obj::DirectDistPathObj direct_dist;

  std::cout << "The aproximated length is: " <<
    direct_dist.computeCost(swaths_path) << std::endl;

*The aproximated length is: 3*



PP objective functions
------------------------

Path length
^^^^^^^^^^^

Compute the length of the path


.. code-block:: cpp

  F2CPath path;
  path.appendSwath(swaths_path.at(0), 1);
  path.appendSwath(swaths_path.at(1), 1);

  f2c::obj::PathLength path_length;
  std::cout << "The path length is: " <<
    path_length.computeCost(path) << std::endl;

*The path length is: 3*

