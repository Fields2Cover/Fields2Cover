#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_hl_const_gen_emptyArea():
  rand = f2c.Random();
  field = rand.generateRandField(1e3, 5);
  near(field.area(), 1e3);

  hl_gen = f2c.HG_Const_gen();
  no_hl = hl_gen.generateHeadlands(field.getField(), 1e2);
  near(no_hl.area(), 0);

def test_fields2cover_hl_const_gen_borderArea():
  rand = f2c.Random();
  field = rand.generateRandField(1e5, 5);

  hl_gen = f2c.HG_Const_gen();
  no_hl = hl_gen.generateHeadlands(field.getField(), 1.0);

  assert field.area() > 0;
  near(field.area(), 1e5);
  assert (no_hl.area() < 1e5 * 0.99);
  assert (no_hl.area() > 1e5 * 0.9);
  assert (no_hl.area() / field.area() > 0.9);
  assert (no_hl.area() / field.area() < 1.0);

  extra_hl = hl_gen.generateHeadlands(field.getField(), -10.0);
  assert (field.area() < extra_hl.area());

