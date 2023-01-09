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
                    swig

RUN python3 -m pip install gcovr




