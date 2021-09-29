#!/bin/bash

# TEST BUILD
cmake -S . -B build/LinTest -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Test -D PLATFORM="LINUX" || exit 1
make -C build/LinTest || exit 1
build/LinTest/tests || exit 1

#DEBUG BUILD
cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C build/LinDebug || exit 1
cp build/LinDebug/Gaem bin/LinDebug/Gaem

echo "=================="
echo "  ~Running Gaem~  "
echo "=================="
gdb bin/LinDebug/Gaem -ex 'run'
