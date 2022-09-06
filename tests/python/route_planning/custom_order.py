import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_custom_order():
  n = 10;
  swaths = f2c.Swaths();
  for i in range(n):
    swaths.push_back(f2c.Swath(f2c.LineString(f2c.VectorPoint(  \
        [f2c.Point(0, i), f2c.Point(1, i)])), i, i));

  swath_order = f2c.RP_CustomOrder(swaths, range(10));
  swaths = swath_order.genSortedSwaths();

  near(swaths[0].getWidth(), 0);
  near(swaths[1].getWidth(), 1);
  near(swaths[2].getWidth(), 2);
  near(swaths[3].getWidth(), 3);
  near(swaths[4].getWidth(), 4);
  near(swaths[5].getWidth(), 5);
  near(swaths[6].getWidth(), 6);
  near(swaths[7].getWidth(), 7);
  near(swaths[8].getWidth(), 8);
  near(swaths[9].getWidth(), 9);
