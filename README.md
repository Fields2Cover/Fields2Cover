<p align="center">
  <img src="https://raw.githubusercontent.com/Fields2Cover/Fields2Cover/main/docs/figures/logo_fields2cover.jpeg" alt="Fields2Cover logo" width="600">
</p>

<p align="center"><b>Robust and efficient coverage paths for autonomous agricultural vehicles</b></p>

<p align="center">
  <a href="https://fields2cover.github.io">Documentation</a> •
  <a href="https://fields2cover.github.io/source/installation.html">Installation</a> •
  <a href="https://fields2cover.github.io/source/quick_start.html">Quick Start</a> •
  <a href="https://fields2cover.github.io/source/tutorials.html">Tutorials</a> •
  <a href="https://fields2cover.github.io/api/f2c_library.html">API</a> •
  <a href="https://ieeexplore.ieee.org/document/10050562">Paper</a>
</p>

<p align="center">
  <a href="https://github.com/Fields2Cover/Fields2Cover/actions/workflows/build.yml"><img src="https://github.com/Fields2Cover/Fields2Cover/actions/workflows/build.yml/badge.svg?branch=main" alt="build"></a>
  <a href="https://coveralls.io/github/Fields2Cover/Fields2Cover?branch=main"><img src="https://coveralls.io/repos/github/Fields2Cover/Fields2Cover/badge.svg?branch=main" alt="coverage"></a>
  <a href="https://pypi.org/project/fields2cover/"><img src="https://img.shields.io/pypi/v/fields2cover.svg" alt="PyPI"></a>
  <a href="https://github.com/Fields2Cover/Fields2Cover/blob/main/LICENSE"><img src="https://img.shields.io/badge/license-BSD--3-orange.svg" alt="license"></a>
  <a href="https://doi.org/10.1109/LRA.2023.3248439"><img src="https://zenodo.org/badge/DOI/10.1109/LRA.2023.3248439.svg" alt="DOI"></a>
</p>

Fields2Cover is an open-source C++ library with Python bindings that solves the Coverage Path Planning problem for agriculture: given a field and a vehicle, it computes a complete coverage path — headlands, swaths, an optimized route, and a drivable path with feasible turns. It also provides a common, extensible framework to implement and compare coverage path planning algorithms, so researchers don't have to re-implement every algorithm they want to compare against. The API is still evolving, so expect occasional breaking changes between releases.

<p align="center">
  <img src="https://raw.githubusercontent.com/Fields2Cover/Fields2Cover/main/docs/figures/diagram-F2C.jpg" alt="Fields2Cover module diagram" width="500">
</p>

## Core features

- **Modular pipeline**: headland generation, swath generation, route planning and path planning as interchangeable modules — or a single call to `planCovPath`.
- **Non-convex fields and obstacles**, handled by trapezoidal and boustrophedon decomposition.
- **Route optimization** with [OR-tools](https://developers.google.com/optimization), plus classic patterns like boustrophedon, snake and spiral.
- **Kinematically feasible turns** using Dubins and Reeds-Shepp curves, with or without continuous curvature.
- **C++17 core, Python bindings**, and a ROS 2 integration via [opennav_coverage](https://github.com/open-navigation/opennav_coverage).

## Quick Start

Fields2Cover builds on GDAL, GEOS and OR-tools, which need to be installed first (details in the [installation guide](https://fields2cover.github.io/source/installation.html)):

```bash
# Ubuntu
sudo apt install build-essential libgdal-dev libgeos-dev libeigen3-dev libboost-dev \
     libtbb-dev libtinyxml2-dev nlohmann-json3-dev libpython3-dev gnuplot
# plus OR-tools for C++: https://developers.google.com/optimization/install/cpp

# macOS
brew install gdal geos or-tools tinyxml2 eigen tbb boost gnuplot
```

### C++

```cpp
#include "fields2cover.h"

int main() {
  F2CField field = f2c::Parser::importFieldGml("data/test1.xml");
  F2CRobot robot(2.0, 6.0, 0.5, 0.2);

  F2CPath path = f2c::planCovPath(robot, field, false);

  f2c::Visualizer::figure();
  f2c::Visualizer::plot(field.getCellsAbsPosition());
  f2c::Visualizer::plot(path);
  f2c::Visualizer::show();
  return 0;
}
```

```cmake
find_package(Fields2Cover REQUIRED)
target_link_libraries(<your_target> Fields2Cover)
```

### Python

```bash
pip install fields2cover
```

```python
import fields2cover as f2c

field = f2c.Parser().importFieldGml("data/test1.xml")
robot = f2c.Robot(2.0, 6.0, 0.5, 0.2)

path = f2c.planCovPath(robot, field, False)

f2c.Visualizer.figure()
f2c.Visualizer.plot(field.getCellsAbsPosition())
f2c.Visualizer.plot(path)
f2c.Visualizer.show()
```

## Installation

- **Python**: `pip install fields2cover` — the package is built from source on your machine, so the system dependencies above must be installed.
- **C++ / from source**: clone this repository and build with CMake — full walkthrough in the [installation guide](https://fields2cover.github.io/source/installation.html).
- **ROS 2**: see [opennav_coverage](https://github.com/open-navigation/opennav_coverage), a Nav2-compatible coverage task server built on Fields2Cover.

## Documentation

The extended documentation lives at [fields2cover.github.io](https://fields2cover.github.io):

- [Tutorials](https://fields2cover.github.io/source/tutorials.html) — step through every module of the pipeline, in C++ and Python
- [API reference](https://fields2cover.github.io/api/f2c_library.html)
- [Migration guide v1 → v2](https://fields2cover.github.io/source/migration_to_v2.html)
- [FAQ](https://fields2cover.github.io/source/faq.html)

## Contributing

If you find an issue, a bug or have a proposal, [open an issue](https://github.com/Fields2Cover/Fields2Cover/issues). Pull requests are more than welcome; for major changes, please open an issue first to discuss what you would like to change, and make sure to update tests as appropriate. A one-click development environment is available:

<p>
  <a href="https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=501080115"><img src="https://github.com/codespaces/badge.svg" alt="Open in GitHub Codespaces"></a>
</p>

## Citing

Please cite [this paper](https://ieeexplore.ieee.org/document/10050562) when using Fields2Cover for your research:

```bibtex
@article{Mier_Fields2Cover_An_open-source_2023,
  author={Mier, Gonzalo and Valente, João and de Bruin, Sytze},
  journal={IEEE Robotics and Automation Letters},
  title={Fields2Cover: An Open-Source Coverage Path Planning Library for Unmanned Agricultural Vehicles},
  year={2023},
  volume={8},
  number={4},
  pages={2166-2172},
  doi={10.1109/LRA.2023.3248439}
}
```

## License & credits

Fields2Cover is released under the [BSD-3 license](https://github.com/Fields2Cover/Fields2Cover/blob/main/LICENSE). It stands on the shoulders of [GDAL](https://gdal.org/), [OR-tools](https://developers.google.com/optimization), [steering_functions](https://github.com/hbanzhaf/steering_functions), [nlohmann/json](https://github.com/nlohmann/json/), [tinyxml2](https://github.com/leethomason/tinyxml2), [CubicSplineClass](https://github.com/joshhooker/CubicSplineClass) and [matplotplusplus](https://github.com/alandefreitas/matplotplusplus). The project is (partly) financed by the Dutch Research Council (NWO).
