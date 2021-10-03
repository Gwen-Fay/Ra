#include "window.hpp"
#include "log.h"

namespace gaem {

bool Window::isRunning() { return running; }

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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr,
                                  nullptr);

        if (!window) {
            return -1;
        }
        return 0;
    } else {
        return -1;
    }
}

void WindowGlfw::updateWindow() {
    glfwPollEvents();
    running = !glfwWindowShouldClose(window);
}

} // namespace gaem
