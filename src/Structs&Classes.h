#ifndef STRUCTUSES_AND_CLASSES_HEADER_
#define STRUCTUSES_AND_CLASSES_HEADER_
#include <stdint.h>
#include <functional>
#include <chrono>
struct Entity
{
    uint32_t id;
};
struct Task{
    std::function<void()> func;
};
struct TickTask{
    std::function<void()> func;
    std::chrono::milliseconds intervalMs;
    std::chrono::steady_clock::time_point lastExecutedMs;
    size_t id;
    size_t executionsRemaining;
    bool active;
    float timeAccumulator = 0.0f;

};
class Event {
    public:
    virtual ~Event() = default;
};
template<typename T>
class SystemECS;
#endif