#include <libgaem/application.hpp>
#include <libgaem/core.hpp>

#include <array>
#include <stdexcept>

namespace gaem {

Application::Application() {
  loadModels();
  createPipelineLayout();
  createPipeline();
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
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to create pipeline layout !");
    throw std::runtime_error("failed to create pipeline layout");
  }
}
void Application::createPipeline() {
  PipelineConfig pipelineConfig{};
  GaemPipeline::defaultPipelineConfig(pipelineConfig, gaemSwapChain.width(),
                                      gaemSwapChain.height());
  pipelineConfig.renderPass = gaemSwapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  gaemPipeline = std::make_unique<GaemPipeline>(device, "example.vert",
                                                "example.frag", pipelineConfig);
}

void Application::createCommandBuffers() {
  commandBuffers.resize(gaemSwapChain.imageCount());
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

  for (int i = 0; i < commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      LOG_CRITICAL("FAILED to begin recording command buffer!");
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = gaemSwapChain.getRenderPass();
    renderPassInfo.framebuffer = gaemSwapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = gaemSwapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);
    gaemPipeline->bind(commandBuffers[i]);
    model->bind(commandBuffers[i]);
    model->draw(commandBuffers[i]);

    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      LOG_CRITICAL("FAILED to record command buffer!");
      throw std::runtime_error("failed to record command buffer");
    }
  }
}

void Application::drawFrame() {
  uint32_t imageIndex;
  auto result = gaemSwapChain.acquireNextImage(&imageIndex);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    // TODO handle this differently, it can happen when window is resized
    LOG_CRITICAL("FAILED to acquire swap chain image!");
    throw std::runtime_error("failed to acquire swap chain image");
  }

  result = gaemSwapChain.submitCommandBuffers(&commandBuffers[imageIndex],
                                              &imageIndex);
  if (result != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to present swap chain image!");
    throw std::runtime_error("failed to present swap chain image");
  }
}

void Application::loadModels() {
  std::vector<GaemModel::Vertex> vertices{
      {{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};

  model = std::make_unique<GaemModel>(device, vertices);
}

} // namespace gaem
