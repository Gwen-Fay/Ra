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
class Window {
  public:
    Window(){};
    virtual ~Window(){};
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    bool isRunning();
    virtual void updateWindow() = 0;

  protected:
    virtual int initWindow() = 0;

    bool running = true;
};

/**
 * GLFW window wrapper.
 */

class WindowGlfw : public Window {
  public:
    WindowGlfw(int w, int h, std::string name);
    ~WindowGlfw();
    WindowGlfw(const WindowGlfw &) = delete;
    WindowGlfw &operator=(const WindowGlfw &) = delete;
    void updateWindow() override;

  protected:
    int initWindow() override;
    const int width;
    const int height;
    std::string windowName;
    GLFWwindow *window;
};
} // namespace gaem
