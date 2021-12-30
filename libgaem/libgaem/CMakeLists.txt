# handles glslc
add_subdirectory(shaders)

if(CICD STREQUAL "TRUE")
  #Github Ubuntu environment does not have glslc
else()
  # src folders
  add_subdirectory(render)
endif()

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCS ${SRCS}
  ${DIR}/libgaem.hpp
  ${DIR}/core.hpp
  ${DIR}/log.hpp
  ${DIR}/application.hpp ${DIR}/application.cpp
  PARENT_SCOPE)

# Note - test.cpp is included in root CMakeLists if testing

