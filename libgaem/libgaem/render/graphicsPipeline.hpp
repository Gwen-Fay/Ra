/**
 *
 * Pipeline
 *
 * vulkan render pipeline abstraction
 */

#pragma once
#include <libgaem/render/device.hpp>

#include <string>
#include <vector>

namespace gaem {

struct GraphicsPipelineConfig {
  GraphicsPipelineConfig(const GraphicsPipelineConfig &) = delete;
  GraphicsPipelineConfig &operator=(const GraphicsPipelineConfig) = delete;
  GraphicsPipelineConfig() = default;

  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class GaemGraphicsPipeline {
public:
  GaemGraphicsPipeline(GaemDevice &gaemDevice, const std::string &vertName,
                       const std::string &fragName,
                       const GraphicsPipelineConfig &config);
  ~GaemGraphicsPipeline();

  GaemGraphicsPipeline(const GaemGraphicsPipeline &) = delete;
  void operator=(const GaemGraphicsPipeline &) = delete;

  void bind(VkCommandBuffer commandBuffer);

  static void defaultGraphicsPipelineConfigInfo(GraphicsPipelineConfig &config);

private:
  static std::vector<char> readFile(const std::string &filePath);
  void createGraphicsPipeline(const std::string &vertPath,
                              const std::string &fragPath,
                              const GraphicsPipelineConfig &config);

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
