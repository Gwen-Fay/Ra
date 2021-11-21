#!/bin/bash

#SHADER BUILD
err=false

for f in src/shaders/*.frag; 
do
    glslc $f -o "${f}.spv" || err=true
done

for f in src/shaders/*.vert; 
do
    glslc $f -o "${f}.spv" || err=true
done

if [ "$err" = true ] ; then
    exit 1
fi

mkdir -p src/shaders/build_shaders
mv src/shaders/*.spv src/shaders/build_shaders/

echo ".................."
echo "  ~Testing Gaem~  "
echo ".................."

# TEST BUILD
cmake -S . -B build/LinTest -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Test -D PLATFORM="LINUX" || exit 1
make -C build/LinTest || exit 1
build/LinTest/tests || exit 1

echo "-------------------"
echo "  ~Building Gaem~  "
echo "-------------------"

#DEBUG BUILD
cmake -S . -B build/LinDebug -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C build/LinDebug || exit 1
mkdir -p bin/LinDebug
cp build/LinDebug/Gaem bin/LinDebug/Gaem
rm -rf bin/LinDebug/shaders
mv src/shaders/build_shaders bin/LinDebug/shaders
rm -rf src/shaders/build_shaders

echo "=================="
echo "  ~Running Gaem~  "
echo "=================="
cd bin/LinDebug
gdb Gaem -ex 'run'

exit 0
