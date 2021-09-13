#!/bin/bash

cmake -S . -B build/WinDebug -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix\
    -D CMAKE_BUILD_TYPE=Debug
make -C build/WinDebug
cp build/WinDebug/Gaem.exe bin/WinDebug/Gaem.exe

echo "=================="
echo "  ~Running Gaem~  "
echo "=================="
wine bin/WinDebug/Gaem.exe
