#include "UniformState.h"

void UniformState::initDefault() {
    resolution = glm::vec2(1280.0f, 720.0f);
    radius = 0.4f;
    color = glm::vec3(1.0f, 0.2f, 0.1f);
}

void UniformState::upload(const Shader& shader) const {
    shader.setVec2("u_resolution", resolution);
    shader.setFloat("u_radius", radius);
    shader.setVec3("u_color", color);
}
