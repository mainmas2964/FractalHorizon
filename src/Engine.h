#pragma once

#include <string>

class Engine {
public:
    Engine();
    ~Engine();

    
    bool init(); // initialize engine subsystems. Return true on success
    bool initECS(); // initialize ECS (placeholder)


    void run(); // run the main loop (placeholder)
    void update(); // update ECS and other subsystems (placeholder)



    void shutdown(); // cleanup resources


private:
    bool initialized_;
    bool ecs_initialized_;
    // add other subsystems as needed (graphics, input, audio, etc.)
};
