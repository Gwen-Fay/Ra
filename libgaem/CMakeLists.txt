cmake_minimum_required(VERSION 3.18.4)

project(Gaem VERSION 0.0
    DESCRIPTION "Its a Gaem"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Vulkan REQUIRED COMPONENTS glslc FATAL_ERROR)
find_package(glm REQUIRED FATAL_ERROR)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glfw)

#grab libs and shaders
set(SRCS "")
set(SHADERS "")
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

#Shaders
function(add_shader TARGET SHADER)
    find_program(GLSLC glslc)

    set(current-shader-path libgaem/shaders/${SHADER})
    set(current-output-path ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv)

    # Add a custom command to compile GLSL to SPIR-V.
    get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})

    add_custom_command(
           OUTPUT ${current-output-path}
           COMMAND ${GLSLC} -o ${current-output-path} ${current-shader-path}
           DEPENDS ${current-shader-path}
           IMPLICIT_DEPENDS CXX ${current-shader-path}
           VERBATIM
           WORKING_DIRECTORY ../..)

    # Make sure our build depends on this output.
    set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_shader)

add_shader(${PROJECT_NAME} example.vert)
add_shader(${PROJECT_NAME} example.frag)

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
