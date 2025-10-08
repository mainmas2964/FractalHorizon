#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Shader.h"

struct UniformState {
    glm::vec2 resolution;
    float radius;
    glm::vec3 color;

    void initDefault();
    void upload(const Shader& shader) const; // передает автоматически да я ленивый
};
