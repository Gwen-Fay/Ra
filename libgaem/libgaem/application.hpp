/**
 *
 * This is a simple example Application which manages and updates a window
 *
 */

#pragma once

#include <libgaem/core.hpp>
#include <libgaem/render/device.hpp>
#include <libgaem/render/model.hpp>
#include <libgaem/render/renderer.hpp>
#include <libgaem/render/window.hpp>

#include <memory>
#include <vector>

namespace gaem {

class Application {
public:
  Application();
  virtual ~Application();
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

  void run();

  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

private:
  void loadModels();

  WindowGlfw window{WIDTH, HEIGHT, "Hello Vulkan"};
  GaemDevice device{window};
  GaemRenderer renderer{window, device};

  std::unique_ptr<GaemModel> model;
};

} // namespace gaem
