#!/bin/bash

cmake -S . -B build/WinRelease -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix\
    -D CMAKE_BUILD_TYPE=Release || exit 1
make -C build/WinRelease || exit 1
cp build/WinRelease/Gaem.exe bin/WinRelease/Gaem.exe
