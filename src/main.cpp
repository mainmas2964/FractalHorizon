#include <iostream>
#include "Engine.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "FractalAPI.h"
    struct hp {
        int health;
    };
int main() {
    FractalAPI& api = FractalAPI::instance();
    api.initialize();
    TickTask task1 {
        .func = []() { std::cout << "Task 1 executed\n"; },
        .intervalMs = std::chrono::milliseconds(100),
        .lastExecutedMs = std::chrono::steady_clock::now(),
        .id = 1,
        .executionsRemaining = 1,
        .active = true
    };
    api.registerIntervalTask(task1);

    hp hp1{
        .health = 100
    };
    hp hp2{
        .health = 80
    };
    api.addECScomponent<hp>(100);
    Entity e1 = api.addECSentity();
    Entity e2 = api.addECSentity();
    api.attachECSCompToEntity<hp>(e1, hp1);
    api.attachECSCompToEntity<hp>(e2, hp2);
    SystemECS<hp>& hpSystem = api.system<hp>();
    api.updateSystem<hp>([](hp& component, Entity e) {
    component.health += 10;
    std::cout << "Entity " << e.id << " health: " << component.health << std::endl;
    });


    
    std::thread apiThread([&api]() { api.run(); });
    std::this_thread::sleep_for(std::chrono::seconds(10)); 
    api.stop();
    apiThread.join();
    api.shutdown();

}
