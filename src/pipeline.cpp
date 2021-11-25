#include "pipeline.hpp"
#include "model.hpp"

#include "log.h"

#include <cassert>
#include <fstream>
#include <stdexcept>
#include <string>

namespace gaem {

Pipeline::Pipeline(Device &device, const std::string &vertName,
                   const std::string &fragName, const PipelineConfig &config)
    : device{device} {

  createGraphicsPipeline(SHADER_PATH + vertName + SHADER_EXT,
                         SHADER_PATH + fragName + SHADER_EXT, config);
}

Pipeline::~Pipeline() {
  vkDestroyShaderModule(device.device(), vertShaderModule, nullptr);
  vkDestroyShaderModule(device.device(), fragShaderModule, nullptr);
  vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
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
                                      const std::string &fragPath,
                                      const PipelineConfig &config) {
  assert(
      config.pipelineLayout != VK_NULL_HANDLE &&
      "Cannot create graphics pipeline::no pipelineLayout provided in config");
  assert(config.renderPass != VK_NULL_HANDLE &&
         "Cannot create graphics pipeline:: no renderPass provided in config");

  auto vertCode = readFile(vertPath);
  auto fragCode = readFile(fragPath);

  createShaderModule(vertCode, &vertShaderModule);
  createShaderModule(fragCode, &fragShaderModule);

  VkPipelineShaderStageCreateInfo shaderStages[2];
  shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  shaderStages[0].module = vertShaderModule;
  shaderStages[0].pName = "main";
  shaderStages[0].flags = 0;
  shaderStages[0].pNext = nullptr;
  shaderStages[0].pSpecializationInfo = nullptr;

  shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  shaderStages[1].module = fragShaderModule;
  shaderStages[1].pName = "main";
  shaderStages[1].flags = 0;
  shaderStages[1].pNext = nullptr;
  shaderStages[1].pSpecializationInfo = nullptr;

  auto bindingDescriptions = Model::Vertex::getBindingDescriptions();
  auto attributeDescriptions = Model::Vertex::getAtributeDescriptions();
  VkPipelineVertexInputStateCreateInfo vertInputInfo{};
  vertInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertInputInfo.vertexAttributeDescriptionCount =
      static_cast<uint32_t>(attributeDescriptions.size());
  vertInputInfo.vertexBindingDescriptionCount =
      static_cast<uint32_t>(bindingDescriptions.size());
  vertInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
  vertInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertInputInfo;
  pipelineInfo.pInputAssemblyState = &config.inputAssemblyInfo;
  pipelineInfo.pViewportState = &config.viewportInfo;
  pipelineInfo.pRasterizationState = &config.rasterizationInfo;

  pipelineInfo.pColorBlendState = &config.colorBlendInfo;
  pipelineInfo.pDepthStencilState = &config.depthStencilInfo;
  pipelineInfo.pDynamicState = nullptr;
  pipelineInfo.pMultisampleState = &config.multisampleInfo;
  pipelineInfo.layout = config.pipelineLayout;
  pipelineInfo.renderPass = config.renderPass;
  pipelineInfo.subpass = config.subpass;

  pipelineInfo.basePipelineIndex = -1;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1,
                                &pipelineInfo, nullptr,
                                &graphicsPipeline) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to create graphics pipeline!");
    throw std::runtime_error("failed to create graphics pipeline");
  }
}

void Pipeline::createShaderModule(const std::vector<char> &code,
                                  VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(device.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to create shader module!");
    throw std::runtime_error("failed to create shader module");
  }
}

void Pipeline::bind(VkCommandBuffer commandBuffer) {
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    graphicsPipeline);
}

void Pipeline::defaultPipelineConfig(PipelineConfig &config, uint32_t width,
                                     uint32_t height) {
  config.inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  config.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  config.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  config.viewport.x = 0.0f;
  config.viewport.y = 0.0f;
  config.viewport.width = static_cast<float>(width);
  config.viewport.height = static_cast<float>(height);
  config.viewport.minDepth = 0.0f;
  config.viewport.maxDepth = 1.0f;

  config.scissor.offset = {0, 0};
  config.scissor.extent = {width, height};

  config.viewportInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  config.viewportInfo.viewportCount = 1;
  config.viewportInfo.pViewports = &config.viewport;
  config.viewportInfo.scissorCount = 1;
  config.viewportInfo.pScissors = &config.scissor;

  config.rasterizationInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  config.rasterizationInfo.depthClampEnable = VK_FALSE;
  config.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  config.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  config.rasterizationInfo.lineWidth = 1.0f;
  config.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
  config.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
  config.rasterizationInfo.depthBiasEnable = VK_FALSE;
  config.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
  config.rasterizationInfo.depthBiasClamp = 0.0f;          // Optional
  config.rasterizationInfo.depthBiasSlopeFactor = 0.0f;    // Optional

  config.multisampleInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  config.multisampleInfo.sampleShadingEnable = VK_FALSE;
  config.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  config.multisampleInfo.minSampleShading = 1.0f;          // Optional
  config.multisampleInfo.pSampleMask = nullptr;            // Optional
  config.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
  config.multisampleInfo.alphaToOneEnable = VK_FALSE;      // Optional

  config.colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  config.colorBlendAttachment.blendEnable = VK_FALSE;
  config.colorBlendAttachment.srcColorBlendFactor =
      VK_BLEND_FACTOR_ONE; // Optional
  config.colorBlendAttachment.dstColorBlendFactor =
      VK_BLEND_FACTOR_ZERO;                                   // Optional
  config.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
  config.colorBlendAttachment.srcAlphaBlendFactor =
      VK_BLEND_FACTOR_ONE; // Optional
  config.colorBlendAttachment.dstAlphaBlendFactor =
      VK_BLEND_FACTOR_ZERO;                                   // Optional
  config.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

  config.colorBlendInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  config.colorBlendInfo.logicOpEnable = VK_FALSE;
  config.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
  config.colorBlendInfo.attachmentCount = 1;
  config.colorBlendInfo.pAttachments = &config.colorBlendAttachment;
  config.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
  config.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
  config.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
  config.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

  config.depthStencilInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  config.depthStencilInfo.depthTestEnable = VK_TRUE;
  config.depthStencilInfo.depthWriteEnable = VK_TRUE;
  config.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
  config.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
  config.depthStencilInfo.minDepthBounds = 0.0f; // Optional
  config.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
  config.depthStencilInfo.stencilTestEnable = VK_FALSE;
  config.depthStencilInfo.front = {}; // Optional
  config.depthStencilInfo.back = {};  // Optional
}

} // namespace gaem
