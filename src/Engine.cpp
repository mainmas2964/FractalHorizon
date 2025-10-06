#include "Engine.h"
#include "System.h"
#include <iostream>

Engine::Engine()
    : initialized_(false),
      accumulateTime_(0.0f),
      running_(false),
      maxTicks_(0) {}

Engine::~Engine() { shutdown(); }

bool Engine::init() {
    lastFrameTime_ = std::chrono::steady_clock::now();
    initialized_ = true;
    return true;
}

void Engine::addSystem(std::shared_ptr<System> sys) {
    systems_.push_back(sys);
}

void Engine::run() {
    if (!initialized_) return;
    running_ = true;
    int tickCount = 0;
    while (running_) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta = now - lastFrameTime_;
        lastFrameTime_ = now;
        float dt = delta.count();
        accumulateTime_ += dt;

        while (accumulateTime_ >= fixedDelta_) {
            for (auto &sys : systems_) sys->fixedUpdate(fixedDelta_);
            accumulateTime_ -= fixedDelta_;
        }

        for (auto &sys : systems_) sys->update(dt);

        for (auto &sys : systems_) sys->render();

        if (maxTicks_ > 0 && ++tickCount >= maxTicks_) running_ = false;
    }
}

void Engine::stop() {
    running_ = false;
}

void Engine::shutdown() {
    systems_.clear();
    initialized_ = false;
}
