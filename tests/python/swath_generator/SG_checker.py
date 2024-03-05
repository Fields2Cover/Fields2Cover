#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def IsSwathGenerationCorrect(sw_gen, obj, poly, swath_width = 1.0, checks = 100):
  swaths = sw_gen.generateBestSwaths(obj, swath_width, poly);
  assert not (swaths.size() < 1), "Error 2001: No swaths were generated.";
  assert not (swaths.size() > poly.area() / (swath_width * swath_width)),       \
      "Error 2002: " + str(swaths.size()) + " swaths were generated." +            \
      "Expected less than " + str(poly.area() / (swath_width * swath_width)) +  \
      "swaths.";

  rand = f2c.Random();
  for i in range(checks):
    rand_ang = rand.getAngleRandom();
    swaths2 = sw_gen.generateSwaths(rand_ang, swath_width, poly);
    assert (obj.computeCostWithMinimizingSign(swaths) <=                    \
            obj.computeCostWithMinimizingSign(swaths2)),                     \
            "Error 2003: Cost (signed) of the optimal angle is " +                              \
            str(obj.computeCostWithMinimizingSign(swaths)) +                            \
            ", but the cost (signed) of a non-optimal angle (" +                \
            str(rand_ang) + ") is " +                                                \
            str(obj.computeCostWithMinimizingSign(swaths2)) + ".";
    assert not (swaths.size() > poly.area() / (swath_width * swath_width)), \
        "Error 2004: " + str(swaths2.size()) +                                      \
        " swaths were generated with angle " + str(rand_ang) +                      \
        "Expected less than " + str(poly.area() / (swath_width * swath_width)) + \
        "swaths.";

