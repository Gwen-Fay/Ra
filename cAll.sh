#!/bin/bash

#WinDebug
cmake -S . -B build/WinDebug -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix\
    -D CMAKE_BUILD_TYPE=Debug || exit 1
make -C build/WinDebug || exit 1
cp build/WinDebug/Gaem.exe bin/WinDebug/Gaem.exe

#WinRelease
cmake -S . -B build/WinRelease -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix\
    -D CMAKE_BUILD_TYPE=Release || exit 1
make -C build/WinRelease || exit 1
cp build/WinRelease/Gaem.exe bin/WinRelease/Gaem.exe

#LinRelease
cmake -S . -B build/LinRelease -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Release || exit 1
make -C build/LinRelease || exit 1
cp build/LinRelease/Gaem bin/LinRelease/Gaem

#LinDebug
cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Debug || exit 1
make -C build/LinDebug || exit 1
cp build/LinDebug/Gaem bin/LinDebug/Gaem
