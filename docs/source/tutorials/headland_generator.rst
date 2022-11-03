Part 3: Headland generator
===========================

Agricultural vehicles need space to turn between swaths. This area is called a headland.
In Fields2Cover, the headland generator will take a ``F2CCells`` and the width of the headland as input, and it will return another ```F2CCells``` of the same field without the headlands.

Lets create a random field with ``5`` sides and ``1e4`` meters squared of area, and a robot with ``2`` meters of width.

.. code-block:: cpp

  f2c::Random rand(42);
  F2CField field = rand.generateRandField(5, 1e4);
  F2CCells cells = field.field;
  F2CRobot robot (2.0, 26.0);



Constant width headland
-------------------------------

This class creates a constant headland around the field.

.. code-block:: cpp

  f2c::hg::ConstHL const_hl;
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);
  std::cout << "The complete area is " << cells.getArea() <<
    ", and the area without headlands is " << no_hl.getArea() << std::endl;

*The complete area is 10000, and the area without headlands is 7676.63*


.. image:: ../../figures/Tutorial_3_1_Const_width.png


On orange it is ``cells`` and in green it is ``no_hl``
