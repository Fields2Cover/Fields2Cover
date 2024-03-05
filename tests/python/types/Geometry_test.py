#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import numpy as np
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def test_fields2cover_types_geometry_getAngContinuity():
  v = f2c.VectorDouble();
  for d in np.arange(-50.0, 50.0, 0.1):
    v.push_back(f2c.Point.mod_2pi(d));
    near(f2c.Point.getAngContinuity(d - 0.1, f2c.Point.mod_2pi(d)), d, 1e-5);

  for d in np.arange(50.0, -50.0, -0.1):
    v.push_back(f2c.Point.mod_2pi(d));
    near(f2c.Point.getAngContinuity(d + 0.1, f2c.Point.mod_2pi(d)), d, 1e-5);

  restored_v = f2c.Point.getAngContinuity(v);

  for i in range(1, restored_v.size()):
    near(abs(restored_v[i] - restored_v[i - 1]), 0.1, 1e-5);




