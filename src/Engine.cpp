#include "Engine.h"
#include <iostream>

Engine::Engine() : initialized_(false) {}
Engine::~Engine() {}

bool Engine::init() {
    std::cout << "[Engine] init()" << std::endl;
    initialized_ = true;
    // TODO: initialize graphics, window, input, audio, etc.
    return initialized_;
}

void Engine::run() {
    std::cout << "[Engine] run() - entering main loop (placeholder)" << std::endl;
    if (!initialized_) {
        std::cout << "[Engine] run() called before init()" << std::endl;
        return;
    }
    // Placeholder main loop: print a few ticks then exit.
    for (int i = 0; i < 3; ++i) {
        std::cout << "[Engine] tick " << (i + 1) << std::endl;
    }
    std::cout << "[Engine] run() - exiting main loop" << std::endl;
}

void Engine::shutdown() {
    std::cout << "[Engine] shutdown()" << std::endl;
    if (initialized_) {
        // TODO: cleanup subsystems
        initialized_ = false;
    }
}
