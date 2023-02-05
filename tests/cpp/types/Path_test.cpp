//=============================================================================
//    Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include <gtest/gtest.h>
#include "fields2cover/types.h"

TEST(fields2cover_types_path, appendSwath) {
  F2CLineString line1({F2CPoint(0.0, 1.0), F2CPoint(1.0, 1.0), F2CPoint(1.0, 4.0)});
  F2CSwath swath1(line1);
  F2CPath path1;
  path1.appendSwath(swath1, 2.0);
  EXPECT_EQ(path1.states[0].point.getX(), 0.0);
  EXPECT_EQ(path1.states[0].point.getY(), 1.0);
  EXPECT_EQ(path1.states[0].velocity, 2.0);
  EXPECT_EQ(path1.states[0].duration, 0.5);
  EXPECT_EQ(static_cast<int>(path1.states[0].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.states[1].point.getX(), 1.0);
  EXPECT_EQ(path1.states[1].point.getY(), 1.0);
  EXPECT_EQ(path1.states[1].velocity, 2.0);
  EXPECT_EQ(path1.states[1].duration, 1.5);
  EXPECT_EQ(static_cast<int>(path1.states[1].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));

  EXPECT_EQ(path1.states[2].point.getX(), 1.0);
  EXPECT_EQ(path1.states[2].point.getY(), 4.0);
  EXPECT_EQ(path1.states[2].velocity, 2.0);
  EXPECT_EQ(path1.states[2].duration, 0.0);
  EXPECT_EQ(static_cast<int>(path1.states[2].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.task_time, 2.0);
}

TEST(fields2cover_types_path, op_plus_equal) {
  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 1.0), F2CPoint(1.0, 1.0), F2CPoint(1.0, 4.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(1.0, 4.0), F2CPoint(1.0, 0.0), F2CPoint(0.0, 0.0)}));

  F2CPath path1, path2;
  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;
  EXPECT_EQ(path1.states[0].point.getX(), 0.0);
  EXPECT_EQ(path1.states[0].point.getY(), 1.0);
  EXPECT_EQ(path1.states[0].velocity, 2.0);
  EXPECT_EQ(path1.states[0].duration, 0.5);
  EXPECT_EQ(static_cast<int>(path1.states[0].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.states[1].point.getX(), 1.0);
  EXPECT_EQ(path1.states[1].point.getY(), 1.0);
  EXPECT_EQ(path1.states[1].velocity, 2.0);
  EXPECT_EQ(path1.states[1].duration, 1.5);
  EXPECT_EQ(static_cast<int>(path1.states[1].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.states[2].point.getX(), 1.0);
  EXPECT_EQ(path1.states[2].point.getY(), 4.0);
  EXPECT_EQ(path1.states[2].velocity, 2.0);
  EXPECT_EQ(path1.states[2].duration, 0.0);
  EXPECT_EQ(static_cast<int>(path1.states[2].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.states[3].point.getX(), 1.0);
  EXPECT_EQ(path1.states[3].point.getY(), 4.0);
  EXPECT_EQ(path1.states[3].velocity, 1.0);
  EXPECT_EQ(path1.states[3].duration, 4.0);
  EXPECT_EQ(static_cast<int>(path1.states[3].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.states[4].point.getX(), 1.0);
  EXPECT_EQ(path1.states[4].point.getY(), 0.0);
  EXPECT_EQ(path1.states[4].velocity, 1.0);
  EXPECT_EQ(path1.states[4].duration, 1.0);
  EXPECT_EQ(static_cast<int>(path1.states[4].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.states[5].point.getX(), 0.0);
  EXPECT_EQ(path1.states[5].point.getY(), 0.0);
  EXPECT_EQ(path1.states[5].velocity, 1.0);
  EXPECT_EQ(path1.states[5].duration, 0.0);
  EXPECT_EQ(static_cast<int>(path1.states[5].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.task_time, 7.0);
  EXPECT_EQ(path1.size(), 6);
}

TEST(fields2cover_types_path, populate_and_reduce) {
  F2CPath path1, path2;
  path1.populate(200);
  EXPECT_EQ(path1.size(), 0);
  EXPECT_NEAR(path1.task_time, 0.0, 1e-6);

  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0), F2CPoint(2.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(3.0, 1.0), F2CPoint(3.0, 4.0), F2CPoint(1.0, 4.0)}));

  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;

  EXPECT_EQ(path1.length(), 9);
  EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto s : path1.states) {
    EXPECT_EQ( static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
  }
  auto path_c = path1.clone();
  path1.populate(200);
  EXPECT_GT(path1.length(), 9.0);
  EXPECT_LT(path1.length(), 2.0 * 9.0);
  EXPECT_EQ(path1.size(), 200);
  EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto s : path1.states) {
    EXPECT_EQ(static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
    EXPECT_EQ(static_cast<int>(s.type),
        static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
  path1.populate(10);
  EXPECT_GT(path1.length(), 9.0);
  EXPECT_LT(path1.length(), 2.0 * 9.0);
  EXPECT_EQ(path1.size(), 10);
  EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto s : path1.states) {
    EXPECT_EQ(static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
    EXPECT_EQ(static_cast<int>(s.type),
        static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
  path1.populate(500);
  EXPECT_GT(path1.length(), 9.0);
  EXPECT_LT(path1.length(), 2.0 * 9.0);
  path1.reduce(0.1);
  EXPECT_GT(path1.length(), 9.0);
  EXPECT_LT(path1.length(), 2.0 * 9.0);
  path1.reduce(0.1);
  EXPECT_GT(path1.length(), 9.0);
  EXPECT_LT(path1.length(), 2.0 * 9.0);
  EXPECT_LT(path1.size(), 100);

  // Time may change with reduce command.
  // Unexpected behaviour
  // EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto s : path1.states) {
    EXPECT_EQ(static_cast<int>(s.dir),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
    EXPECT_EQ(static_cast<int>(s.type),
        static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
}

TEST(fields2cover_types_path, length) {
  F2CLineString line1, line2;
  line1.addPoint( 0.0, 1.0);
  line1.addPoint( 1.0, 1.0);
  line1.addPoint( 1.0, 4.0);
  line2.addPoint( 1.0, 4.0);
  line2.addPoint( 1.0, 0.0);
  line2.addPoint( 0.0, 0.0);
  F2CSwath swath1(line1);
  F2CSwath swath2(line2);

  F2CPath path1, path2;
  path1.appendSwath(swath1, 2.0);
  EXPECT_EQ(path1.length(), 4.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;
  EXPECT_EQ(path1.length(), 9.0);
  path1 = path2;
  EXPECT_EQ(path1.length(), 5.0);
}

TEST(fields2cover_types_path, serialize) {
  F2CPath path, path_read;
  f2c::types::PathState state;

  EXPECT_EQ(0, path.serializePath().compare(""));
  state.point = F2CPoint(2.3, -5);
  state.angle = 0.1;
  state.velocity = 3.0;
  state.duration = 6.0;
  state.dir = f2c::types::PathDirection::FORWARD;
  state.type = f2c::types::PathSectionType::TURN;
  path.states.push_back(state);

  EXPECT_EQ(path.serializePath(),
        "2.3 -5 0 0.1 3 6 1 2\n");

  state.point = F2CPoint(0.1234567890123, -9.87654);
  state.angle = 0.2;
  state.velocity = 3.2;
  state.duration = 6.2;
  state.dir = f2c::types::PathDirection::BACKWARD;
  state.type = f2c::types::PathSectionType::TURN;
  path.states.push_back(state);

  EXPECT_EQ(path.serializePath(10),
        "2.3 -5 0 0.1 3 6 1 2\n0.123456789 -9.87654 0 0.2 3.2 6.2 -1 2\n");

  path.saveToFile("/tmp/test_path");
  path_read.loadFile("/tmp/test_path");
  EXPECT_EQ(path.serializePath(), path_read.serializePath());
}


TEST(fields2cover_types_path, moveTo) {
  F2CLineString line1({F2CPoint(0.0, 1.0), F2CPoint(1.0, 1.0), F2CPoint(1.0, 4.0)});
  F2CSwath swath1(line1);
  F2CPath path1;
  path1.appendSwath(swath1, 2.0);
  path1.moveTo(F2CPoint(100, 200));
  EXPECT_EQ(path1.states[0].point.getX(), 100.0);
  EXPECT_EQ(path1.states[0].point.getY(), 201.0);
  EXPECT_EQ(path1.states[0].velocity, 2.0);
  EXPECT_EQ(path1.states[0].duration, 0.5);
  EXPECT_EQ(static_cast<int>(path1.states[0].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.states[1].point.getX(), 101.0);
  EXPECT_EQ(path1.states[1].point.getY(), 201.0);
  EXPECT_EQ(path1.states[1].velocity, 2.0);
  EXPECT_EQ(path1.states[1].duration, 1.5);
  EXPECT_EQ(static_cast<int>(path1.states[1].type),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
}


