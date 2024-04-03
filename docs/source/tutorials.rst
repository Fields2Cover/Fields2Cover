Tutorials
=========

On these tutorials, we will learn how to create a coverage path for a field.
Fields2Cover library is made by several modules.
Each of them solves an individual part of the Coverage Path Planning (CPP) problem.
Here, we will learn how to concatenate them to solve the CPP.

All the code examples shown in the tutorials are located on the ``tutorials`` folder.

Moreover, tutorials are compiled automatically with the project.
If you want to run the examples, go to your `build` folder and run:

.. code-block:: bash

    sudo chmod a+rwx tutorials/*
    ./tutorials/<name-of-the-tutorial>

Computed images will appear in the same folder as you are now.


.. toctree::
    :maxdepth: 3

    tutorials/basic_types.rst
    tutorials/objective_functions.rst
    tutorials/headland_generator.rst
    tutorials/swath_generator.rst
    tutorials/route_planning.rst
    tutorials/path_planning.rst
    tutorials/decomposition.rst
    tutorials/complete_flow.rst


