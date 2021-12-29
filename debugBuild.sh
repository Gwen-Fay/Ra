#!/bin/bash

echo "-----------------------"
echo "  ~Building LibGaem~  "
echo "-----------------------"

#DEBUG BUILD LIBRARY
cmake -S libgaem -B libgaem/build/LinDebug -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C libgaem/build/LinDebug || exit 1
cd libgaem/build/LinDebug
ctest || exit 1
cd ../../..

rm -rf libgaem/bin
mkdir -p libgaem/bin/LinDebug
cp libgaem/build/LinDebug/libGaem.so libgaem/bin/LinDebug/libGaem.so
cp -R libgaem/build/LinDebug/shaders libgaem/bin/LinDebug/shaders

rm -f demoGaem/lib/libGaem.so
cp libgaem/bin/LinDebug/libGaem.so demogaem/lib/libGaem.so

echo "========================"
echo "  ~Building DemoGaem~  "
echo "========================"

#DEBUG BUILD DEMO
cmake -S demogaem -B demogaem/build/LinDebug -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C demogaem/build/LinDebug || exit 1

cp -R libgaem/bin/LinDebug/shaders demogaem/build/LinDebug/shaders

cd demogaem/build/LinDebug
lldb Demo -o r

exit 0
