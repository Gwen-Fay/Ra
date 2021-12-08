#!/bin/bash

#SHADER BUILD
err=false

for f in libgaem/shaders/*.frag; 
do
    glslc $f -o "${f}.spv" || err=true
done

for f in libgaem/shaders/*.vert; 
do
    glslc $f -o "${f}.spv" || err=true
done

if [ "$err" = true ] ; then
    exit 1
fi

mkdir -p libgaem/shaders/build_shaders
mv libgaem/shaders/*.spv libgaem/shaders/build_shaders/

echo "......................"
echo "  ~Testing Gaem Lib~  "
echo "......................"

# TEST BUILD LIBRARY
touch CMakeLists.txt
cmake -S . -B build/LinTest -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Test -D PLATFORM="LINUX" || exit 1
make -C build/LinTest || exit 1
build/LinTest/tests || exit 1

echo "-----------------------"
echo "  ~Building Gaem Lib~  "
echo "-----------------------"

#DEBUG BUILD LIBRARY
cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C build/LinDebug || exit 1
mkdir -p bin/LinDebug
cp build/LinDebug/libGaem.so bin/LinDebug/libGaem.so
rm -rf bin/LinDebug/shaders
mv libgaem/shaders/build_shaders bin/LinDebug/shaders
rm -rf libgaem/shaders/build_shaders

rm -f ../demoGaem/lib/libGaem.so
cp bin/LinDebug/libGaem.so ../demogaem/lib/libGaem.so

echo "========================"
echo "  ~building GaemDebug~  "
echo "========================"
cd ../demogaem

#DEBUG BUILD DEMO
cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C build/LinDebug || exit 1

rm -rf build/LinDebug/shaders
cp -r ../libgaem/bin/LinDebug/shaders build/LinDebug/shaders

cd build/LinDebug
lldb Demo -o r

exit 0
