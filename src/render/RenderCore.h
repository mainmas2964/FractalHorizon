#pragma once

#include "System.h"
#include "Shader.h"
#include "WindowGLFW.h"
#include "UniformState.h"
#include "ChunkRender.h"
#include "world/chunk/chunk.h"
#include <glm/glm.hpp>
#include <memory>
#include "Camera.h"
class RenderCore : public System {
public:
    RenderCore();
    ~RenderCore() override;

    bool init();
    void fixedUpdate(float) override {}
    void update(float dt) override;
    void render() override;
    Camera camera;

    bool shouldClose() const;

    GLFWwindow* getWindowHandle() const;

private:
    float _animAngle = 0.0f;
    bool _rotateModel = true;
    float _rotationSpeed = glm::radians(30.0f);
    std::unique_ptr<WindowGLFW> window_;
    Shader shader_;
    UniformState uniforms_;

    // single-chunk storage and renderer
    std::unique_ptr<Chunk> chunk_;
    std::unique_ptr<ChunkRenderer> chunkRenderer_;
};
