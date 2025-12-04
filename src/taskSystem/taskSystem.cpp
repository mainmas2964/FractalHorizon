#include "taskSystem.h"
TaskSystem::TaskSystem(size_t numThreads) : stop(false), activeTasks(0) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&TaskSystem::workerThread, this);
    }
}
TaskSystem::~TaskSystem() {
    stop = true;
    condition.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
void TaskSystem::enqueueTask(const Task& task){
    std::lock_guard<std::mutex> lock(queueMutex);
    tasks.push(task);
    activeTasks.fetch_add(1, std::memory_order_relaxed);
    condition.notify_one();
}
void TaskSystem::waitForCompletion(){
    std::unique_lock<std::mutex> lock(queueMutex);
    condition.wait(lock, [this]{ return activeTasks.load(std::memory_order_acquire) == 0; });
}
void TaskSystem::workerThread(){
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this]{ return stop.load(std::memory_order_acquire) || !tasks.empty(); });
            if (stop.load(std::memory_order_acquire) && tasks.empty()) return;
            task = tasks.front();
            tasks.pop();
        }
        task.func();
        activeTasks.fetch_sub(1, std::memory_order_release);
        condition.notify_all(); 
    }
}
void TaskSystem::registerIntervalFunc(const TickTask& tickTask){
    tickTasks.push_back(tickTask);
}
void TaskSystem::updateIntervalTasks(Clock& clock) {
    float dtMs = clock.getDeltaTime() * 1000.0f;
    for (auto& t : tickTasks) {
        if (!t.active) continue;

        t.timeAccumulator += dtMs;
        if (t.timeAccumulator >= t.intervalMs.count()) {
            t.timeAccumulator -= t.intervalMs.count();
            enqueueTask(Task{ t.func });
            if (t.executionsRemaining > 0) {
                t.executionsRemaining--;
                if (t.executionsRemaining == 0)
                    t.active = false;
            }
        }
    }
}
