/*
created by mainmas2964  2025, 4 october


*/
#pragma once
#include "ECS/component/component.h"
#include "ECS/entity/entity.h"
#include "taskSystem/taskSystem.h"
template<typename T>
class SystemECS {
public:
    SystemECS(Component<T>& c) : component(c) {}

    template<typename Func>
    void update(Func&& func, int iterationStep = 1) {
        for (size_t i = 0; i < component.comp.size(); i += iterationStep) {
            Entity e{ component.dense[i] };
            func(component.comp[i], e);
        }
    }

    template<typename Func>
    void updateSingle(Func&& func, Entity e) {
        if (component.hasComponent(e)) {
            func(component.getComponent(e));
        }
    }

    template<typename Func>
    void updateParallel(TaskSystem& tasksystem, Func&& func, size_t chunkSize = 64) {
        size_t total = component.comp.size();
        size_t numChunks = (total + chunkSize - 1) / chunkSize;
        std::atomic<size_t> chunksCompleted(0);

        for (size_t chunk = 0; chunk < numChunks; chunk++) {
            size_t chunkStart = chunk * chunkSize;
            size_t chunkEnd = std::min(chunkStart + chunkSize, total);

            tasksystem.enqueueTask(Task{
                [this, chunkStart, chunkEnd, &func, &chunksCompleted]() {
                    for (size_t i = chunkStart; i < chunkEnd; i++) {
                        func(this->component.comp[i], this->component.dense[i]);
                    }
                    chunksCompleted.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
        while (chunksCompleted.load(std::memory_order_relaxed) < numChunks) {
            std::this_thread::yield();
        }
    }

private:
    Component<T>& component;
};
