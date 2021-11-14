/**
 *
 * This is a simple example Application which manages and updates a window
 *
 */

#pragma once
#include "window.hpp"

namespace gaem {

class ExampleApp {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    void run();

  private:
    WindowGlfw window{WIDTH, HEIGHT, "Hello Vulkan"};
};

} // namespace gaem
