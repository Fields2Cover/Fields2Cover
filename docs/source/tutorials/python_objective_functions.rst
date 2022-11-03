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

.. code-block:: python

    total_field = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
        f2c.Point(-2,-2), f2c.Point(6,-2), f2c.Point(6,6),
        f2c.Point(-2,6), f2c.Point(-2,-2)]))));
    field = f2c.Cells(f2c.Cell(f2c.LinearRing(f2c.VectorPoint([
        f2c.Point(0,0), f2c.Point(4,0), f2c.Point(4,4),
        f2c.Point(0,4), f2c.Point(0,0)]))));

    rem_area = f2c.OBJ_RemArea();
    print("The remaining area is ",
        rem_area.computeCost(total_field, field), ", and with sign is ",
        rem_area.computeCostWithMinimizingSign(total_field, field));

*The remaining area is  0.25 , and with sign is  -0.25*

SG objective functions
-----------------------

Field coverage
^^^^^^^^^^^^^^

Compute the percentage of the field covered by the swaths. The cost is a value between [0, 1], and it is defined as a maximization problem.

.. note::
   Right now, it only works with same width swaths.

.. code-block:: python

   double width {2.0};
   width = 2.0;
   swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint([
       f2c.Point(0.0, 1.0), f2c.Point(4.0, 1.0)])), width);
   swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint([
       f2c.Point(0.0, 3.0), f2c.Point(4.0, 3.0)])), width);
   swath3 = f2c.Swath(f2c.LineString(f2c.VectorPoint([
       f2c.Point(0.0, 2.0), f2c.Point(4.0, 2.0)])), width);

   swaths1 = f2c.Swaths();
   swaths1.push_back(swath1);
   swaths3 = f2c.Swaths();
   [swaths3.push_back(s) for s in [swath1, swath2, swath3]];

   field_cov = f2c.OBJ_FieldCoverage();
   print("The field coverage with swath1 is ",
       field_cov.computeCost(field, swaths1), " and with all of the swaths ",
       field_cov.computeCost(field, swaths3));


| *The field coverage with swath1 is 0.5 and with all of the swaths 1*

If you want to create an algorithm that reduce the objective function, use ``computeCostWithMinimizingSign()`` function instead, as it consider if it is a maximization or minimization problem:


.. code-block:: python

   print("The field coverage with sign for all of the swaths is ",
       field_cov.computeCostWithMinimizingSign(field, swaths3));

*The field coverage with sign for all of the swaths is -1*


Number of swaths
^^^^^^^^^^^^^^^^

Compute the number of swaths needed to cover the field. The idea is the slowest part of covering a field is turning between swaths. If the number of swaths are reduced, the number of turns too, and consequently, the time needed to cover the field.

.. code-block:: python

  n_swaths = f2c.OBJ_NSwath();

  print("The number of swaths with swath1 is ",
      n_swaths.computeCost(swaths1), " and with all of the swaths ",
      n_swaths.computeCost(field, swaths3));

*The number of swaths with swath1 is 1 and with all of the swaths 3*

.. note::
  As the number of swaths do not depend on the field, the parameter can be omitted.


Overlap
^^^^^^^

Compute percentage of the overlapping area in relation with the area of the field.

.. code-block:: python

   overlaps = f2c.OBJ_Overlaps();
   print("The field overlapping with swath1 is ",
       overlaps.computeCost(field, swaths1), " and with all of the swaths ",
       overlaps.computeCost(field, swaths3));

*The field overlapping with swath1 is 0 and with all of the swaths 0.5*


Swath Length
^^^^^^^^^^^^

Compute the sum of the path length of each swath.

.. code-block:: python

   swath_length = f2c.OBJ_SwathLength();
   print("The swath length with swath1 is "
       swath_length.computeCost(field, swaths1), " and with all of the swaths ",
       swath_length.computeCost(field, swaths3));

*The swath length with swath1 is 4 and with all of the swaths 12*

RP objective functions
------------------------

Distance with turns
^^^^^^^^^^^^^^^^^^^

Compute the complete distance of the path, including turns. This objective function actually computes each turn needed, so we will need to define the way to compute the turns.

.. code-block:: python
  :linenos:

  line1 = f2c.LineString(f2c.VectorPoint([f2c.Point(0.0, 0.0), f2c.Point(0.0, 1.0)]));
  swath1 = f2c.Swath(line1);
  line2 = f2c.LineString(f2c.VectorPoint([f2c.Point(1.0, 1.0), f2c.Point(1.0, 0.0)]));
  swath2 = f2c.Swath(line2);
  swaths_path = f2c.Swaths();
  swaths_path.push_back(swath1);
  swaths_path.push_back(swath2);
  robot = f2c.Robot(2.0, 3.0);
  robot.setMinRadius(0.5);
  complete_length = f2c.OBJ_CompleteTurnPathObj_Dubins(robot);
  print("The complete length is: ", complete_length.computeCost(swaths_path),
    " =~= ", 1 + 1 + math.pi/2.0);

*The complete length is: 3.57166 =~= 3.5708*

On line 7, we define the cost function with the class to compute the turns. In this case, ``f2c.PP_DubinsCurves``.

Direct distance without turns
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Compute an approximation of the distance of the path, replacing turns by straight lines.
This is faster than computing the turns and doesn't require to provide a class to compute the turns.

.. code-block:: python

  direct_dist = f2c.OBJ_DirectDistPathObj();
  print("The aproximated length is: ", direct_dist.computeCost(swaths_path));

*The aproximated length is: 3*



PP objective functions
-----------------------

Path length
^^^^^^^^^^^

.. code-block:: python

    path = f2c.Path()
    path.appendSwath(swaths_path.at(0), 1);
    path.appendSwath(swaths_path.at(1), 1);

    path_length = f2c.OBJ_PathLength();
    print("The path length is: ", path_length.computeCost(path));

*The path length is: 3*




