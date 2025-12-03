#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#pragma once
struct Task{
    std::function<void()> func;
};
class TaskSystem {
public:
    TaskSystem(size_t numThreads = std::thread::hardware_concurrency());
    ~TaskSystem();
    void enqueueTask(const Task& task);
    void waitForCompletion();
private:
    void workerThread();
    std::vector<std::thread> workers;
    std::queue<Task> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    std::atomic<size_t> activeTasks;
};