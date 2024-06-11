FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    cmake \
    pkg-config \
    ninja-build \
    build-essential \
    libeigen3-dev \
    g++ \
    gcc \
    python3-dev \
    python3-numpy \
    graphviz \
    libgraphviz-dev \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /code

COPY . /code/
CMD ["/bin/bash"]