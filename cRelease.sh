#!/bin/bash

cmake -S . -B build/LinRelease -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Release
make -C build/LinRelease
cp build/LinRelease/Gaem bin/LinRelease/Gaem
