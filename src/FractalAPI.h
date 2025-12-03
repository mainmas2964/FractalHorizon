// Created by mainmasgoose [mr.slimmy] 
#pragma once
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <any>
#include <memory>

#include "ECS/entity/entity.h"
#include "ECS/component/component.h"
#include "taskSystem/taskSystem.h"
#include "Clock.h"
#include "ECS/system/system.h"

class FractalAPI {
public:
    static FractalAPI& instance() {
        static FractalAPI instance;
        return instance;
    }

    void initialize();
    void shutdown();
    void initTaskSystem();
    void getTaskSystem();

    // == ECS Methods ==
    template<typename T>
    void addECScomponent(size_t maxEntities) {
        std::type_index typeId(typeid(T));
        if (components.find(typeId) != components.end()) {
            std::cout << "component already created\n";
            return;
        }

        components[typeId] = Component<T>(maxEntities);
        std::cout << "component created: " << typeid(T).name() << std::endl;
    }

    template<typename T>
    Component<T>* getECScomponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return &std::any_cast<Component<T>&>(it->second);
        } else {
            std::cout << "component not found: " << typeid(T).name() << std::endl;
            return nullptr;
        }
    }

    template<typename T>
    SystemECS<T>& system() {
        std::type_index typeId(typeid(T));

        if (systems.find(typeId) == systems.end()) { 
            Component<T>* comp = getECScomponent<T>();
            if (!comp) throw std::runtime_error("Component not found");
            systems[typeId] = SystemECS<T>(*comp);
        }

        return std::any_cast<SystemECS<T>&>(systems[typeId]);
    }
    template<typename T, typename Func>
    void updateSystemParallel(Func&& func, size_t chunkSize = 64) {
    SystemECS<T>& sys = system<T>();
    sys.updateParallel(taskSystem, std::forward<Func>(func), chunkSize);
    }


    void addECSentity();

private:
    bool initialized = false;
    TaskSystem* taskSystem = nullptr;
    Clock* engineClock = nullptr;
    bool running = false;
    bool taskSystemInitialized = false;

    FractalAPI() = default;
    ~FractalAPI() = default;

    std::unordered_map<std::type_index, std::any> systems;
    std::unordered_map<std::type_index, std::any> components;
};
