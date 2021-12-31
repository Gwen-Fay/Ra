#grab libs and shaders
set(SRCS "")
set(SHADERS "")
add_subdirectory(libgaem)

if(CICD STREQUAL "TRUE")
  #Github ubuntu environment does not have glslc
  #also, don't build library, only tests
else() 
  
  add_library(${PROJECT_NAME} STATIC ${SRCS})

  target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

  #Shaders
  function(add_shader TARGET SHADER)
      find_program(GLSLC glslc)

      set(current-shader-path libgaem/shaders/${SHADER})
      set(current-output-path ${CMAKE_BINARY_DIR}/bin/shaders/${SHADER}.spv)

      # Add a custom command to compile GLSL to SPIR-V.
      get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
      file(MAKE_DIRECTORY ${current-output-dir})

      add_custom_command(
        OUTPUT ${current-output-path}
        COMMAND ${GLSLC} -o ${current-output-path} ${current-shader-path}
        DEPENDS ${current-shader-path}
        IMPLICIT_DEPENDS CXX ${current-shader-path}
        VERBATIM
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

      # Make sure our build depends on this output.
      set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
      target_sources(${TARGET} PRIVATE ${current-output-path})
  endfunction(add_shader)

  #TODO make this load from SHADERS
  add_shader(${PROJECT_NAME} example.vert)
  add_shader(${PROJECT_NAME} example.frag)
endif()

#Testing
enable_testing()

add_executable(tester libgaem/test.cpp ${SRCS})
target_include_directories(tester PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

set(TEST_BIN ${CMAKE_BINARY_DIR}/bin)
add_test(Tester ${TEST_BIN}/tester)
