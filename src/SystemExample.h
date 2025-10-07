#pragma once

/*
#include "System.h"
#include <iostream>

class ExampleSystem : public System {
public:
    ~ExampleSystem() override = default;
    void fixedUpdate(float fixedDelta) override {
        (void)fixedDelta;
    }
    void update(float dt) override {
        (void)dt;
    }
    void render() override {}
};
*/

#include "System.h"
#include <iostream>

class ExampleSystem : public System {
public:
    ~ExampleSystem() override = default;
    void fixedUpdate(float fixedDelta) override {
        (void)fixedDelta;
        std::cout << "[ExampleSystem] fixedUpdate\n";
    }
    void update(float dt) override {
        (void)dt;
        std::cout << "[ExampleSystem] update\n";
    }
    void render() override {
        std::cout << "[ExampleSystem] render\n";
    }
};
