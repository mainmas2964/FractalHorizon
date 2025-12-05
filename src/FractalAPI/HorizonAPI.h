#pragma once
#include <functional>
#include "Structs&Classes.h"
class HorizonAPI {
public:
    static HorizonAPI& instance();

    void run();
    void initialize();
    void shutdown();
    void stop();

    template<typename T>
    void addComponent(size_t maxEntities = 1024);

    
    void enququeTask(const Task& task);

    void enququeTaskAsync(const Task& task);

    void registerIntervalTask(const TickTask& tickTask);

    template<typename T>

    SystemECS<T>& system();

    void processEvents();
    template<typename T>
    void attachComponent(Entity e, const T& data);

    template<typename T, typename Func>
    void updateSystem(Func&& func);

    Entity createEntity();

    template<typename EventType, typename... Args>
    void pushEvent(Args&&... args);

    template<typename EventType>
    void subscribe(std::function<void(const EventType&)> callback);

    template<typename EventType>
    void emitEvent(const EventType& e);




private:
    HorizonAPI() = default;
};
