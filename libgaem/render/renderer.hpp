/**
 *
 * This class in in charge of the device, swapchain, and window to render.
 *
 */

#pragma once

#include <libgaem/core.hpp>
#include <libgaem/render/device.hpp>
#include <libgaem/render/swapChain.hpp>
#include <libgaem/render/window.hpp>

#include <cassert>
#include <memory>
#include <vector>

namespace gaem {

class GaemRenderer {
public:
  GaemRenderer(GaemWindow &window, GaemDevice &device);
  virtual ~GaemRenderer();

  GaemRenderer(const GaemRenderer &) = delete;
  GaemRenderer &operator=(const GaemRenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {
    return swapChain->getRenderPass();
  }

  bool isFrameInProgress() const { return isFrameStarted; }
  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame is not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(isFrameStarted &&
           "Cannot get frame index when frame is not in progress");
    return currentFrameIndex;
  };

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  GaemWindow &window;
  GaemDevice &device;
  std::unique_ptr<GaemSwapChain> swapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex;
  int currentFrameIndex;
  bool isFrameStarted = false;
};

} // namespace gaem
