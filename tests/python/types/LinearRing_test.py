#==============================================================================
#     Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
#                      Author: Gonzalo Mier
#                         BSD-3 License
#==============================================================================

import pytest
import fields2cover as f2c

def near(a, b):
  assert pytest.approx(a) == pytest.approx(b)


def test_fields2cover_types_linearring_init():
  ring1 = f2c.LinearRing();
  ring1.addPoint(1,1);
  ring1.addPoint(2,1);
  near(ring1.size(), 2);
  ring2 = f2c.LinearRing(f2c.VectorPoint([f2c.Point(1,2), f2c.Point(3,2), f2c.Point(1,3)]));
  near(ring2.size(), 3);


def test_fields2cover_types_linearring_parallel_line():
  ring = f2c.LinearRing(f2c.VectorPoint([
    f2c.Point(0,0), f2c.Point(4,0), f2c.Point(4,4), f2c.Point(0,4), f2c.Point(0,0)]));
  near(ring.getParallelLine(1.0).area(), 4);
  near(ring.getParallelLine(-1.0).area(), 36);
  near(ring.getParallelLine(f2c.VectorDouble([1.0, 0.0, 0.0, 0.0])).area(), 12);


def test_fields2cover_types_linearring_buffer_outwards_inwards():
  ring = f2c.LinearRing(f2c.VectorPoint([
    f2c.Point(0,0), f2c.Point(4,0), f2c.Point(4,4), f2c.Point(0,4), f2c.Point(0,0)]));
  ds = f2c.VectorDouble([1.0, 1.0, 1.0, 1.0]);
  # clone() first: a copy shares the underlying geometry.
  out_ring = ring.clone();
  out_ring.bufferOutwards(ds);
  near(out_ring.area(), 36);
  in_ring = ring.clone();
  in_ring.bufferInwards(ds);
  near(in_ring.area(), 4);


def test_fields2cover_types_linearring_segments():
  ring = f2c.LinearRing(f2c.VectorPoint([
    f2c.Point(0,0), f2c.Point(4,0), f2c.Point(4,3), f2c.Point(0,0)]));
  near(ring.segmentLength(2), 5);
  near(ring.segmentAng(1), 0.5 * 3.14159265358979);
  near(ring.getSegment(0).length(), 4);
  near(ring.getLastSegment().length(), 5);
  near(ring.removePoint(1).size(), 3);
