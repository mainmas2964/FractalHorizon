#pragma once

#include <vector>
#include <memory>
#include <chrono>

class System;

class Engine {
public:
    Engine();
    ~Engine();

    bool init();
    void run();
    void shutdown();
    void addSystem(std::shared_ptr<System> sys);
    void stop();

private:
    bool initialized_;
    std::vector<std::shared_ptr<System>> systems_;
    std::chrono::steady_clock::time_point lastFrameTime_;
    float accumulateTime_;
    const float fixedDelta_ = 1.0f / 60.0f;
    bool running_;
    int maxTicks_ = 0; // 0 = бесконечно
};
