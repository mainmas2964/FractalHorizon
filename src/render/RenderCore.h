#pragma once

#include "System.h"
#include "WindowGLFW.h"
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

private:
    std::unique_ptr<WindowGLFW> window_;
};
