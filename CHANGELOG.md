# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- `f2c::hg::ReqHL`, a headland generator that sizes each border on its own: a border the swaths run along is only entered, while a border they end on takes a whole turn. The difference is left to the mainland instead of being given up on every border.
- `HeadlandGeneratorBase::generateHeadlands` overloads taking a robot and the track angles, and `maxHLWidthRequired`.
- `F2CLinearRing::getParallelLine`, `bufferOutwards`, `bufferInwards`, `filterSelfIntersections`, `removePoint`, `getSegment`, `getLastSegment`, `segmentLength` and `segmentAng`, to offset each segment of a ring by its own distance.
- `Geometry::contains`, the other side of `Geometry::within`.
- `f2c::hg::CorridorHL`, a headland generator that opens a corridor where cells border each other instead of shrinking every border. Only the part of an edge a neighbour actually touches is cut, and the corridor comes out of the smaller cell so the larger neighbour keeps its shape; cells of the same size split it evenly. Edges facing the outer boundary or a void are left untouched.
- Python module is built as a proper package with scikit-build-core (`pip install .`); version is taken from `CMakeLists.txt` and exposed as `fields2cover.__version__`.
- Source distribution published to PyPI (`pip install fields2cover`).

### Fixed
- `Cells::splitByLine` no longer throws `std::invalid_argument` when a split leaves a piece touching itself at a single point. Reinflating each split piece went through `Cell::buffer`, which only accepts a single polygon back; a positive buffer on a pinched piece can separate it into two. The reinflate step was also a no-op — a range-for loop reassigning its own by-value loop variable, never written back to the result — so it is removed instead of fixed, which changes nothing for cells that were never pinched.
- `F2CCells::getCellBorder`, `getInteriorRing` and `addRing` no longer segfault on an empty polygon or an out-of-range index; they throw `std::out_of_range` like `getGeometry` does.
- `NSwathModified::computeCost` read the wrong neighbouring point for the first edge: `(i - 1) % ring.size()` wraps a `size_t` to `SIZE_MAX % n`, which is not the previous vertex. The cost of a polygon now no longer depends on which vertex its ring starts from.
- `generateBestSwaths` no longer returns an angle that covers nothing. The objectives estimate the cost from the cell border alone, so a cell with a hairline spur could score best on an angle producing no swath at all and was silently left uncovered.

### Changed
- The decomposition tutorial carves a corridor between cells instead of running the headland generator a second time, which also shrank the outer boundary.
- `cmake --install` places the python module in the interpreter's site-packages instead of calling `setup.py install`.
- Building the python module requires CMake >= 3.18 and Python >= 3.9.

## [2.0.0] - 07-02-2024

- Route planner travelling through the headlands

## [1.3.0] - 21-04-2023

- Add decomposition algorithms: trapezoidal, boustrophedon
  


## [1.2.0] - 17-10-2022
### Added
- Tests to do cover < 90% functions

### Changes
- SG use the objective function as a parameter instead of a template.
- RP do not save the swaths and modify them using the functions provided
- PP do not save the robot and use the robot params with a param on the function.

### Changes
- Objectives are split for each of the modules.
- Global objective renamed to SG objective.
- Path objective renamed to RP objective.

### Added
- PP objective
- HL objective




## [1.1.0]
### Added
- On HL module: constant headland algorithm.
- On SG module: brute force algorithm.
- On RP module: Boustrophedon, custom, snake and spiral.
- On PP module: Dubins and Reeds-Sheep with/without continuous curvature.
- Objective functions are split between global and path cost functions.














