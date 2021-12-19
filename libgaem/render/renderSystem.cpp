#include <libgaem/core.hpp>
#include <libgaem/render/renderSystem.hpp>

#include <array>
#include <stdexcept>

namespace gaem {

struct SimplePushConstantData {
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

GaemRenderSystem::GaemRenderSystem(GaemDevice &device, VkRenderPass renderPass)
    : device{device} {
  createPipelineLayout();
  createPipeline(renderPass);
}

GaemRenderSystem::~GaemRenderSystem() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void GaemRenderSystem::createPipelineLayout() {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to create pipeline layout !");
    throw std::runtime_error("failed to create pipeline layout");
  }
}
void GaemRenderSystem::createPipeline(VkRenderPass renderPass) {
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipelineLayout!");

  PipelineConfig pipelineConfig{};
  GaemPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<GaemPipeline>(device, "example.vert",
                                            "example.frag", pipelineConfig);
}

// TODO make do something other than just show off push constants
void GaemRenderSystem::renderModels(VkCommandBuffer commandBuffer,
                                    std::unique_ptr<GaemModel> &model) {
  pipeline->bind(commandBuffer);
  model->bind(commandBuffer);

  for (int j = 0; j < 4; j++) {
    SimplePushConstantData push{};
    push.offset = {0.0f, -0.4f + j * 0.25f};
    float c = 1.0f - 0.3f * j;
    push.color = {c, c, c};

    vkCmdPushConstants(commandBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &push);
    model->draw(commandBuffer);
  }
}
} // namespace gaem
