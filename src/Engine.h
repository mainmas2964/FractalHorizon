#pragma once

#include "System.h"
#include <memory>
#include <vector>
#include <chrono>

class Engine {
public:
    Engine();
    ~Engine();

    bool init();
    void shutdown();

    void addSystem(std::shared_ptr<System> sys);
    void run();
    void stop();

    void setupDefaultSystems(); // создает и добавляет RenderCore, InputSystem и тд

    void setMaxTicks(int max) { maxTicks_ = max; }
    void setFixedDelta(float d) { fixedDelta_ = d; }

private:
    bool initialized_;
    float accumulateTime_;
    bool running_;
    int maxTicks_;
    float fixedDelta_;

    std::chrono::steady_clock::time_point lastFrameTime_;
    std::vector<std::shared_ptr<System>> systems_;
};
