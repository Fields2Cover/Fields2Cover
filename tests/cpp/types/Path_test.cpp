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
  EXPECT_EQ(path1.points[0].getX(), 0.0);
  EXPECT_EQ(path1.points[0].getY(), 1.0);
  EXPECT_EQ(path1.velocities[0], 2.0);
  EXPECT_EQ(path1.durations[0], 0.5);
  EXPECT_EQ(static_cast<int>(path1.type[0]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.points[1].getX(), 1.0);
  EXPECT_EQ(path1.points[1].getY(), 1.0);
  EXPECT_EQ(path1.velocities[1], 2.0);
  EXPECT_EQ(path1.durations[1], 1.5);
  EXPECT_EQ(static_cast<int>(path1.type[1]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));

  EXPECT_EQ(path1.points[2].getX(), 1.0);
  EXPECT_EQ(path1.points[2].getY(), 4.0);
  EXPECT_EQ(path1.velocities[2], 2.0);
  EXPECT_EQ(path1.durations[2], 0.0);
  EXPECT_EQ(static_cast<int>(path1.type[2]),
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
  EXPECT_EQ(path1.points[0].getX(), 0.0);
  EXPECT_EQ(path1.points[0].getY(), 1.0);
  EXPECT_EQ(path1.velocities[0], 2.0);
  EXPECT_EQ(path1.durations[0], 0.5);
  EXPECT_EQ(static_cast<int>(path1.type[0]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.points[1].getX(), 1.0);
  EXPECT_EQ(path1.points[1].getY(), 1.0);
  EXPECT_EQ(path1.velocities[1], 2.0);
  EXPECT_EQ(path1.durations[1], 1.5);
  EXPECT_EQ(static_cast<int>(path1.type[1]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.points[2].getX(), 1.0);
  EXPECT_EQ(path1.points[2].getY(), 4.0);
  EXPECT_EQ(path1.velocities[2], 2.0);
  EXPECT_EQ(path1.durations[2], 0.0);
  EXPECT_EQ(static_cast<int>(path1.type[2]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.points[3].getX(), 1.0);
  EXPECT_EQ(path1.points[3].getY(), 4.0);
  EXPECT_EQ(path1.velocities[3], 1.0);
  EXPECT_EQ(path1.durations[3], 4.0);
  EXPECT_EQ(static_cast<int>(path1.type[3]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.points[4].getX(), 1.0);
  EXPECT_EQ(path1.points[4].getY(), 0.0);
  EXPECT_EQ(path1.velocities[4], 1.0);
  EXPECT_EQ(path1.durations[4], 1.0);
  EXPECT_EQ(static_cast<int>(path1.type[4]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.points[5].getX(), 0.0);
  EXPECT_EQ(path1.points[5].getY(), 0.0);
  EXPECT_EQ(path1.velocities[5], 1.0);
  EXPECT_EQ(path1.durations[5], 0.0);
  EXPECT_EQ(static_cast<int>(path1.type[5]),
      static_cast<int>(f2c::types::PathSectionType::SWATH));
  EXPECT_EQ(path1.task_time, 7.0);
  EXPECT_EQ(path1.points.size(), 6);
  EXPECT_EQ(path1.durations.size(), 6);
  EXPECT_EQ(path1.directions.size(), 6);
  EXPECT_EQ(path1.velocities.size(), 6);
  EXPECT_TRUE(path1.isValid());
}

TEST(fields2cover_types_path, populate_and_reduce) {
  F2CPath path1, path2;
  path1.populate(200);
  EXPECT_TRUE(path1.isValid());
  EXPECT_EQ(path1.velocities.size(), 0);
  EXPECT_NEAR(path1.task_time, 0.0, 1e-6);

  F2CSwath swath1(F2CLineString({F2CPoint(0.0, 0.0), F2CPoint(0.0, 1.0), F2CPoint(2.0, 1.0)}));
  F2CSwath swath2(F2CLineString({F2CPoint(3.0, 1.0), F2CPoint(3.0, 4.0), F2CPoint(1.0, 4.0)}));
  	
  path1.appendSwath(swath1, 2.0);
  path2.appendSwath(swath2, 1.0);
  path1 += path2;

  EXPECT_EQ(path1.length(), 9);
  EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto d : path1.directions) {
    EXPECT_EQ( static_cast<int>(d),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
  }
  auto path_c = path1.clone();
  path1.populate(200);
  EXPECT_GT(path1.length(), 9.0);
  EXPECT_LT(path1.length(), 2.0 * 9.0);
  EXPECT_TRUE(path1.isValid());
  EXPECT_EQ(path1.points.size(), 200);
  EXPECT_EQ(path1.angles.size(), 200);
  EXPECT_EQ(path1.velocities.size(), 200);
  EXPECT_EQ(path1.durations.size(), 200);
  EXPECT_EQ(path1.directions.size(), 200);
  EXPECT_EQ(path1.type.size(), 200);
  EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto d : path1.directions) {
    EXPECT_EQ(static_cast<int>(d),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
  }
  for (auto t : path1.type) {
    EXPECT_EQ(static_cast<int>(t),
        static_cast<int>(f2c::types::PathSectionType::SWATH));
  }
  path1.populate(10);
  EXPECT_TRUE(path1.isValid());
  EXPECT_GT(path1.length(), 9.0);
  EXPECT_LT(path1.length(), 2.0 * 9.0);
  EXPECT_EQ(path1.points.size(), 10);
  EXPECT_EQ(path1.angles.size(), 10);
  EXPECT_EQ(path1.velocities.size(), 10);
  EXPECT_EQ(path1.durations.size(), 10);
  EXPECT_EQ(path1.directions.size(), 10);
  EXPECT_EQ(path1.type.size(), 10);
  EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto d : path1.directions) {
    EXPECT_EQ(static_cast<int>(d),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
  }
  for (auto t : path1.type) {
    EXPECT_EQ(static_cast<int>(t),
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
  EXPECT_TRUE(path1.isValid());
  EXPECT_LT(path1.points.size(), 100);
  EXPECT_LT(path1.angles.size(), 100);
  EXPECT_LT(path1.velocities.size(), 100);
  EXPECT_LT(path1.durations.size(), 100);
  EXPECT_LT(path1.directions.size(), 100);
  EXPECT_LT(path1.type.size(), 100);

  // Time may change with reduce command. 
  // Unexpected behaviour
  // EXPECT_NEAR(path1.task_time, 6.5, 1e-6);
  for (auto d : path1.directions) {
    EXPECT_EQ(static_cast<int>(d),
        static_cast<int>(f2c::types::PathDirection::FORWARD));
  }
  for (auto t : path1.type) {
    EXPECT_EQ(static_cast<int>(t),
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
  F2CPath path;
  EXPECT_EQ(0, path.serializePath().compare(""));
  path.points.push_back(F2CPoint(2.3, -5));
  path.angles.push_back(0.1);
  path.velocities.push_back(3);
  path.durations.push_back(3);
  path.directions.push_back(f2c::types::PathDirection::FORWARD);
  path.type.push_back(f2c::types::PathSectionType::TURN);

  EXPECT_EQ(path.serializePath(),
        "2.3 -5 0.1 3 3 1 2\n");
}


TEST(fields2cover_types_path, save_load) {
  F2CPath path, path_read;
  path.points.push_back(F2CPoint(-2.3, 5));
  path.angles.push_back(0.1);
  path.velocities.push_back(4.5);
  path.durations.push_back(6);
  path.directions.push_back(f2c::types::PathDirection::BACKWARD);
  path.type.push_back(f2c::types::PathSectionType::SWATH);

  EXPECT_EQ(path.serializePath(),
        "-2.3 5 0.1 4.5 6 -1 1\n");
  path.saveToFile("/tmp/test_path");
  path_read.loadFile("/tmp/test_path");
  EXPECT_EQ(path.serializePath(), path_read.serializePath());
}


