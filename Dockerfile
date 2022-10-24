FROM ubuntu:20.04

LABEL NAME="fields2cover" \
      VERSION="1.2.0" \
      DESC="Fields2Cover is a complete coverage path planning package for autonomous robots" \
      MAINTAINER="Gonzalo Mier"


ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -y update
RUN apt-get install -y --no-install-recommends apt-utils software-properties-common 
RUN add-apt-repository -y ppa:ubuntugis/ppa
RUN apt-get -y update
RUN apt-get install -y --no-install-recommends \
                    build-essential \
                    ca-certificates \
                    cmake \
                    doxygen \
                    g++ \
                    git \
                    libeigen3-dev \
                    libgdal-dev \
                    libpython3-dev \
                    python3 \
                    python3-pip \
                    python3-matplotlib \
                    lcov \
                    libgtest-dev \
                    libtbb-dev \
                    swig \
                    && \
                    apt-get autoclean && \
                    apt-get autoremove && \
                    apt-get clean && \
                    rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install gcovr


COPY . /workspace/fields2cover
RUN rm -rf /workspace/fields2cover/build && mkdir /workspace/fields2cover/build
WORKDIR /workspace/fields2cover/build

RUN cmake -DBUILD_CPP=ON \
          -DBUILD_PYTHON=ON \
          -DBUILD_TUTORIALS=OFF \
          -DBUILD_TESTS=ON \
          -DBUILD_DOC=OFF \
          -DCMAKE_BUILD_TYPE=Release ..
RUN make -j8
RUN make install



