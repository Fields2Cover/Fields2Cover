Part 5: Route planning
=========================

.. |boustrophedon1| image:: ../../figures/Tutorial_5_1_Boustrophedon_1.png
    :scale: 60%
.. |boustrophedon2| image:: ../../figures/Tutorial_5_1_Boustrophedon_2.png
    :scale: 60%
.. |boustrophedon3| image:: ../../figures/Tutorial_5_1_Boustrophedon_3.png
    :scale: 60%
.. |boustrophedon4| image:: ../../figures/Tutorial_5_1_Boustrophedon_4.png
    :scale: 60%
.. |snake1| image:: ../../figures/Tutorial_5_2_Snake_1.png
    :scale: 60%
.. |snake2| image:: ../../figures/Tutorial_5_2_Snake_2.png
    :scale: 60%
.. |snake3| image:: ../../figures/Tutorial_5_2_Snake_3.png
    :scale: 60%
.. |snake4| image:: ../../figures/Tutorial_5_2_Snake_4.png
    :scale: 60%
.. |spiral1| image:: ../../figures/Tutorial_5_3_Spiral_1.png
    :scale: 60%
.. |spiral2| image:: ../../figures/Tutorial_5_3_Spiral_2.png
    :scale: 60%
.. |spiral3| image:: ../../figures/Tutorial_5_3_Spiral_3.png
    :scale: 60%
.. |spiral4| image:: ../../figures/Tutorial_5_3_Spiral_4.png
    :scale: 60%


A route planner searches the best order to cover previously generated swaths.
Usually, metaheuristics are used in the search.

.. note::
  In this version of Fields2Cover, no metaheuristic is provided, only predefined patterns.
  Route patterns do not return ``F2CRoute``, but ``F2CSwaths`` instead.
  In the next version, route planners will support metaheuristics, ``F2CCells`` as inputs, and ``F2CRoute`` as output.


For these examples, we will continue from the previous tutorial:

.. code-block:: cpp

  f2c::Random rand(42);
  F2CRobot robot (2.0, 6.0);
  f2c::hg::ConstHL const_hl;
  F2CCells cells = rand.generateRandField(5, 1e4).field;
  F2CCells no_hl = const_hl.generateHeadlands(cells, 3.0 * robot.robot_width);
  f2c::sg::BruteForce bf;
  F2CSwaths swaths = bf.generateSwaths(M_PI, robot.op_width, no_hl.getGeometry(0));

Once planned the swaths, it would be awesome to find the best order to cover the field.
To plot the order, we have used green for earlier covered swaths and black for last covered.
The direction of swaths is also green dot to black cross.

.. image:: ../../figures/Tutorial_4_1_Brute_force_Angle.png


Boustrophedon order
-------------------------------

Boustrophedon pattern is one of the most known patterns to cover a field.
Swaths are traveled in the simplest order, covering first the first swath, then the second, and so on.
This pattern can have 4 results on the same field, depending on the start point.

With the next code, swaths are order as the first image.
Calling again ``genSortedSwaths`` produces the other variants.
Once it has been called 4 times, the loop starts over.

.. code-block:: cpp

   f2c::rp::BoustrophedonOrder boustrophedon_sorter;
   boustrophedon_swaths = boustrophedon_sorter.genSortedSwaths(swaths);


+------------------+------------------+
| |boustrophedon1| | |boustrophedon2| |
+------------------+------------------+
| |boustrophedon3| | |boustrophedon4| |
+------------------+------------------+

Snake order
-------------------------------

Snake order covers the field skipping one swath each turn, and then coming back using uncovered swaths. This pattern, compared to boustrophedon, reduces the number of sharp turns.

As with boustrophedon pattern, snake pattern also has 4 variants:

.. code-block:: cpp

  f2c::rp::SnakeOrder snake_sorter;
  snake_swaths = snake_sorter.genSortedSwaths(swaths);

+----------+----------+
| |snake1| | |snake2| |
+----------+----------+
| |snake3| | |snake4| |
+----------+----------+

Spiral order
-------------------------------

Spiral order covers the field in multiple spirals with predefined size.
This pattern is commonly used when harvesting.
Harvesters have a limited capacity and sometimes have to unload onto a truck.

Because the side to which they can unload is usually fixed (either left, or right), it is best to minimize the number of occasions,
of the truck having to drive into the unharvested part of the field, or the harvester making way for the truck and waiting until it is unloaded.

With this order, there is always only one swath in entire spiral, where this event can occur.

The higher the spiral size, the lower the chance of having to unload onto an unharvested path of the field,
at the price of longer distance travelled between the swaths.

With the spiral size of 6, the order of swaths travelled is:
1, 6, 2, 5, 3, 4, 7, 12, 8, 11 and so on...

Same as previous patterns, spiral pattern also has 4 variants:

.. code-block:: cpp

  f2c::rp::SpiralOrder spiral_sorter(6);
  spiral_swaths = spiral_sorter.genSortedSwaths(swaths);

+-----------+-----------+
| |spiral1| | |spiral2| |
+-----------+-----------+
| |spiral3| | |spiral4| |
+-----------+-----------+


Custom order
-------------------------------

To support more general approach for coverage path planning it's possible to define
custom order of the swaths for the path planning process.

.. code-block:: cpp

  f2c::rp::CustomOrder custom_order({0, 1, 2, 3, 4});
  custom_swaths = custom_order.genSortedSwaths(swaths);

.. code-block:: cpp

    f2c::rp::CustomOrder custom_order;
    custom_order = custom_order.setCustomOrder({0, 1 , 2, 3, 4})
    custom_swaths2 = custom_order.genSortedSwaths(swaths);


.. note::
    There are several checks whether the customer order can be used or not.

    - The custom order may not contain any elements more than once
    - The supplied list/vector length must be the same as the number of the swaths
    - The order vector may contain only elements from the swath range: `<0, swaths.size() - 1>`
