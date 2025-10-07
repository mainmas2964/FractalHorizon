#pragma once

#include <GLFW/glfw3.h>
#include <string>

class WindowGLFW {
public:
    WindowGLFW(const std::string& title, int width, int height);
    ~WindowGLFW();

    bool init();
    void pollEvents();
    void swapBuffers();
    void makeContextCurrent();
    bool shouldClose() const;
    GLFWwindow* getHandle() const;

private:
    std::string title_;
    int width_;
    int height_;
    GLFWwindow* window_;
};
