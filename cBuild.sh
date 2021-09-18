#!/bin/bash

cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Debug || exit 1
make -C build/LinDebug || exit 1
cp build/LinDebug/Gaem bin/LinDebug/Gaem

echo "=================="
echo "  ~Running Gaem~  "
echo "=================="
bin/LinDebug/Gaem
