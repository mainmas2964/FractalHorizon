#include "Engine.h"
#include "System.h"
#include <chrono>
#include <iostream>

// Подсистемы
#include "render/RenderCore.h"
#include "render/InputSystem.h"

Engine::Engine()
    : initialized_(false),
      accumulateTime_(0.0f),
      running_(false),
      maxTicks_(0),
      fixedDelta_(1.0f / 60.0f) {}

Engine::~Engine() {
    shutdown();
}

bool Engine::init() {
    lastFrameTime_ = std::chrono::steady_clock::now();
    initialized_ = true;

    setupDefaultSystems();

    return true;
}

void Engine::setupDefaultSystems() {
    // Создаём RenderCore в нём инициализируется GLFW,  GLAD и окно
    auto renderCore = std::make_shared<RenderCore>();
    if (!renderCore->init()) {
        std::cerr << "[Engine] RenderCore initialization failed aborting default setup\n";
        return;
    }
    addSystem(renderCore);

    // Создаём InputSystem передаём raw handle окна InputSystem не владеет окном как минимум я надеюсь
    GLFWwindow* win = nullptr;

    win = renderCore->getWindowHandle();
    if (!win) {
        std::cerr << "[Engine] Warning: RenderCore returned null window handle\n";
    }
    auto inputSys = std::make_shared<InputSystem>(win);
    addSystem(inputSys);

    // Здесь можно добавлять другие подсистемы типо ECS, Physics и тд
    // auto ecs = std::make_shared<CustomSystem>();
    // if (ecs->init()) addSystem(ecs);
}

void Engine::addSystem(std::shared_ptr<System> sys) {
    systems_.push_back(std::move(sys));
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
            for (auto &sys : systems_) {
                sys->fixedUpdate(fixedDelta_);
            }
            accumulateTime_ -= fixedDelta_;
        }

        for (auto &sys : systems_) {
            sys->update(dt);
        }

        for (auto &sys : systems_) {
            if (sys->shouldStop()) {
                running_ = false;
                break;
            }
        }
        if (!running_) break;

        for (auto &sys : systems_) {
            sys->render();
        }

        for (auto &sys : systems_) {
            if (sys->shouldStop()) {
                running_ = false;
                break;
            }
        }
        if (!running_) break;

        if (maxTicks_ > 0 && ++tickCount >= maxTicks_) {
            running_ = false;
            break;
        }
    }
}

void Engine::stop() {
    running_ = false;
}

void Engine::shutdown() {
    systems_.clear();
    initialized_ = false;
}
