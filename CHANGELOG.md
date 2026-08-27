# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- `F2CCells::carveSharedBorders(width)` opens a corridor of the given width where cells of the set border each other. Only the part of an edge a neighbour actually touches is cut, and the whole corridor comes out of the cell with the shorter border, so the larger neighbour keeps its shape. Edges facing the outer boundary or a void are left untouched.
- Python module is built as a proper package with scikit-build-core (`pip install .`); version is taken from `CMakeLists.txt` and exposed as `fields2cover.__version__`.
- Source distribution published to PyPI (`pip install fields2cover`).

### Fixed
- `F2CCells::getCellBorder`, `getInteriorRing` and `addRing` no longer segfault on an empty polygon or an out-of-range index; they throw `std::out_of_range` like `getGeometry` does.
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














