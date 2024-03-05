#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_types_linearring_init():
  ring1 = f2c.LinearRing();
  ring1.addPoint(1,1);
  ring1.addPoint(2,1);
  near(ring1.size(), 2);
  ring2 = f2c.LinearRing(f2c.VectorPoint([f2c.Point(1,2), f2c.Point(3,2), f2c.Point(1,3)]));
  near(ring2.size(), 3);

