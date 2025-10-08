#include "glad/glad.h"
#include "RenderCore.h"
#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>

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

    // Build shader paths relative to this source file so they are found regardless of CWD
    namespace fs = std::filesystem;
    fs::path srcDir = fs::path(__FILE__).parent_path(); // .../src/render
    fs::path shaderDir = srcDir / "shaders";
    fs::path vertPath = shaderDir / "circle.vertex";
    fs::path fragPath = shaderDir / "circle.fragment";
    std::cout << "[RenderCore] loading shaders from: " << vertPath << " and " << fragPath << std::endl;
    if (!fs::exists(vertPath) || !fs::exists(fragPath)) {
        std::cerr << "[RenderCore] Shader files not found\n";
    } else {
        shader_.compile(vertPath.string().c_str(), fragPath.string().c_str());
        std::cout << "[RenderCore] shader compiled successfully" << std::endl;
    }

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
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int vao_, vbo_;

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);


    // Здесь будет отрисовка, когда появится геометрия

    window_->swapBuffers();
}

bool RenderCore::shouldClose() const {
    return window_ ? window_->shouldClose() : true;
}

GLFWwindow* RenderCore::getWindowHandle() const {
    return window_ ? window_->getHandle() : nullptr;
}
