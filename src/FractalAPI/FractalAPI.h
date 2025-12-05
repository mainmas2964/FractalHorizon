// Created by mainmasgoose [mr.slimmy] 
#pragma once
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <any>

#include "ECS/entity/entity.h"
#include "ECS/component/component.h"
#include "taskSystem/taskSystem.h"
#include "Clock.h"
#include "ECS/system/system.h"
#include "eventManager.h"
//TODO: сделать namespac'ы для АПИ
class FractalAPI {
public:
    static FractalAPI& instance() {
        static FractalAPI inst;
        return inst;
    }

    void run();
    void initEntityManager();
    void initialize();
    void initEngineClock();
    void initEventBus();
    void shutdown();
    void initTaskSystem();
    TaskSystem& getTaskSystem();
    void enququeTaskAsync(const Task& task);
    void registerIntervalTask(const TickTask& tickTask);
    void updateIntervalTasks();
    void stop();
    template<typename EventType, typename... Args>
    void pushEvent(Args&&... args){
        eventBus->pushEvent<EventType>(std::forward<Args>(args)...);
    };
    void processEvents();
    template<typename EventType>
    void subscribe(std::function<void(const EventType&)> callback){
        eventBus->subscribe<EventType>([callback](const Event& e){
            callback(static_cast<const EventType&>(e));
        });
    };
    template<typename EventType>
    void emitEvent(const EventType& e){
        eventBus->emit(e);
    }; //ONLY make it sync with the one thread, no multithreading 

    Clock& getEngineClock();


    // == ECS Methods ==
    template<typename T>
    void addECScomponent(size_t maxEntities) {
        std::type_index typeId(typeid(T));
        if (components.find(typeId) != components.end()) {
            std::cout << "component already created\n";
            return;
        }
        components[typeId] = std::make_any<Component<T>>(maxEntities);
        std::cout << "component created: " << typeid(T).name() << std::endl;
    }

    template<typename T>
    Component<T>* getECScomponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return &std::any_cast<Component<T>&>(it->second);
        }
        std::cout << "component not found: " << typeid(T).name() << std::endl;
        return nullptr;
    }

    template<typename T>
    SystemECS<T>& system() {
    std::type_index typeId(typeid(T));
    if (systems.find(typeId) == systems.end()) {
        Component<T>* comp = getECScomponent<T>();
        if (!comp) throw std::runtime_error("component not found");
        systems[typeId] = std::make_any<SystemECS<T>>(*comp);
    }
    return std::any_cast<SystemECS<T>&>(systems.at(typeId));
    }

    template<typename T, typename Func>
    void updateSystemParallel(Func&& func, size_t chunkSize = 64) {
        SystemECS<T>& sys = system<T>();
        sys.updateParallel(taskSystem.get(), std::forward<Func>(func), chunkSize);
    }
    template<typename T, typename Func>
    void updateSystem(Func&& func) {
        SystemECS<T>& sys = system<T>();
        sys.update(std::forward<Func>(func));
    }
    template<typename T, typename Func>
    void updateSingleCompSystem(Func&& func, Entity e) {
        SystemECS<T>& sys = system<T>();
        sys.updateSingle(std::forward<Func>(func), e);
    }

    template<typename T>
    void attachECSCompToEntity(Entity e, const T& componentData) {
        Component<T>* comp = getECScomponent<T>();
        if (!comp) throw std::runtime_error("component not found");
        comp->addComponent(e, componentData);
    }

    Entity createEntity();
    void getECSentity(uint32_t id = 0);

private:

    bool initialized = false;
    std::unique_ptr<TaskSystem> taskSystem = nullptr;
    std::unique_ptr<Clock> engineClock = nullptr;
    std::unique_ptr<EntityManager> entityManager = nullptr;
    std::unique_ptr<EventBus> eventBus = nullptr;
    bool taskSystemInitialized = false;
    bool entityManagerInitialized = false;
    bool engineClockInitialized = false;
    bool eventBusInitialized = false;
    bool running = false;

    FractalAPI() = default;
    ~FractalAPI() = default;

    std::unordered_map<std::type_index, std::any> systems;
    std::unordered_map<std::type_index, std::any> components;

};

