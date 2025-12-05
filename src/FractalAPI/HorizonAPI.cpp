#include "HorizonAPI.h"
#include "FractalAPI.h" // heavy include только в cpp

HorizonAPI& HorizonAPI::instance() {
    static HorizonAPI inst;
    return inst;
}

void HorizonAPI::run() { FractalAPI::instance().run(); }
void HorizonAPI::initialize() { FractalAPI::instance().initialize(); }
void HorizonAPI::shutdown() { FractalAPI::instance().shutdown(); }
void HorizonAPI::stop() { FractalAPI::instance().stop(); }

Entity HorizonAPI::createEntity() {
    return FractalAPI::instance().createEntity();
}

void HorizonAPI::processEvents(){
    FractalAPI::instance().processEvents();
}
void HorizonAPI::registerIntervalTask(const TickTask& tickTask){
    FractalAPI::instance().registerIntervalTask(tickTask);
}
template<typename T>
SystemECS<T>& HorizonAPI::system() {
    return FractalAPI::instance().system<T>();
}
template<typename T>
void HorizonAPI::addComponent(size_t maxEntities) {
    FractalAPI::instance().addECScomponent<T>(maxEntities);
}

template<typename T>
void HorizonAPI::attachComponent(Entity e, const T& data) {
    FractalAPI::instance().attachECSCompToEntity<T>(e, data);
}
template<typename T, typename Func>
void HorizonAPI::updateSystem(Func&& func) {
    FractalAPI::instance().updateSystem<T>(std::forward<Func>(func));
}



template<typename EventType, typename... Args>
void HorizonAPI::pushEvent(Args&&... args) {
    FractalAPI::instance().pushEvent<EventType>(std::forward<Args>(args)...);
}

template<typename EventType>
void HorizonAPI::subscribe(std::function<void(const EventType&)> callback) {
    FractalAPI::instance().subscribe<EventType>(callback);
}

template<typename EventType>
void HorizonAPI::emitEvent(const EventType& e) {
    FractalAPI::instance().emitEvent<EventType>(e);
}


