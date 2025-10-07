#pragma once

#include "System.h"
#include <GLFW/glfw3.h>

class InputSystem : public System {
public:
    explicit InputSystem(GLFWwindow* window);
    ~InputSystem() override = default;

    void fixedUpdate(float) override {}
    void update(float dt) override;
    void render() override {}

    bool shouldStop() const override;

private:
    GLFWwindow* window_;
    bool stopRequested_;
};
