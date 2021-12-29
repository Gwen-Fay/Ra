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

set(SRCS "")

#grab libs and handle shaders
add_subdirectory(libgaem)

add_library(${PROJECT_NAME} SHARED ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}) 

target_include_directories(${PROJECT_NAME} PUBLIC vendor/spdlog/include)
target_include_directories(${PROJECT_NAME} PUBLIC vendor/glfw/include)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} vulkan)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
  add_definitions(-DDEBUG -DBUILD -O0 -g)
else()
  add_definitions(-DRELEASE -DBUILD -O2)
endif()

if(PLATFORM STREQUAL "WINDOWS")
    add_definitions(-DWINDOWS)
elseif(PLATFORM STREQUAL "LINUX")
    add_definitions(-DLINUX)
endif()

#Testing
enable_testing()

add_executable(tester libgaem/test.cpp ${SRCS})
target_include_directories(tester PUBLIC ${CMAKE_SOURCE_DIR}) 
target_include_directories(tester PUBLIC vendor/spdlog/include)
target_include_directories(tester PUBLIC vendor/glfw/include)

target_link_libraries(tester glfw ${GLFW_LIBRARIES})
target_link_libraries(tester vulkan)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(tester spdlog::spdlog)
  add_definitions(-DDEBUG -DBUILD -O0 -g)
else()
  add_definitions(-DRELEASE -DBUILD -O2)
endif()

if(PLATFORM STREQUAL "WINDOWS")
    add_definitions(-DWINDOWS)
elseif(PLATFORM STREQUAL "LINUX")
    add_definitions(-DLINUX)
endif()

add_test(Tester tester)
