#pragma once

#include <string>

class Engine {
public:
    Engine();
    ~Engine();

    // Initialize engine subsystems. Return true on success.
    bool init();

    // Run the main loop (placeholder).
    void run();

    // Cleanup resources.
    void shutdown();

private:
    bool initialized_;
};
