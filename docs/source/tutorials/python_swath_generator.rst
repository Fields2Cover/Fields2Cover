Part 4: Swath generator
===========================

Swath generators plan swaths on the field to be able to cover it completely.
Usually, they have a search method to find the best angle to cover the field.
This algorithms need to define a Global objective function to search the best angle.

For these examples, we will continue from the previous tutorial:

.. code-block:: python

  rand = f2c.Random(42);
  robot = f2c.Robot(2.0, 6.0);
  const_hl = f2c.HG_Const_gen();
  field = rand.generateRandField(5, 1e4);
  cells = field.field;
  no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);


Brute force
-------------------------------

Brute force algorithm tries all the possible angles (using a step_angle) and keeps the one that produces the smallest cost of the objective function.

Due to the objective function defines the best swaths, changing one objective function for another modifies the output of the algorithm.

This is the output if the global cost function is the number of swaths:

.. code-block:: python

  n_swath = f2c.OBJ_NSwath();
  bf_sw_gen = f2c.SG_BruteForce();
  swaths_bf_nswath = bf_sw_gen.generateBestSwaths(n_swath, robot.op_width, no_hl.getGeometry(0));

.. image:: ../../figures/Tutorial_4_1_Brute_force_NSwath.png


This is the output if the global cost function is the sum of the length of swaths:

.. code-block:: python

  swath_length = f2c.OBJ_SwathLength();
  swaths_bf_swathlength = bf_sw_gen.generateBestSwaths(swath_length, robot.op_width, no_hl.getGeometry(0));

.. image:: ../../figures/Tutorial_4_1_Brute_force_SwathLength.png

Of course, you can also create swaths with a custom angle:

.. code-block:: python

  swaths_bf_angle = bf_sw_gen.generateSwaths(math.pi, robot.op_width, no_hl.getGeometry(0));

.. image:: ../../figures/Tutorial_4_1_Brute_force_Angle.png

