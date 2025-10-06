#pragma once

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
