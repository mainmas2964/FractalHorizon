#pragma once

#include "System.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/glm.hpp>

class InputSystem : public System {
public:
    explicit InputSystem(GLFWwindow* window);
    ~InputSystem() override = default;

    void fixedUpdate(float) override {}
    void update(float dt) override;
    void render() override {}

    bool shouldStop() const override;


    bool isKeyDown(int glfwKey) const;
    glm::vec2 getMouseDelta();    
    float getScrollDelta();     

private:
    GLFWwindow* window_;
    bool stopRequested_;

    // mouse tracking
    double lastX_;
    double lastY_;
    bool firstMouse_;
    double mouseDeltaX_;
    double mouseDeltaY_;

    // scroll
    double scrollDeltaY_;
};
