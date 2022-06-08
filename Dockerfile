FROM ubuntu:20.04

LABEL NAME="fields2cover" \
      VERSION="1.0.0" \
      DESC="Fields2Cover is a complete coverage path planning package for autonomous robots" \
      MAINTAINER="Gonzalo Mier"


ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:ubuntugis/ppa && \
      apt-get -y update && \
      apt-get install -y --no-install-recommends \
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
                    && \
                    apt-get autoclean && \
                    apt-get autoremove && \
                    apt-get clean && \
                    rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install gcovr


COPY . /workspace/fields2cover
RUN rm -rf /workspace/fields2cover/build 
WORKDIR /workspace/fields2cover
RUN mkdir badges build coverage
RUN cmake -Bbuild -DBUILD_CPP=ON \
                  -DBUILD_TUTORIALS=OFF \
                  -DBUILD_TESTS=ON \
                  -DBUILD_DOC=ON \
                  -DUSING_MATPLOTLIB=OFF \
                  -DCMAKE_BUILD_TYPE=Coverage .

#RUN cd build & make -j8



