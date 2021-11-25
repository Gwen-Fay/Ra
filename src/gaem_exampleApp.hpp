/**
 *
 * This is a simple example Application which manages and updates a window
 *
 */

#pragma once

#include "gaem_device.hpp"
#include "gaem_model.hpp"
#include "gaem_pipeline.hpp"
#include "gaem_swapChain.hpp"
#include "gaem_window.hpp"

#include <memory>
#include <vector>

namespace gaem {

class ExampleApp {
public:
  ExampleApp();
  ~ExampleApp();
  ExampleApp(const ExampleApp &) = delete;
  ExampleApp &operator=(const ExampleApp &) = delete;

  void run();

  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

private:
  void loadModels();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();

  WindowGlfw window{WIDTH, HEIGHT, "Hello Vulkan"};
  GaemDevice device{window};
  GaemSwapChain gaemSwapChain{device, window.getExtent()};
  std::unique_ptr<GaemPipeline> gaemPipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<GaemModel> model;
};

} // namespace gaem
