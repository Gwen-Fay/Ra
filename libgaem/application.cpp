#include <libgaem/application.hpp>
#include <libgaem/core.hpp>

#include <array>
#include <stdexcept>

namespace gaem {

struct SimplePushConstantData {
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

Application::Application() {
  loadModels();
  createPipelineLayout();
  createPipeline();
}

Application::~Application() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void Application::run() {
  while (window.isRunning()) {
    window.updateWindow();

    if (auto commandBuffer = renderer.beginFrame()) {
      renderer.beginSwapChainRenderPass(commandBuffer);

      // TODO replace code with something better
      renderModels(commandBuffer);

      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }
  vkDeviceWaitIdle(device.device());
}

void Application::createPipelineLayout() {
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
void Application::createPipeline() {
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipelineLayout!");

  PipelineConfig pipelineConfig{};
  GaemPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<GaemPipeline>(device, "example.vert",
                                            "example.frag", pipelineConfig);
}

void Application::loadModels() {
  std::vector<GaemModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                          {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                          {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  model = std::make_unique<GaemModel>(device, vertices);
}

void Application::renderModels(VkCommandBuffer commandBuffer) {
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
