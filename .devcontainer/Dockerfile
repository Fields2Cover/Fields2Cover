FROM osgeo/gdal:ubuntu-full-3.6.3

LABEL NAME="fields2cover" \
      VERSION="2.0.0" \
      DESC="Fields2Cover is a complete coverage path planning package for autonomous robots" \
      MAINTAINER="Gonzalo Mier"


ENV DEBIAN_FRONTEND noninteractive

WORKDIR /workspaces/
RUN mkdir -p /usr/include/new_gdal && \
    cp -r /usr/include/gdal* /usr/include/new_gdal/ && \
    cp /usr/include/ogr* /usr/include/new_gdal/ && \
    cp /usr/include/cpl* /usr/include/new_gdal/ && \
    mv /usr/include/new_gdal/ /usr/include/gdal/

RUN apt-get update --allow-insecure-repositories -y && \
    apt-get install -y --allow-unauthenticated --no-install-recommends ca-certificates


RUN if gdalinfo --version | grep -o " 3\.[0-2]\."; then \
      apt-get install wget && \
      wget https://github.com/Kitware/CMake/releases/download/v3.17.2/cmake-3.17.2-Linux-x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/cmake-install.sh; \
    else \
      apt install -y  --allow-unauthenticated --no-install-recommends cmake ; \ 
    fi

ENV PATH="/usr/bin/cmake/bin:${PATH}"


RUN apt-get install -y --allow-unauthenticated --no-install-recommends \
                    build-essential \
                    ca-certificates \
                    doxygen \
                    g++ \
                    git \
                    gnuplot \
                    lcov \
                    libboost-dev \
                    libgeos-dev \
                    libgtest-dev \
                    libtbb-dev \
                    libeigen3-dev \
                    libpython3-dev \
                    python3 \
                    python3-pip \
                    python3-matplotlib \
                    python3-pytest \
                    python3-tk \
                    ranger \
                    vim \
                    libtinyxml2-dev \
                    nlohmann-json3-dev
#                    && \
#                    apt-get autoclean && \
#                    apt-get autoremove && \
#                    apt-get clean && \
#                    rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install gcovr setuptools
RUN python3 -c "import matplotlib" && \
    echo "backend: Agg" > ~/.config/matplotlib/matplotlibrc

RUN apt-get install -y libgtest-dev \
    && cd /usr/src/gtest \
    && cmake CMakeLists.txt \
    && make \
    && (cp *.a /usr/lib/ 2>\dev\null || :) \
    && (cp lib/*.a /usr/lib/ 2>\dev\null || :)

RUN if gdalinfo --version | grep -o " 3\.[0-2]\."; then \
      apt-get install -y --no-install-recommends  --allow-unauthenticated autoconf automake autotools-dev libpcre2-dev bison \
      && git clone https://github.com/swig/swig.git \
      && cd swig \
      && ./autogen.sh \
      && ./configure \
      && make -j8 \
      && make install; \
    else \
      apt-get install -y --no-install-recommends  --allow-unauthenticated swig; \
    fi

RUN wget https://github.com/google/or-tools/releases/download/v9.9/or-tools_amd64_ubuntu-22.04_cpp_v9.9.3963.tar.gz -q -O /tmp/ortools.tar.gz \
    && mkdir -p /tmp/ortools \
    && tar -zxf /tmp/ortools.tar.gz -C /tmp/ortools --strip-components=1 \
    && cp -r /tmp/ortools/bin/. /usr/bin \
    && cp -r /tmp/ortools/include/. /usr/include \
    && cp -r /tmp/ortools/lib/. /usr/lib \
    && cp -r /tmp/ortools/lib/cmake/. /usr/share \
    && cp -r /tmp/ortools/share/. /usr/share/ortools

WORKDIR /workspaces/Fields2Cover




