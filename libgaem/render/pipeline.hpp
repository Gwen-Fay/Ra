/**
 *
 * Pipeline
 *
 * vulkan render pipeline abstraction
 *
 * TODO actually use shaders it reads from
 */

#pragma once
#include <libgaem/render/device.hpp>

#include <string>
#include <vector>

namespace gaem {

struct PipelineConfig {
  PipelineConfig(const PipelineConfig &) = delete;
  PipelineConfig &operator=(const PipelineConfig) = delete;
  PipelineConfig() = default;

  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class GaemPipeline {
public:
  GaemPipeline(GaemDevice &gaemDevice, const std::string &vertName,
               const std::string &fragName, const PipelineConfig &config);
  ~GaemPipeline();

  GaemPipeline(const GaemPipeline &) = delete;
  void operator=(const GaemPipeline &) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static void defaultPipelineConfig(PipelineConfig &config, uint32_t width,
                                    uint32_t height);

private:
  static std::vector<char> readFile(const std::string &filePath);
  void createGraphicsPipeline(const std::string &vertPath,
                              const std::string &fragPath,
                              const PipelineConfig &config);

  void createShaderModule(const std::vector<char> &code,
                          VkShaderModule *shaderModule);

  const std::string SHADER_PATH = "shaders/";
  const std::string SHADER_EXT = ".spv";

  GaemDevice &gaemDevice;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};

} // namespace gaem
