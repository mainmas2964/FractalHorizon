#include "glad/glad.h"
#include "RenderCore.h"
#include <glm/glm.hpp>
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

    shader_.compile("shaders/circle.glsl", "shaders/circle.glsl");

    int w, h;
    glfwGetFramebufferSize(window_->getHandle(), &w, &h);
    uniforms_.initDefault();
    uniforms_.resolution = glm::vec2(w, h);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    return true;
}

void RenderCore::update(float dt) {
    (void)dt;
    window_->pollEvents();
}

void RenderCore::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.use();
    uniforms_.upload(shader_);

    // Здесь будет отрисовка, когда появится геометрия

    window_->swapBuffers();
}

bool RenderCore::shouldClose() const {
    return window_ ? window_->shouldClose() : true;
}

GLFWwindow* RenderCore::getWindowHandle() const {
    return window_ ? window_->getHandle() : nullptr;
}
