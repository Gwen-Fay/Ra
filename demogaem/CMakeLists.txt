file(GLOB_RECURSE SRCS demogaem/*.cpp demogaem/*.hpp)
add_executable(Demo ${SRCS})

target_include_directories(Demo PUBLIC ${CMAKE_SOURCE_DIR}/libgaem)

target_link_libraries(Demo ${PROJECT_NAME})
