#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_hl_const_gen_emptyArea():
  rand = f2c.Random();
  field = rand.generateRandField(5, 1e3);
  near(field.getArea(), 1e3);

  hl_gen = f2c.HG_Const_gen();
  no_hl = hl_gen.generateHeadlands(field.field, 1e2);
  near(no_hl.getArea(), 0);

def test_fields2cover_hl_const_gen_borderArea():
  rand = f2c.Random();
  field = rand.generateRandField(5, 1e5);

  hl_gen = f2c.HG_Const_gen();
  no_hl = hl_gen.generateHeadlands(field.field, 1.0);

  assert field.getArea() > 0;
  near(field.getArea(), 1e5);
  assert (no_hl.getArea() < 1e5 * 0.99);
  assert (no_hl.getArea() > 1e5 * 0.9);
  assert (no_hl.getArea() / field.getArea() > 0.9);
  assert (no_hl.getArea() / field.getArea() < 1.0);

  extra_hl = hl_gen.generateHeadlands(field.field, -10.0);
  assert (field.getArea() < extra_hl.getArea());

