#include <libgaem/application.hpp>
#include <libgaem/core.hpp>
#include <libgaem/render/renderSystem.hpp>

#include <array>
#include <stdexcept>

namespace gaem {

Application::Application() { loadModels(); }

Application::~Application() {}

void Application::run() {

  RenderSystem renderSystem{device, renderer.getSwapChainRenderPass()};

  while (window.isRunning()) {
    window.updateWindow();

    if (auto commandBuffer = renderer.beginFrame()) {
      renderer.beginSwapChainRenderPass(commandBuffer);

      renderSystem.renderModels(commandBuffer, model);

      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }
  vkDeviceWaitIdle(device.device());
}

void Application::loadModels() {
  std::vector<GaemModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                          {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                          {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  model = std::make_unique<GaemModel>(device, vertices);
}
} // namespace gaem
