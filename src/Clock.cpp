#include "Clock.h"
Clock::Clock() {
    start_time = std::chrono::high_resolution_clock::now();
    last_time = start_time;
}

void Clock::update() {
    last_time = std::chrono::high_resolution_clock::now();
}
void Clock::reset() {
    start_time = std::chrono::high_resolution_clock::now();
    last_time = start_time;
}

float Clock::getDeltaTime() const {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - last_time;
    return delta.count();
}
float Clock::getTotalTime() const {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> total = now - start_time;
    return total.count();
}