#include "glad/glad.h"
#include "RenderCore.h"
#include <iostream>

RenderCore::RenderCore()
    : window_(std::make_unique<WindowGLFW>("FractalHorizon", 1280, 720)) {}

RenderCore::~RenderCore() {

    if (window_ && window_->getHandle()) {
        glfwDestroyWindow(window_->getHandle());
    }
    glfwTerminate();
}

bool RenderCore::init() {
    if (!window_) return false;
    if (!window_->init()) {
        std::cerr << "[RenderCore] Window init failed\n";
        return false;
    }

    // basic GL state
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    return true;
}

void RenderCore::update(float dt) {
    (void)dt;
    // Poll events here so InputSystem reads fresh state afterwards
    window_->pollEvents();
}

void RenderCore::render() {
    // simple clear pass real render passes will live in render/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window_->swapBuffers();
}

bool RenderCore::shouldClose() const {
    return window_ ? window_->shouldClose() : true;
}

// optional getter if other systems need raw handle
GLFWwindow* RenderCore::getWindowHandle() const {
    return window_ ? window_->getHandle() : nullptr;
}
