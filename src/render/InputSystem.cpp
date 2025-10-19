#include "InputSystem.h"
#include <iostream>

InputSystem::InputSystem(GLFWwindow* window)
    : window_(window),
      stopRequested_(false),
      lastX_(0.0),
      lastY_(0.0),
      firstMouse_(true),
      mouseDeltaX_(0.0),
      mouseDeltaY_(0.0),
      scrollDeltaY_(0.0)
{
    if (!window_) return;

    double x, y;
    glfwGetCursorPos(window_, &x, &y);
    lastX_ = x;
    lastY_ = y;

    glfwSetWindowUserPointer(window_, this);

    glfwSetScrollCallback(window_, +[](GLFWwindow* win, double /*xoffset*/, double yoffset){
        auto *inst = static_cast<InputSystem*>(glfwGetWindowUserPointer(win));
        if (inst) inst->scrollDeltaY_ += yoffset;
    });


    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputSystem::update(float /*dt*/) {
    if (!window_) return;

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        stopRequested_ = true;
    }


    double xpos, ypos;
    glfwGetCursorPos(window_, &xpos, &ypos);
    if (firstMouse_) {
        lastX_ = xpos; lastY_ = ypos; firstMouse_ = false;
    }
    double dx = xpos - lastX_;
    double dy = lastY_ - ypos;
    lastX_ = xpos; lastY_ = ypos;


    if (glfwGetInputMode(window_, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        mouseDeltaX_ += dx;
        mouseDeltaY_ += dy;
    }

    static bool tabPrev = false;
    bool tabNow = (glfwGetKey(window_, GLFW_KEY_TAB) == GLFW_PRESS);
    if (tabNow && !tabPrev) {
        int mode = glfwGetInputMode(window_, GLFW_CURSOR);
        if (mode == GLFW_CURSOR_DISABLED) glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    tabPrev = tabNow;
}

bool InputSystem::shouldStop() const {
    return stopRequested_;
}

bool InputSystem::isKeyDown(int glfwKey) const {
    if (!window_) return false;
    return glfwGetKey(window_, glfwKey) == GLFW_PRESS;
}

glm::vec2 InputSystem::getMouseDelta() {
    glm::vec2 d(static_cast<float>(mouseDeltaX_), static_cast<float>(mouseDeltaY_));
    mouseDeltaX_ = 0.0;
    mouseDeltaY_ = 0.0;
    return d;
}

float InputSystem::getScrollDelta() {
    float v = static_cast<float>(scrollDeltaY_);
    scrollDeltaY_ = 0.0;
    return v;
}
