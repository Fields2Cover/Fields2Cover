//=============================================================================
//    Copyright (C) 2021-2024 Wageningen University - All Rights Reserved
//                     Author: Gonzalo Mier
//                        BSD-3 License
//=============================================================================

#include "fields2cover/decomposition/boustrophedon_decomp.h"

namespace f2c::decomp
{

bool BoustrophedonDecomp::isLinePartOfLinearRing(
  const F2CLinearRing & ring,
  const F2CLineString & line) const
{
  if (ring.size() == 0 || line.size() == 0) {return false;}

  // Iterate over the edges (segments) of the ring
  for (int i = 0; i < ring.size() - 1; ++i) { // Exclude the last point to avoid looping twice
    auto ringPointStart = ring.getGeometry(i);
    auto ringPointEnd = ring.getGeometry(i + 1);

    // Check if the line matches the current edge of the ring
    if ((line.startPoint() == ringPointStart && line.endPoint() == ringPointEnd) ||
      (line.startPoint() == ringPointEnd && line.endPoint() == ringPointStart))
    {
      // The line matches one of the ring edges
      return true;
    }
  }

  // If the line doesn't match any edge of the ring, return false
  return false;
}

F2CMultiLineString BoustrophedonDecomp::genSplitLines(
  const F2CCells & cells, const obj::DecompObjective & obj)
{
  F2CMultiLineString split_lines;
  for (auto && cell : cells) {
    for (auto && ring : cell) {
      for (auto && p : ring) {
        auto line1 = cell.createLineUntilBorder(p, split_angle);
        while (isLinePartOfLinearRing(ring, line1)) {
          line1 = cell.createLineUntilBorder(line1.endPoint(), split_angle);
        }
        auto line2 = cell.createLineUntilBorder(p, split_angle + M_PI);
        while (isLinePartOfLinearRing(ring, line2)) {
          line2 = cell.createLineUntilBorder(line2.endPoint(), split_angle + M_PI);
        }

        if (line1.length() > 1e-5 && line2.length() > 1e-5) {
          split_lines.addGeometry(line1);
          split_lines.addGeometry(line2);
        }
      }
    }
  }
  return split_lines;
}

}  // namespace f2c::decomp
