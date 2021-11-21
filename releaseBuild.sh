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

if ["$err" = true ] ; then
  exit 1
fi

mkdir -p src/shaders/build_shaders
mv src/shaders/*.spv src/shaders/build_shaders/

#RELEASE BUILD
cmake -S . -B build/LinRelease -D CMAKE_CXX_COMPILER=g++\
    -D CMAKE_BUILD_TYPE=Release -D PLATFORM="LINUX"|| exit 1
make -C build/LinRelease || exit 1
mkdir -p bin/LinRelease
cp build/LinRelease/Gaem bin/LinRelease/Gaem
rm bin/LinRelease/shaders
mv src/shaders/build_shaders bin/LinRelease/shaders
rm src/shaders/build_shaders
