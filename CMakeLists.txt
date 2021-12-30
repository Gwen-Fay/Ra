cmake_minimum_required(VERSION 3.18.4)

project(Gaem VERSION 0.0
    DESCRIPTION "Its a Gaem"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

if(CICD STREQUAL "TRUE")

else()
find_package(Vulkan REQUIRED COMPONENTS glslc FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)
endif()

add_subdirectory(vendor/glfw)
include_directories(vendor/spdlog/include)

link_libraries(glfw ${GLFW_LIBRARIES})
link_libraries(vulkan)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_subdirectory(vendor/spdlog)
  include_directories(venor/spdlog/include)
  add_definitions(-DDEBUG -O0 -g)
else()
  add_definitions(-DRELEASE -O2)
endif()

if(PLATFORM STREQUAL "WINDOWS")
  add_definitions(-DWINDOWS)
elseif(PLATFORM STREQUAL "LINUX")
  add_definitions(-DLINUX)
endif()

#Compile and test libgaem
add_subdirectory(libgaem)

#compile demogaem
add_subdirectory(demogaem)
