#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def IsSwathGenerationCorrect(sw_gen, poly, swath_width = 1.0, checks = 100):
  swaths = sw_gen.generateBestSwaths(swath_width, poly);
  assert not (sw_gen.getBestAngle() == -1e7), "Error 2000: Best angle not set.";
  assert not (swaths.size() < 1), "Error 2001: No swaths were generated.";
  assert not (swaths.size() > poly.getArea() / (swath_width * swath_width)),  \
      "Error 2002: " + swaths.size() + " swaths were generated." +            \
      "Expected less than " + poly.getArea() / (swath_width * swath_width) +  \
      "swaths.";

  rand = f2c.Random();
  for i in range(checks):
    rand_ang = rand.getAngleRandom();
    swaths2 = sw_gen.generateSwaths(rand_ang, swath_width, poly);
    sign = 0.0;
    assert not (sw_gen.computeCostWithMinimizingSign(swaths) >                  \
            sw_gen.computeCostWithMinimizingSign(swaths2)),                     \
            "Error 2003: Cost (signed) of the optimal angle (" +                \
            sw_gen.getBestAngle() + ") is " +                                   \
            sign * sw_gen.computeCost(swaths) +                                 \
            ", but the cost (signed) of a non-optimal angle (" +                \
            rand_ang + ") is " +                                                \
            sw_gen.computeCostWithMinimizingSign(swaths2) + ".";
    assert not (swaths.size() > poly.getArea() / (swath_width * swath_width)), \
        "Error 2004: " + swaths2.size() +                                      \
        " swaths were generated with angle " + rand_ang +                      \
        "Expected less than " + poly.getArea() / (swath_width * swath_width) + \
        "swaths.";
  
