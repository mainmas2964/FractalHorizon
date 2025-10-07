#include "WindowGLFW.h"
#include <iostream>
#include "glad/glad.h"

WindowGLFW::WindowGLFW(const std::string& title, int width, int height)
    : title_(title), width_(width), height_(height), window_(nullptr) {}

WindowGLFW::~WindowGLFW() {
    if (window_) glfwDestroyWindow(window_);
    glfwTerminate();
}

bool WindowGLFW::init() {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    if (!window_) return false;

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;

    std::cout << "[WindowGLFW] Initialized\n";
    return true;
}

void WindowGLFW::pollEvents() {
    glfwPollEvents();
}

void WindowGLFW::swapBuffers() {
    glfwSwapBuffers(window_);
}

bool WindowGLFW::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

GLFWwindow* WindowGLFW::getHandle() const {
    return window_;
}
