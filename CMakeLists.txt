cmake_minimum_required(VERSION 3.18.4)

project(Gaem VERSION 0.0
    DESCRIPTION "Its a Gaem"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)

file(GLOB_RECURSE SRCS src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRCS})

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)

target_include_directories(${PROJECT_NAME} PUBLIC vendor/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC vendor/glfw/include)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} vulkan)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

    add_definitions(-DDEBUG -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE -02)
endif()

