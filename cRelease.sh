#!/bin/bash

cmake -S . -B build/LinRelease -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Release || exit 1
make -C build/LinRelease || exit 1
cp build/LinRelease/Gaem bin/LinRelease/Gaem
