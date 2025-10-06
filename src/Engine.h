#pragma once

#include <string>

class Engine {
public:
    Engine();
    ~Engine();


    bool init(); // initialize engine subsystems. Return true on success
    bool initECS(); // initialize ECS (placeholder)
    //TODO: do realization to this method in Engine.cpp


    void run(); // run the main loop (placeholder)
    void update(); // update ECS and other subsystems (placeholder)
    //TODO: do realization to this method in Engine.cpp



    void shutdown(); // cleanup resources
    void cleanup(); // cleanup ECS & other subsystems (placeholder)
    void saveState(const std::string& filename); // save engine state to a file (placeholder)
    //TODO: do realization to this method in Engine.cpp
    void loadState(const std::string& filename); // load engine state from a file (placeholder)
    //TODO: do realization to this method in Engine.cpp


private:
    bool initialized_;
    bool ecs_initialized_;
    // add other subsystems as needed (graphics, input, audio, etc.)
};
