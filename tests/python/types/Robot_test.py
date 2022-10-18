#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)

def test_fields2cover_types_robot_init():
  robot = f2c.Robot(3, 6);
  near(robot.robot_width, 3);
  near(robot.op_width, 6);
  near(robot.getRobotWidth(), 3);

  r1 = robot;
  near(r1.robot_width, 3);
  near(r1.op_width, 6);
  near(r1.getRobotWidth(), 3);

  r2 = f2c.Robot(1, 2);
  near(r2.robot_width, 1);
  near(r2.op_width, 2);
  near(r2.getRobotWidth(), 1);

  r3 = f2c.Robot();
  near(r3.robot_width, 0);
  near(r3.op_width, 0);
  near(r3.getRobotWidth(), 0);

  r4 = f2c.Robot();
  r4.setMinRadius(20.0);
  near(r4.op_width, 0);
  near(r4.robot_width, 0);
  near(r4.max_icc, 1/20.0);

  with pytest.raises(Exception) as e_info:
    f2c.Robot(0, 0);
  with pytest.raises(Exception) as e_info:
    f2c.Robot(-3, 3);
  with pytest.raises(Exception) as e_info:
    f2c.Robot(3, -3);
  with pytest.raises(Exception) as e_info:
    f2c.Robot(6, 3);
