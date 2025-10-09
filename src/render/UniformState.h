#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Shader.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct UniformState {
    glm::vec2 resolution;
    float radius;
    glm::vec3 color;
    glm::mat4 mat4; 

    void initDefault();
    void upload(const Shader& shader); // передает автоматически да я ленивый
};
