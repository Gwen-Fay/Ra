#!/bin/bash

cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Debug
make -C build/LinDebug
cp build/LinDebug/Gaem bin/LinDebug/Gaem

echo "=================="
echo "  ~Running Gaem~  "
echo "=================="
bin/LinDebug/Gaem
