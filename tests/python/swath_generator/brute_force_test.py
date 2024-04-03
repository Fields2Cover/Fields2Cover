#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c
import math as m
from .SG_checker import IsSwathGenerationCorrect

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_swath_brute_gen_bestAngle():
  rand = f2c.Random(4);
  obj = f2c.OBJ_NSwath();
  sw_gen = f2c.SG_BruteForce();
  sw_gen.setStepAngle(m.pi/180.0);
  field = rand.genConvexField(1e3);
  poly = field.getField().getCell(0);
  IsSwathGenerationCorrect(sw_gen, obj, poly);
