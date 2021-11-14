/**
 *
 * Pipeline
 *
 * vulkan render pipeline abstraction
 *
 * TODO actually use shaders it reads from
 */

#pragma once
#include <string>
#include <vector>

namespace gaem {

class Pipeline {
public:
  Pipeline(const std::string &vertName, const std::string &fragName);

private:
  static std::vector<char> readFile(const std::string &filePath);
  void createGraphicsPipeline(const std::string &vertPath,
                              const std::string &fragPath);

  const std::string SHADER_PATH = "shaders/";
  const std::string SHADER_EXT = ".spv";
};

} // namespace gaem
