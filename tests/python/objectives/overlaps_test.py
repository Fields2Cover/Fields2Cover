#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error


def test_fields2cover_obj_overlaps_computeCost():
  path1 = f2c.LineString();
  path2 = f2c.LineString();
  path3 = f2c.LineString();
  width = 2.0;
  path1.addPoint( 0.0, 1.0);
  path1.addPoint( 4.0, 1.0);
  path2.addPoint( 0.0, 3.0);
  path2.addPoint( 4.0, 3.0);
  path3.addPoint( 0.0, 2.0);
  path3.addPoint( 4.0, 2.0);  
  swath1 = f2c.Swath(path1, width);
  swath2 = f2c.Swath(path2, width);
  swath3 = f2c.Swath(path3, width);
  	
  swaths_no_overlap = f2c.Swaths();
  [swaths_no_overlap.push_back(i) for i in [swath1, swath2]]
  swaths_overlap_025 = f2c.Swaths();
  [swaths_overlap_025.push_back(i) for i in [swath1, swath3]]
  swaths_overlap_050_1 = f2c.Swaths();
  [swaths_overlap_050_1.push_back(i) for i in [swath1, swath2, swath3]]
  swaths_overlap_050_2 = f2c.Swaths();
  [swaths_overlap_050_2.push_back(i) for i in [swath2, swath1, swath3]]
  swaths_overlap_050_3 = f2c.Swaths();
  [swaths_overlap_050_3.push_back(i) for i in [swath2, swath2]]
  
  field = f2c.Cell();
  fields = f2c.Cells();
  
  line = f2c.LinearRing();
  line.addPoint(0, 0);
  line.addPoint(4, 0);
  line.addPoint(4, 4);
  line.addPoint(0, 4);
  line.addPoint(0, 0);
  field.addRing(line);
  fields.addGeometry(field);
  
  overlap = f2c.OBJ_Overlaps();
    
  near(overlap.computeCost(field, swaths_no_overlap), 0.0);
  near(overlap.computeCost(fields, swaths_no_overlap), 0.0);
  near(overlap.computeCostWithMinimizingSign(fields, swaths_no_overlap), 0.0);

  near(overlap.computeCost(fields, swaths_overlap_025), 0.25);
  near(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_025), 0.25);
  
  near(overlap.computeCost(fields, swaths_overlap_050_1), 0.5);
  near(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_050_1), 0.5);

  near(overlap.computeCost(fields, swaths_overlap_050_2), 0.5);
  near(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_050_2), 0.5);
  
  near(overlap.computeCost(fields, swaths_overlap_050_3), 0.5);
  near(overlap.computeCostWithMinimizingSign(fields, swaths_overlap_050_3), 0.5);

def test_fields2cover_obj_overlaps_params_check():
  objective = f2c.OBJ_Overlaps();

  assert not (objective.isMaximizing());
  assert (objective.isMinimizing());

