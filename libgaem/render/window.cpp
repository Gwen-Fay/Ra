#include <libgaem/core.hpp>
#include <libgaem/render/window.hpp>

#include <stdexcept>

namespace gaem {

bool GaemWindow::isRunning() { return running; }

WindowGlfw::WindowGlfw(int w, int h, std::string name)
    : width{w}, height{h}, windowName{name} {

  if (initWindow() == 0) {
    LOG_INFO("initWindow Sucessful");
  } else {
    LOG_CRITICAL("initWindow FAILED!!!");
    running = false;
  }
}

WindowGlfw::~WindowGlfw() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int WindowGlfw::initWindow() {
  if (glfwInit()) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window =
        glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

    if (!window) {
      return -1;
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

    return 0;
  } else {
    return -1;
  }
}

void WindowGlfw::updateWindow() {
  glfwPollEvents();
  running = !glfwWindowShouldClose(window);
}

void WindowGlfw::createWindowSurface(VkInstance instance,
                                     VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create runtime surface.");
    LOG_CRITICAL("FAILED to create runtime surface!");
  }
}

void WindowGlfw::frameBufferResizeCallback(GLFWwindow *window, int width,
                                           int height) {
  auto gaemWindow =
      reinterpret_cast<WindowGlfw *>(glfwGetWindowUserPointer(window));
  gaemWindow->frameBufferResized = true;
  gaemWindow->width = width;
  gaemWindow->height = height;
}

} // namespace gaem
