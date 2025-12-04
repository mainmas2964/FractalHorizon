#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>
#include "Clock.h"
#pragma once
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
class TaskSystem {
public:
    TaskSystem(size_t numThreads = std::thread::hardware_concurrency());
    ~TaskSystem();
    void enqueueTask(const Task& task);
    void waitForCompletion();
    void registerIntervalFunc(const TickTask& tickTask);
    void updateIntervalTasks(Clock& clock);
private:
    void workerThread();
    std::vector<std::thread> workers;
    std::queue<Task> tasks;
    std::vector<TickTask> tickTasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    std::atomic<size_t> activeTasks;
};