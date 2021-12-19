#include <libgaem/core.hpp>
#include <libgaem/render/renderer.hpp>

#include <array>
#include <stdexcept>

namespace gaem {

GaemRenderer::GaemRenderer(GaemWindow &window, GaemDevice &device)
    : window{window}, device{device} {

  recreateSwapChain();
  createCommandBuffers();
}

GaemRenderer::~GaemRenderer() { freeCommandBuffers(); }

void GaemRenderer::recreateSwapChain() {
  auto extent = window.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = window.getExtent();
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device.device());
  if (swapChain == nullptr) {
    swapChain = std::make_unique<GaemSwapChain>(device, extent);
  } else {
    std::shared_ptr<GaemSwapChain> oldSwapChain = std::move(swapChain);
    swapChain = std::make_unique<GaemSwapChain>(device, extent, oldSwapChain);

    if (oldSwapChain->compareSwapFormat(*swapChain.get())) {
      throw std::runtime_error("Swap chain image(or depth) format has changed");
    }
  }

  // TODO check if renderPipelines are compatable, if so do nothing.
  // come back to create pipelines
}

void GaemRenderer::createCommandBuffers() {
  commandBuffers.resize(GaemSwapChain::MAX_FRAMES_IN_FLIGHT);
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

void GaemRenderer::freeCommandBuffers() {
  vkFreeCommandBuffers(device.device(), device.getCommandPool(),
                       static_cast<uint32_t>(commandBuffers.size()),
                       commandBuffers.data());
  commandBuffers.clear();
}

VkCommandBuffer GaemRenderer::beginFrame() {
  assert(!isFrameStarted && "Cant call begin frame while already in progress");

  auto result = swapChain->acquireNextImage(&currentImageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    // TODO handle this differently, it can happen when window is resized
    LOG_CRITICAL("FAILED to acquire swap chain image!");
    throw std::runtime_error("failed to acquire swap chain image");
  }

  isFrameStarted = true;

  auto commandBuffer = getCurrentCommandBuffer();

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to begin recording command buffer!");
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  return commandBuffer;
}

void GaemRenderer::endFrame() {
  assert(isFrameStarted && "Cant call endFrame while frame is not in progress");
  auto commandBuffer = getCurrentCommandBuffer();
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to record command buffer!");
    throw std::runtime_error("failed to record command buffer");
  }
  auto result =
      swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window.wasFrameBufferResized()) {
    window.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    LOG_CRITICAL("FAILED to present swap chain image!");
    throw std::runtime_error("failed to present swap chain image");
  }

  isFrameStarted = false;
  currentFrameIndex =
      (currentFrameIndex + 1) % GaemSwapChain::MAX_FRAMES_IN_FLIGHT;
}
void GaemRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(isFrameStarted &&
         " Cant call beginSwapChainRenderPass if frame is not in progress");
  assert(commandBuffer == getCurrentCommandBuffer() &&
         "Cant begin render pass on command buffer from a different frame");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = swapChain->getRenderPass();
  renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
  viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
void GaemRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(isFrameStarted &&
         " Cant call endSwapChainRenderPass if frame is not in progress");
  assert(commandBuffer == getCurrentCommandBuffer() &&
         "Cant end render pass on command buffer from a different frame");

  vkCmdEndRenderPass(commandBuffer);
}

} // namespace gaem
