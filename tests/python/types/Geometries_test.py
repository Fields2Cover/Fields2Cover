#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_types_geometries_accessIterators():
  N = 10;
  line = f2c.LineString();

  for i in range(N):
    line.addPoint(i,i);

  for i in range(N):
    near(line.getX(i), i);
    near(line.getY(i), i);

  near(line.size(), N);

  """
  i = 0;
  for p in line:
    near(p.getX(), i);
    near(p.getY(), i);
    i = i + 1;
  """


def test_fields2cover_types_geometries_multIterators():
  N = 2;
  line = f2c.LinearRing();
  print(1)
  for i in range(N):
    line.addPoint(i,i);
  print(2)

  for i in range(N):
    near(line.getX(i), i);
    near(line.getY(i), i);
  print(3)

  near(line.size(), N);

  """
  i = 0;
  for p in line:
    near(p.getX(), i);
    near(p.getY(), i);
    p *= 10.0;
    near(p.getX(), 1e1 * i);
    near(p.getY(), 1e1 * i);
    i = i+ 1;
  print(4)

  i = 0;
  for p in line:
    near(p.getX(), 1e1 * i);
    near(p.getY(), 1e1 * i);
    i = i + 1;
  print(5)

  i = 0;
  for p in line:
    near(p.getX(), 1e1 * i);
    near(p.getY(), 1e1 * i);
    p *= 10.0;
    near(p.getX(), 1e2 * i);
    near(p.getY(), 1e2 * i);
    i = i + 1;

  i = 0;
  for p in line:
    near(p.getX(), 1e2 * i);
    near(p.getY(), 1e2 * i);
    i = i + 1;
  """
