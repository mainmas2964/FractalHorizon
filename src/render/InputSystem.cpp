#include "InputSystem.h"
#include <iostream>

InputSystem::InputSystem(GLFWwindow* window)
    : window_(window), stopRequested_(false) {
    if (!window_) std::cerr << "[InputSystem] Warning: window handle is null\n";
}

void InputSystem::update(float dt) {
    (void)dt;
    if (!window_) return;

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        stopRequested_ = true;
    }

    if (glfwWindowShouldClose(window_)) {
        stopRequested_ = true;
    }
}

bool InputSystem::shouldStop() const {
    return stopRequested_;
}
