import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_route_spiral_genSortedSwathsEven():
  n = 11;
  size = 6;
  swaths = f2c.Swaths();
  for i in range(1, n):
    swaths.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(  \
        [f2c.Point(0, i), f2c.Point(1, i)])), i, i));

  swath_order = f2c.RP_Spiral(swaths, size);
  swaths = swath_order.genSortedSwaths();

  near(swaths[0].getWidth(), 1);
  near(swaths[1].getWidth(), 6);
  near(swaths[2].getWidth(), 2);
  near(swaths[3].getWidth(), 5);
  near(swaths[4].getWidth(), 3);
  near(swaths[5].getWidth(), 4);
  near(swaths[6].getWidth(), 7);
  near(swaths[7].getWidth(), 10);
  near(swaths[8].getWidth(), 8);
  near(swaths[9].getWidth(), 9);
