#include "FractalAPI.h"

void FractalAPI::initialize(){
    if(!initialized){
        std::cout << "FractalAPI initializing..." << std::endl;
        initTaskSystem();
        initEntityManager();
        initEngineClock();
        initEventBus();
        initialized = true;
        std::cout << "FractalAPI initialized" << std::endl;
    } else {
        std::cout << "FractalAPI already initialized" << std::endl;
    }
}
void FractalAPI::initTaskSystem(){
    if(!taskSystemInitialized){
        taskSystem = std::make_unique<TaskSystem>();
        taskSystemInitialized = true;
        std::cout << "Task System initialized" << std::endl;
    } else {
        std::cout << "Task System already initialized" << std::endl;
    }
}
void FractalAPI::processEvents(){
    eventBus->processEvents();
}
void FractalAPI::initEntityManager(){
    if(!entityManagerInitialized){
        entityManagerInitialized = true;
        entityManager = std::make_unique<EntityManager>();
    }
}void FractalAPI::initEventBus() {
    if(!eventBusInitialized){
        eventBus = std::make_unique<EventBus>();
        eventBusInitialized = true;
        std::cout << "Event bus imitialized" << std::endl;
    } else {
        std::cout << "Event Bus already initialized" << std::endl;
    }
}
TaskSystem& FractalAPI::getTaskSystem(){
    return *taskSystem;
}
void FractalAPI::initEngineClock(){
    if(!engineClockInitialized){
        engineClock = std::make_unique<Clock>();
        engineClockInitialized = true;
        std::cout << "Engine Clock initialized" << std::endl;
    } else {
        std::cout << "Engine Clock already initialized" << std::endl;
    }
}
Entity FractalAPI::addECSentity(){
    return entityManager->createEntity();

}
void FractalAPI::getECSentity(uint32_t id){
    entityManager->getEntityById(id);
}
void FractalAPI::shutdown(){
    if(initialized){
        std::cout << "Shutting down FractalAPI..." << std::endl;
        if(taskSystemInitialized){
            taskSystem.reset();
            std::cout << "Task System shut down" << std::endl;
        }
        if(engineClockInitialized){
            engineClock.reset();
            std::cout << "Engine Clock shut down" << std::endl;
        }
        if(entityManagerInitialized){
            entityManager.reset();
            std::cout << "Entity Manager shut down" << std::endl;
        }
        if(eventBusInitialized){
            eventBus.reset();
            std::cout << "Event Bus shut down" << std::endl;
        }
        engineClockInitialized = false;
        entityManagerInitialized = false;
        taskSystemInitialized = false;
        initialized = false;
        running = false;
        std::cout << "FractalAPI shut down" << std::endl;
    } else {
        std::cout << "FractalAPI is not initialized" << std::endl;
    }
}
void FractalAPI::enququeTaskAsync(const Task& task) {
    if (taskSystem) {
        taskSystem->enqueueTask(task);
    } else {
        std::cout << "Task System is not initialized" << std::endl;
    }
}
void FractalAPI::registerIntervalTask(const TickTask& tickTask) {
    if (taskSystem) {
        taskSystem->registerIntervalFunc(tickTask);
    } else {
        std::cout << "Task System is not initialized" << std::endl;
    }
}
Clock& FractalAPI::getEngineClock() {
    if (engineClock) {
        return *engineClock;
    } else {
        throw std::runtime_error("Engine Clock is not initialized");
    }
}
void FractalAPI::run() {
    if (!initialized) {
        std::cout << "FractalAPI is not initialized, call initialize() first" << std::endl;
        return;
    }

    if (running) {
        std::cout << "FractalAPI is already running..." << std::endl;
        return;
    }

    running = true;
    std::cout << "FractalAPI started running..." << std::endl;

    while (running) {
        engineClock->update();
        taskSystem->updateIntervalTasks(*engineClock);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "FractalAPI run loop ended" << std::endl;
}

void FractalAPI::stop() {
    running = false;
}
