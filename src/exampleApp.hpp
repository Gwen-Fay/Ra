/**
 *
 * This is a simple example Application which manages and updates a window
 *
 */

#pragma once

#include "device.hpp"
#include "model.hpp"
#include "pipeline.hpp"
#include "swapChain.hpp"
#include "window.hpp"

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
  Device device{window};
  SwapChain swapChain{device, window.getExtent()};
  std::unique_ptr<Pipeline> pipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<Model> model;
};

} // namespace gaem
