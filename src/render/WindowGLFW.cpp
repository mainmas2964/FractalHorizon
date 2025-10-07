#include <iostream>
#include "glad/glad.h"
#include "WindowGLFW.h"
WindowGLFW::WindowGLFW(const std::string& title, int width, int height)
    : title_(title), width_(width), height_(height), window_(nullptr) {}

WindowGLFW::~WindowGLFW() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    // Do not call glfwTerminate() here if other parts of the app may still use GLFW.
    // Responsibility for termination can be left to RenderCore (which owns first init).
}

bool WindowGLFW::init() {
    if (!glfwInit()) {
        std::cerr << "[WindowGLFW] glfwInit failed\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    if (!window_) {
        std::cerr << "[WindowGLFW] glfwCreateWindow failed\n";
        glfwTerminate();
        return false;
    }

    makeContextCurrent();
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[WindowGLFW] gladLoadGLLoader failed\n";
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();
        return false;
    }

    return true;
}

void WindowGLFW::makeContextCurrent() {
    if (window_) glfwMakeContextCurrent(window_);
}

void WindowGLFW::pollEvents() {
    glfwPollEvents();
}

void WindowGLFW::swapBuffers() {
    if (window_) glfwSwapBuffers(window_);
}

bool WindowGLFW::shouldClose() const {
    return window_ ? glfwWindowShouldClose(window_) : true;
}

GLFWwindow* WindowGLFW::getHandle() const {
    return window_;
}
