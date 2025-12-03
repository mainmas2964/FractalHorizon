#pragma once
#include <chrono>
class Clock {
    public:
    Clock();
    ~Clock() = default;
    void reset();
    void update();
    float getDeltaTime() const;
    float getTotalTime() const;
    private:
        std::chrono::high_resolution_clock::time_point start_time;
        std::chrono::high_resolution_clock::time_point last_time;

};