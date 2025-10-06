#include <iostream>
#include "Engine.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "start/test.h"
int main(int argc, char** argv) {
    //test12

    std::cout << "FractalHorizon3DEngine starting..." << std::endl;

    Engine engine;
    if (!engine.init()) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return 1;
    }

    engine.run();
    engine.shutdown();

    std::cout << "FractalHorizon3DEngine exiting and close." << std::endl;
    execute1(); // run ECS test
    return 0;
}
