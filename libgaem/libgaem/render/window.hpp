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
  GaemWindow();
  virtual ~GaemWindow(){};
  GaemWindow(const GaemWindow &) = delete;
  GaemWindow &operator=(const GaemWindow &) = delete;
  bool isRunning();
  virtual void updateWindow() = 0;
  virtual void createWindowSurface(VkInstance instance,
                                   VkSurfaceKHR *surface) = 0;

  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
  }

  bool wasFrameBufferResized() { return frameBufferResized; }
  void resetWindowResizedFlag() { frameBufferResized = false; };

protected:
  GaemWindow(int w, int h, std::string name)
      : width(w), height(h), windowName(name){};
  virtual int initWindow() = 0;

  int width;
  int height;
  std::string windowName;

  bool running = true;
  bool frameBufferResized = false;
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
  void updateWindow() override;
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) override;

protected:
  int initWindow() override;
  static void frameBufferResizeCallback(GLFWwindow *window, int width,
                                        int height);
  GLFWwindow *window;
};
} // namespace gaem
