cmake_minimum_required(VERSION 3.18.4)

project(Gaem VERSION 0.0
    DESCRIPTION "Its a Gaem"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

file(GLOB_RECURSE SRCS src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRCS})

add_subdirectory(vendor/spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC vendor/spdlog/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE)
endif()

