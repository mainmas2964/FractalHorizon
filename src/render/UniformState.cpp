#include "UniformState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
void UniformState::initDefault() {
    resolution = glm::vec2(1280.0f, 720.0f);
    radius = 0.4f;
    color = glm::vec3(1.0f, 0.2f, 0.1f);
    mat4 = glm::mat4(1.0f);
}

void UniformState::upload(const Shader& shader) {
    shader.setVec2("u_resolution", resolution);
    shader.setFloat("u_radius", radius);
    shader.setVec3("u_color", color);
    shader.setMat4("u_trans", mat4);
    mat4 = glm::rotate(mat4, glm::radians(0.5f), glm::vec3(0.5f, 0.5f, 0.5f));   
}
