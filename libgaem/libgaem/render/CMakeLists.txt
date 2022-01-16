set(DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCS ${SRCS}
  ${DIR}/renderer.hpp ${DIR}/renderer.cpp
  ${DIR}/renderSystem.hpp ${DIR}/renderSystem.cpp
  ${DIR}/swapChain.hpp ${DIR}/swapChain.cpp
  ${DIR}/window.hpp ${DIR}/window.cpp
  ${DIR}/graphicsPipeline.hpp ${DIR}/graphicsPipeline.cpp
  ${DIR}/device.hpp ${DIR}/device.cpp
  ${DIR}/model.hpp ${DIR}/model.cpp
  PARENT_SCOPE)
