#!/bin/bash

echo "-----------------------"
echo "  ~Building LibGaem~  "
echo "-----------------------"
cmake -S . -B build -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C build || exit 1
cd build/libgaem
ctest --test-dir ../bin  || exit 1
cd ../bin
lldb Demo -o r
exit 0
