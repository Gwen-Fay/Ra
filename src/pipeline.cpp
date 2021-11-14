#include "pipeline.hpp"

#include "log.h"

#include <fstream>
#include <stdexcept>
#include <string>

namespace gaem {

Pipeline::Pipeline(const std::string &vertName, const std::string &fragName) {
  createGraphicsPipeline(SHADER_PATH + vertName + SHADER_EXT,
                         SHADER_PATH + fragName + SHADER_EXT);
}

std::vector<char> Pipeline::readFile(const std::string &filePath) {

  std::ifstream file{filePath, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    LOG_CRITICAL("Failed to open Shader: " + filePath);
    throw std::runtime_error("Failed to open File: " + filePath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;
}

void Pipeline::createGraphicsPipeline(const std::string &vertPath,
                                      const std::string &fragPath) {
  auto vertCode = readFile(vertPath);
  auto fragCode = readFile(fragPath);

  LOG_DEBUG("Vert Shader Code Size: " + std::to_string(vertCode.size()));
  LOG_DEBUG("Frag Shader Code Size: " + std::to_string(fragCode.size()));
}

} // namespace gaem
