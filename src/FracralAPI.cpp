#include "FractalAPI.h"

void FractalAPI::initialize(){
    if(!initialized){
        std::cout << "FractalAPI initializing..." << std::endl;
        engineClock = new Clock();
        initTaskSystem();
        initialized = true;
        running = true;
        std::cout << "FractalAPI initialized" << std::endl;
    } else {
        std::cout << "FractalAPI already initialized" << std::endl;
    }
}
void FractalAPI::initTaskSystem(){
    if(!taskSystemInitialized){
        taskSystem = new TaskSystem();
        taskSystemInitialized = true;
        std::cout << "Task System initialized" << std::endl;
    } else {
        std::cout << "Task System already initialized" << std::endl;
    }
}

void FractalAPI::shutdown(){
    if(initialized){
        std::cout << "Shutting down FractalAPI..." << std::endl;
        if(taskSystemInitialized && taskSystem != nullptr){
            delete taskSystem;
            taskSystem = nullptr;
            taskSystemInitialized = false;
            std::cout << "Task System shut down" << std::endl;
        }
        if(engineClock != nullptr){
            delete engineClock;
            engineClock = nullptr;
        }
        initialized = false;
        running = false;
        std::cout << "FractalAPI shut down" << std::endl;
    } else {
        std::cout << "FractalAPI is not initialized" << std::endl;
    }
}
