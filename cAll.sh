#!/bin/bash

#WinDebug
cmake -S . -B build/WinDebug -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix\
    -D CMAKE_BUILD_TYPE=Debug
make -C build/WinDebug
cp build/WinDebug/Gaem.exe bin/WinDebug/Gaem.exe

#WinRelease
cmake -S . -B build/WinRelease -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix\
    -D CMAKE_BUILD_TYPE=Release
make -C build/WinRelease
cp build/WinRelease/Gaem.exe bin/WinRelease/Gaem.exe

#LinRelease
cmake -S . -B build/LinRelease -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Release
make -C build/LinRelease
cp build/LinRelease/Gaem bin/LinRelease/Gaem

#LinDebug
cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Debug
make -C build/LinDebug
cp build/LinDebug/Gaem bin/LinDebug/Gaem
