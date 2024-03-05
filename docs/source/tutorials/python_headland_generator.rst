Part 3: Headland generator
===========================

Agricultural vehicles need space to turn between swaths. This area is called a headland.
In Fields2Cover, the headland generator will take a ``Cells`` and the width of the headland as input, and it will return another ```F2CCells``` of the same field without the headlands.

Lets create a random field with ``5`` sides and ``1e4`` meters squared of area, and a robot with ``2`` meters of width.

.. code-block:: python

  rand = f2c.Random(42);
  field = rand.generateRandField(1e4, 5);
  cells = field.getField();
  robot = f2c.Robot(2.0, 26.0);


Constant width headland
-------------------------------

This class creates a constant headland around the field.

.. code-block:: python

  const_hl = f2c.HG_Const_gen();
  no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.getWidth());
  print("The complete area is ", cells.area(),
      ", and the area without headlands is ", no_hl.area());

*The complete area is 10000, and the area without headlands is 7676.63*


.. image:: ../../figures/Tutorial_3_1_Const_width.png


On orange it is ``cells`` and in green it is ``no_hl``
