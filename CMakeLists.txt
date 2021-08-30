cmake_minimum_required(VERSION 3.18.4)

project(Gaem VERSION 0.0
    DESCRIPTION "Its a Gaem"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)
