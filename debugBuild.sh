#!/bin/bash

#SHADER BUILD
err=false

for f in libgaem/libgaem/shaders/*.frag; 
do
    glslc $f -o "${f}.spv" || err=true
done

for f in libgaem/libgaem/shaders/*.vert; 
do
    glslc $f -o "${f}.spv" || err=true
done

if [ "$err" = true ] ; then
    exit 1
fi

mkdir -p libgaem/libgaem/shaders/build_shaders
mv libgaem/libgaem/shaders/*.spv libgaem/libgaem/shaders/build_shaders/

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

mkdir -p libgaem/bin/LinDebug
cp libgaem/build/LinDebug/libGaem.so libgaem/bin/LinDebug/libGaem.so
rm -rf libgaem/bin/LinDebug/shaders
mv libgaem/libgaem/shaders/build_shaders libgaem/bin/LinDebug/shaders
rm -rf libgaem/libgaem/shaders/build_shaders

rm -f demoGaem/lib/libGaem.so
cp libgaem/bin/LinDebug/libGaem.so demogaem/lib/libGaem.so

echo "========================"
echo "  ~Building DemoGaem~  "
echo "========================"

#DEBUG BUILD DEMO
cmake -S demogaem -B demogaem/build/LinDebug -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C demogaem/build/LinDebug || exit 1

rm -rf demogaem/build/LinDebug/shaders
cp -r libgaem/bin/LinDebug/shaders demogaem/build/LinDebug/shaders

cd demogaem/build/LinDebug
lldb Demo -o r

exit 0
