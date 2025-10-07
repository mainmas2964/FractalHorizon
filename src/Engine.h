#pragma once

#include <vector>
#include <memory>
#include <chrono>

class System;

class Engine {
public:
    Engine();
    ~Engine();

<<<<<<< HEAD

    bool init(); // initialize engine subsystems. Return true on success
    bool initECS(); // initialize ECS (placeholder)
    //TODO: do realization to this method in Engine.cpp


    void run(); // run the main loop (placeholder)
    void update(); // update ECS and other subsystems (placeholder)
    //TODO: do realization to this method in Engine.cpp



    void shutdown(); // cleanup resources
    void cleanup(); // cleanup ECS & other subsystems (placeholder)
    void saveState(const std::string& filename); // save engine state to a file (placeholder)
    //TODO: do realization to this method in Engine.cpp
    void loadState(const std::string& filename); // load engine state from a file (placeholder)
    //TODO: do realization to this method in Engine.cpp

=======
    bool init();
    void run();
    void shutdown();
    void addSystem(std::shared_ptr<System> sys);
    void stop();
>>>>>>> origin/main

private:
    bool initialized_;
    std::vector<std::shared_ptr<System>> systems_;
    std::chrono::steady_clock::time_point lastFrameTime_;
    float accumulateTime_;
    const float fixedDelta_ = 1.0f / 60.0f;
    bool running_;
    int maxTicks_ = 0; // 0 = бесконечно
};
