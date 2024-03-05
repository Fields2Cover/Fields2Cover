import pytest
import fields2cover as f2c

PARAMETRIZED_INPUTS = [
  (5, [0, 1, 2, 3, 4], [0, 1, 2, 3, 4]),
  (10, range(10), range(10))
]

PARAMETRIZED_INPUTS_WRONG = [
  (5, [0, 1, 2, 3, 4], [1, 2, 3, 4, 5]),
  (5, [0, 1, 2, 3, 4], [1, 1, 2, 2, 4]),
  (10, range(10), range(1,11))
]

@pytest.mark.parametrize('n,custom_order,expected', PARAMETRIZED_INPUTS)
def test_fields2cover_custom_order(n, custom_order, expected):
  swaths = f2c.Swaths()
  for i in range(n):
    swaths.push_back(
        f2c.Swath(
          f2c.LineString(
            f2c.VectorPoint([
              f2c.Point(0, i),
              f2c.Point(1, i)])),
          1,
          i
        )
    )
  swath_order = f2c.RP_CustomOrder(f2c.VectorSize(custom_order))
  swaths = swath_order.genSortedSwaths(swaths)

  actual = [swaths.at(i).getId() for i in range(swaths.size())]
  assert len(actual) == len(expected)
  assert all(a == b for a, b in zip(actual, expected))

@pytest.mark.parametrize('n,custom_order,expected', PARAMETRIZED_INPUTS_WRONG)
def test_fields2cover_wrong_custom_order(n, custom_order, expected):
  swaths = f2c.Swaths()
  for i in range(n):
    swaths.push_back(
        f2c.Swath(
            f2c.LineString(
                f2c.VectorPoint([f2c.Point(0, i),
                                 f2c.Point(1, i)])), 1, i))
  swath_order = f2c.RP_CustomOrder(f2c.VectorSize(custom_order))
  swaths = swath_order.genSortedSwaths(swaths)
  actual = [swaths.at(i).getId() for i in range(swaths.size())]
  assert any(a != b for a, b in zip(actual, expected))

