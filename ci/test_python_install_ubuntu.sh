#!/usr/bin/env bash
# Install the fields2cover python package on the osgeo/gdal ubuntu image
# against system dependencies and run tests/python. Used by CI and locally:
#   docker run --rm -v "$PWD":/work -w /work osgeo/gdal:ubuntu-full-3.6.3 \
#       bash ci/test_python_install_ubuntu.sh [target]
# target: "." (default) builds and installs a wheel from the checkout;
#         a path to an sdist or wheel installs that file.
set -euo pipefail

TARGET="${1:-.}"
ARCH="$(uname -m)"

export DEBIAN_FRONTEND=noninteractive
apt-get update --allow-insecure-repositories -y
apt-get install -y --allow-unauthenticated --no-install-recommends \
  ca-certificates build-essential git wget \
  libboost-dev libeigen3-dev libgeos-dev libtbb-dev libtinyxml2-dev nlohmann-json3-dev \
  python3 python3-dev python3-pip python3-venv

# or-tools release tarball, same version as cmake/F2CUtils.cmake
case "$ARCH" in
  x86_64)  ORTOOLS_URL=https://github.com/google/or-tools/releases/download/v9.9/or-tools_amd64_ubuntu-22.04_cpp_v9.9.3963.tar.gz ;;
  aarch64) ORTOOLS_URL=https://github.com/google/or-tools/releases/download/v9.9/or-tools_arm64_debian-11_cpp_v9.9.3963.tar.gz ;;
  *) echo "unsupported arch $ARCH" >&2; exit 1 ;;
esac
if [[ ! -d /opt/ortools/lib ]]; then
  wget -q -O /tmp/ortools.tar.gz "$ORTOOLS_URL"
  mkdir -p /opt/ortools
  tar -zxf /tmp/ortools.tar.gz -C /opt/ortools --strip-components=1
fi
export CMAKE_PREFIX_PATH=/opt/ortools
export LD_LIBRARY_PATH=/opt/ortools/lib

VENV=/tmp/f2c-venv
python3 -m venv "$VENV"
"$VENV/bin/pip" install --upgrade pip
# No system cmake needed: scikit-build-core fetches one from PyPI if it is missing or < 3.18.

if [[ "$TARGET" == "." ]]; then
  "$VENV/bin/pip" install build
  rm -rf /tmp/f2c-dist
  "$VENV/bin/python" -m build --wheel --outdir /tmp/f2c-dist .
  "$VENV/bin/pip" install /tmp/f2c-dist/*.whl
else
  "$VENV/bin/pip" install "$TARGET"
fi

"$VENV/bin/pip" install pytest numpy
"$VENV/bin/python" -c "import fields2cover as f2c; print('fields2cover', f2c.__version__)"
"$VENV/bin/python" -m pytest tests/python -q
