#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_types_robot_init():
  robot = f2c.Robot(3, 6);
  near(robot.getWidth(), 3);
  near(robot.getCovWidth(), 6);

  r1 = robot;
  near(r1.getWidth(), 3);
  near(r1.getCovWidth(), 6);

  r2 = f2c.Robot(1, 2);
  near(r2.getWidth(), 1);
  near(r2.getCovWidth(), 2);

  r3 = f2c.Robot();
  near(r3.getWidth(), 0);
  near(r3.getCovWidth(), 0);

  r4 = f2c.Robot();
  r4.setMinTurningRadius(20.0);
  r4.setMaxDiffCurv(0.2);
  near(r4.getMaxCurv(), 1/20.0);
  near(r4.getMaxDiffCurv(), 0.2);

  with pytest.raises(Exception) as e_info:
    f2c.Robot(0, 0);
  with pytest.raises(Exception) as e_info:
    f2c.Robot(-3, 3);
  with pytest.raises(Exception) as e_info:
    f2c.Robot(3, -3);


