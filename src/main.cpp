#include <iostream>
#include "Engine.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "start/test.h"
int main() {
    //test12
    std::cout << "FractalHorizon3DEngine starting..." << std::endl;
    std::cout << "FractalHorizon3DEngine exiting and close." << std::endl;
    execute1(); // run ECS test
    return 0;
}
