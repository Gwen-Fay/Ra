cmake_minimum_required(VERSION 3.18.4)

project(Demo VERSION 0.0
  DESCRIPTION "Its a Demo of Gaem"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)


file(GLOB_RECURSE SRCS demogaem/*.cpp demogaem/*.hpp)
add_executable(Demo ${SRCS})

target_include_directories(Demo PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../libgaem)
target_include_directories(Demo PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../libgaem/vendor/spdlog/include)

target_link_libraries(Demo ${CMAKE_CURRENT_LIST_DIR}/lib/libGaem.so)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG -O0 -g)
else()
  add_definitions(-DRELEASE -O2)
endif()
