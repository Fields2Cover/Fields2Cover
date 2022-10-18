#==============================================================================
#     Copyright (C) 2021-2022 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b, error = 1e-7):
  assert abs(a - b) < error

def test_fields2cover_obj_field_coverage_getBestValue():
  width = 2.0;

  swath1 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 1.0), f2c.Point(4.0, 1.0)])), width);
  swath2 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 3.0), f2c.Point(4.0, 3.0)])), width);
  swath3 = f2c.Swath(f2c.LineString(f2c.VectorPoint(
      [f2c.Point(0.0, 2.0), f2c.Point(4.0, 2.0)])), width);

  swaths = f2c.Swaths();
  [swaths.push_back(i) for i in [swath1]]

  field = f2c.Cell();
  line = f2c.LinearRing();
  line.addPoint(0, 0);
  line.addPoint(4, 0);
  line.addPoint(4, 4);
  line.addPoint(0, 4);
  line.addPoint(0, 0);
  field.addRing(line);
  fields = f2c.Cells(field);

  obj = f2c.OBJ_FieldCoverage();
  near(obj.computeCost(field, swaths), 0.5, 1e-5);
  near(obj.computeCost(fields, swaths), 0.5, 1e-5);

  swaths.push_back(swath3);
  near(obj.computeCost(fields, swaths), 0.75, 1e-5);

  swaths.push_back(swath2);
  near(obj.computeCost(fields, swaths), 1.0, 1e-5);


def test_fields2cover_obj_field_coverage_computeCost():
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
  	
  swaths_full = f2c.Swaths();
  [swaths_full.push_back(i) for i in [swath1,swath2]]
  swaths_overlap = f2c.Swaths();
  [swaths_overlap.push_back(i) for i in [swath1,swath3]]
  swaths_half = f2c.Swaths();
  [swaths_half.push_back(i) for i in [swath1]]
  
  fields = f2c.Cells();
  field = f2c.Cell();
  
  line = f2c.LinearRing();
  line.addPoint(0, 0);
  line.addPoint(4, 0);
  line.addPoint(4, 4);
  line.addPoint(0, 4);
  line.addPoint(0, 0);
  field.addRing(line);
  fields.addGeometry(field);
  
  coverage = f2c.OBJ_FieldCoverage();

  near(coverage.computeCost(fields, swaths_full), 1.0);
  near(coverage.computeCostWithMinimizingSign(fields, swaths_full), -1.0);

  near(coverage.computeCost(fields, swaths_overlap), 0.75);
  near(coverage.computeCostWithMinimizingSign(fields, swaths_overlap), -0.75);

  near(coverage.computeCost(fields, swaths_half), 0.5);
  near(coverage.computeCostWithMinimizingSign(fields, swaths_half), -0.5);

def test_fields2cover_obj_field_coverage_paramsCheck():
  objective = f2c.OBJ_FieldCoverage();
  assert (objective.isMaximizing());
  assert not (objective.isMinimizing());

