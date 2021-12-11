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
  recreateSwapChain();
  createCommandBuffers();
}

Application::~Application() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void Application::run() {
  while (window.isRunning()) {
    window.updateWindow();
    drawFrame();
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
  assert(swapChain != nullptr && "Cannot create pipeline before swap chain!");
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipelineLayout!");

  PipelineConfig pipelineConfig{};
  GaemPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = swapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<GaemPipeline>(device, "example.vert",
                                            "example.frag", pipelineConfig);
}

void Application::recreateSwapChain() {
  auto extent = window.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = window.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device.device());
  if (swapChain == nullptr) {
    swapChain = std::make_unique<GaemSwapChain>(device, extent);
  } else {
    swapChain =
        std::make_unique<GaemSwapChain>(device, extent, std::move(swapChain));
    if (swapChain->imageCount() != commandBuffers.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  // TODO check if renderPipelines are compatable, if so do nothing.
  createPipeline();
}

void Application::createCommandBuffers() {
  commandBuffers.resize(swapChain->imageCount());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to allocate command buffers!");
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void Application::freeCommandBuffers() {
  vkFreeCommandBuffers(device.device(), device.getCommandPool(),
                       static_cast<uint32_t>(commandBuffers.size()),
                       commandBuffers.data());
  commandBuffers.clear();
}

void Application::recordCommandBuffer(int imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    LOG_CRITICAL("FAILED to begin recording command buffer!");
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = swapChain->getRenderPass();
  renderPassInfo.framebuffer = swapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
  viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  pipeline->bind(commandBuffers[imageIndex]);
  model->bind(commandBuffers[imageIndex]);

  for (int j = 0; j < 4; j++) {
    SimplePushConstantData push{};
    push.offset = {0.0f, -0.4f + j + 0.25f};
    push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

    vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &push);
    model->draw(commandBuffers[imageIndex]);
  }

  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to record command buffer!");
    throw std::runtime_error("failed to record command buffer");
  }
}
void Application::drawFrame() {
  uint32_t imageIndex;
  auto result = swapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    // TODO handle this differently, it can happen when window is resized
    LOG_CRITICAL("FAILED to acquire swap chain image!");
    throw std::runtime_error("failed to acquire swap chain image");
  }
  recordCommandBuffer(imageIndex);
  result =
      swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window.wasFrameBufferResized()) {
    window.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }
  if (result != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to present swap chain image!");
    throw std::runtime_error("failed to present swap chain image");
  }
}

void Application::loadModels() {
  std::vector<GaemModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                          {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                          {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  model = std::make_unique<GaemModel>(device, vertices);
}

} // namespace gaem
