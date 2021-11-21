#include "exampleApp.hpp"
#include "log.h"

#include <array>
#include <stdexcept>

namespace gaem {

ExampleApp::ExampleApp() {
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}

ExampleApp::~ExampleApp() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void ExampleApp::run() {
  while (window.isRunning()) {
    window.updateWindow();
    drawFrame();
  }
  vkDeviceWaitIdle(device.device());
}

void ExampleApp::createPipelineLayout() {
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
void ExampleApp::createPipeline() {
  PipelineConfig pipelineConfig{};
  Pipeline::defaultPipelineConfig(pipelineConfig, swapChain.width(),
                                  swapChain.height());
  pipelineConfig.renderPass = swapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  pipeline = std::make_unique<Pipeline>(device, "example.vert", "example.frag",
                                        pipelineConfig);
}

void ExampleApp::createCommandBuffers() {
  commandBuffers.resize(swapChain.imageCount());
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
    renderPassInfo.renderPass = swapChain.getRenderPass();
    renderPassInfo.framebuffer = swapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);
    pipeline->bind(commandBuffers[i]);
    vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      LOG_CRITICAL("FAILED to record command buffer!");
      throw std::runtime_error("failed to record command buffer");
    }
  }
}

void ExampleApp::drawFrame() {
  uint32_t imageIndex;
  auto result = swapChain.acquireNextImage(&imageIndex);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    // TODO handle this differently, it can happen when window is resized
    LOG_CRITICAL("FAILED to acquire swap chain image!");
    throw std::runtime_error("failed to acquire swap chain image");
  }

  result =
      swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to present swap chain image!");
    throw std::runtime_error("failed to present swap chain image");
  }
}

} // namespace gaem
