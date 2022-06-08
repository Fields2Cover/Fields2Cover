Fields2Cover
============

::
  is the first coverage path planning library for UGVs in agriculture.


The coverage path planning problem (CPP) aims to create a path to cover an area with one or several vehicles.
CPP has many application as cleaning robots, surveillance, de-mining or agriculture. 
Each of the application has its own requirements.
For example, in cleaning robots environment is usually unknown and the objective is improve the probability of cover the area.
On the other hand, CPP in agriculture knows the area to cover, but the coverage has to be complete.

.. note::
    Although the development of this project is focused on offline planning of agricultural vehicles, the library accepts pull requests from other types of coverage planners.


Fields2Cover library offers several algorithms to plan the coverage path on convex fields:

.. image:: docs/figures/Tutorial_6_1_Dubins.png


Why?
-----

There are many CPP papers out there, but almost no code about their implementation.
Due to this, research about this topic is slow as you need to implement every algorithm you want to compare with.

Fields2Cover provides a flexible structure and several algorithms to compare with, so any developer can research about this topic easily.


.. include:: docs/source/installation.rst



Contribute
-----------

If you find any issue/bug/proposal, open an issue and we will try to solve/discuss it.

Pull requests are more than welcome. For major changes, please open an issue first to discuss what you would like to change. Please make sure to update tests as appropriate.


License
--------

Fields2Cover project is under `BSD-3 license <https://tldrlegal.com/license/bsd-3-clause-license-%28revised%29>`__.


More info
----------

This code repository is part of the project Fields2Cover which is (partly) financed by the Dutch Research Council (NWO).


