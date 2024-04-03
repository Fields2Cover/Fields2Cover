#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def check_point(p, x, y, z):
  assert pytest.approx(p.getX()) == pytest.approx(x);
  assert pytest.approx(p.getY()) == pytest.approx(y);
  assert pytest.approx(p.getZ()) == pytest.approx(z);

def test_fields2cover_types_point_minus():
  p1 = f2c.Point(1.1, 2.2, 3.3);
  p2 = f2c.Point(-0.5, 0.6, 1);
  p3 = p2 - p1;
  check_point(p1, 1.1, 2.2, 3.3);
  check_point(p2, -0.5, 0.6, 1);
  check_point(p3, -1.6, -1.6, -2.3);
  p4 = p2 - p1;
  check_point(p1, 1.1, 2.2, 3.3);
  check_point(p2, -0.5, 0.6, 1);
  check_point(p4, -1.6, -1.6, -2.3);


def test_fields2cover_types_point_plus():
  p1 = f2c.Point(1.1, 2.2, 3.3);
  p2 = f2c.Point(-0.5, 0.6, 1);
  p3 = p2 + p1;
  check_point(p1, 1.1, 2.2, 3.3);
  check_point(p2, -0.5, 0.6, 1);
  check_point(p3, 0.6, 2.8, 4.3);

def test_fields2cover_types_point_mult_equal():
  p1 = f2c.Point(1.1, 2.2, 3.3);
  p1 *= 1.5;
  check_point(p1, 1.65, 3.3, 4.95);

def test_fields2cover_types_point_multiply():
  p1 = f2c.Point(1.1, 2.2, 3.3);
  p2 = p1 * 1.5;
  check_point(p1, 1.1, 2.2, 3.3);
  check_point(p2, 1.65, 3.3, 4.95);



