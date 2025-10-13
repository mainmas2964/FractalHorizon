#include "UniformState.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void UniformState::initDefault() {
    resolution = glm::vec2(1280.0f, 720.0f);
    radius = 0.4f;
    color = glm::vec3(1.0f, 0.2f, 0.1f);
    mat4 = glm::mat4(1.0f);
    uMVP = glm::mat4(1.0f);
}

void UniformState::upload(const Shader& shader) {
    // upload existing uniforms (shader methods already check for program validity)
    shader.setVec2("u_resolution", resolution);
    shader.setFloat("u_radius", radius);
    shader.setVec3("u_color", color);
    shader.setMat4("u_trans", mat4);

    // upload new MVP (used by chunk vertex shader)
    shader.setMat4("uMVP", uMVP);

    // optional: small animation of u_trans for other effects
    mat4 = glm::rotate(mat4, glm::radians(0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
}
