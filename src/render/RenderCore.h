#pragma once

#include "System.h"
#include "Shader.h"
#include "WindowGLFW.h"
#include "UniformState.h"
#include <glm/glm.hpp>
#include <memory>

class RenderCore : public System {
public:
    RenderCore();
    ~RenderCore() override;

    bool init();
    void fixedUpdate(float) override {}
    void update(float dt) override;
    void render() override;

    bool shouldClose() const;

    GLFWwindow* getWindowHandle() const;
private:
    std::unique_ptr<WindowGLFW> window_;
    Shader shader_;
    UniformState uniforms_;
};
