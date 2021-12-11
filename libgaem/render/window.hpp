/**
 *
 * Window is the abstract class for all window wrappers. this way I can in
 * the future swap out GLFW if I want to.
 *
 */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace gaem {
class GaemWindow {
public:
  GaemWindow(){};
  virtual ~GaemWindow(){};
  GaemWindow(const GaemWindow &) = delete;
  GaemWindow &operator=(const GaemWindow &) = delete;
  bool isRunning();
  virtual void updateWindow() = 0;
  virtual void createWindowSurface(VkInstance instance,
                                   VkSurfaceKHR *surface) = 0;

protected:
  virtual int initWindow() = 0;

  bool running = true;
};

/**
 * GLFW window wrapper.
 */

class WindowGlfw : public GaemWindow {
public:
  WindowGlfw(int w, int h, std::string name);
  ~WindowGlfw();
  WindowGlfw(const WindowGlfw &) = delete;
  WindowGlfw &operator=(const WindowGlfw &) = delete;
  bool wasFrameBufferResized() { return frameBufferResized; }
  void resetWindowResizedFlag() { frameBufferResized = false; };
  void updateWindow() override;
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) override;
  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

protected:
  int initWindow() override;
  int width;
  int height;
  bool frameBufferResized = false;
  static void frameBufferResizeCallback(GLFWwindow *window, int width,
                                        int height);
  std::string windowName;
  GLFWwindow *window;
};
} // namespace gaem
