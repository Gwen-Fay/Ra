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

echo "......................"
echo "  ~Testing Gaem Lib~  "
echo "......................"

# TEST BUILD LIBRARY
touch libgaem/CMakeLists.txt
cmake -S libgaem -B libgaem/build/LinTest -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Test -D PLATFORM="LINUX" || exit 1
make -C libgaem/build/LinTest || exit 1
libgaem/build/LinTest/tests || exit 1

echo "-----------------------"
echo "  ~Building Gaem Lib~  "
echo "-----------------------"

#DEBUG BUILD LIBRARY
cmake -S libgaem -B libgaem/build/LinDebug -D CMAKE_CXX_COMPILER=clang++\
    -D CMAKE_BUILD_TYPE=Debug -D PLATFORM="LINUX" || exit 1
make -C libgaem/build/LinDebug || exit 1
mkdir -p libgaem/bin/LinDebug
cp libgaem/build/LinDebug/libGaem.so libgaem/bin/LinDebug/libGaem.so
rm -rf libgaem/bin/LinDebug/shaders
mv libgaem/libgaem/shaders/build_shaders libgaem/bin/LinDebug/shaders
rm -rf libgaem/libgaem/shaders/build_shaders

rm -f demoGaem/lib/libGaem.so
cp libgaem/bin/LinDebug/libGaem.so demogaem/lib/libGaem.so

echo "========================"
echo "  ~building GaemDebug~  "
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
