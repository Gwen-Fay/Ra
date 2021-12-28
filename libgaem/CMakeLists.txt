cmake_minimum_required(VERSION 3.18.4)

project(Gaem VERSION 0.0
    DESCRIPTION "Its a Gaem"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/catch2)

if(CMAKE_BUILD_TYPE STREQUAL "Test")

    # Build gaem library test

    file(GLOB_RECURSE SRCS libgaem/*.cpp libgaem/*.hpp)
    add_executable(tests ${SRCS})
    target_include_directories(tests PUBLIC .) 

    target_include_directories(tests PUBLIC vendor/catch2/include)
    add_definitions(-O0)
    
    target_include_directories(tests PUBLIC vendor/spdlog/include)
    target_include_directories(tests PUBLIC vendor/glfw/include)

    target_link_libraries(tests glfw ${GLFW_LIBRARIES})
    target_link_libraries(tests vulkan)
else()

    # Build Gaem Library

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        file(GLOB_RECURSE SRCS libgaem/*.cpp libgaem/*.hpp)
        list(FILTER SRCS EXCLUDE REGEX "test.cpp$")
        add_library(${PROJECT_NAME} SHARED ${SRCS})
        target_include_directories(${PROJECT_NAME} PUBLIC .)
        
        target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
        add_definitions(-DDEBUG -DBUILD -O0 -g)
      else()
        file(GLOB_RECURSE SRCS libgaem/*.cpp libgaem/*.hpp)
        list(FILTER SRCS EXCLUDE REGEX "test.cpp$")
        add_library(${PROJECT_NAME} SHARED ${SRCS})
        target_include_directories(${PROJECT_NAME} PUBLIC .)
    
        add_definitions(-DRELEASE -DBUILD -O2)
    endif()

    target_include_directories(${PROJECT_NAME} PUBLIC vendor/spdlog/include)
    target_include_directories(${PROJECT_NAME} PUBLIC vendor/glfw/include)

    target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} vulkan)

endif()


if(PLATFORM STREQUAL "WINDOWS")
    add_definitions(-DWINDOWS)
elseif(PLATFORM STREQUAL "LINUX")
    add_definitions(-DLINUX)
endif()

