Part 6: Path planning
=======================

Lastly, time to create the final path. A good path planner will do this task.

For these examples, we will continue from the previous tutorial:

.. code-block:: cpp

  f2c::Random rand(42);
  F2CRobot robot (2.0, 6.0);
  f2c::hg::ConstHL const_hl;
  F2CCells cells = rand.generateRandField(5, 1e4).field;
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);
  f2c::sg::BruteForce bf;
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.op_width, no_hl.getGeometry(0));
  f2c::rp::SnakeOrder snake_sorter;
  swaths = snake_sorter.genSortedSwaths(swaths);

Before continue, we will define the path planner and some parameters that will be needed for computing the turns:

.. code-block:: cpp

  f2c::pp::PathPlanning path_planner;
  robot.setMinRadius(2);  // m
  robot.linear_curv_change = 0.1;  // 1/m^2


Dubins curves
-------------------------------

Dubins curves are generated with 3 segments of turn. Segments of turn in Dubins are always going forward.
Segment types are straight, right curve or left curve.

Dubins produces the shortest turn possible.


.. code-block:: cpp

   f2c::pp::DubinsCurves dubins;
   F2CPath path_dubins = path_planner.searchBestPath(robot, swaths, dubins);


.. image:: ../../figures/Tutorial_6_1_Dubins.png

Dubins curves with Continuous curvature
------------------------------------------

While Dubins curves produces the shortest path possible, going from one segment of the curve to another creates instant curvature change.
A vehicle could not follow a path with this issue, so this path planner implements an integrator to change the curvature smoothly.

.. code-block:: cpp

   f2c::pp::DubinsCurvesCC dubins_cc;
   F2CPath path_dubins_cc = path_planner.searchBestPath(robot, swaths, dubins_cc);

.. image:: ../../figures/Tutorial_6_2_Dubins_CC.png


Reeds-Shepp curves
-------------------------------

Reeds-Shepp curves also computes the shortest path, but allowing the vehicle to go backwards.

.. code-block:: cpp

   f2c::pp::ReedsSheppCurves reeds_shepp;
   F2CPath path_reeds_shepp = path_planner.searchBestPath(robot, swaths, reeds_shepp);


.. image:: ../../figures/Tutorial_6_3_Reeds_Shepp.png


Reeds-Shepp curves with Continuous curvature
------------------------------------------------

As with Dubins, instantaneous curvature change is a problem in this kind of curves.
This planner has an integrator to smooth the path.

.. code-block:: cpp

   f2c::pp::ReedsSheppCurvesHC reeds_shepp_hc;
   F2CPath path_reeds_shepp_hc = path_planner.searchBestPath(robot, swaths, reeds_shepp_hc);

.. image:: ../../figures/Tutorial_6_4_Reeds_Shepp_HC.png
