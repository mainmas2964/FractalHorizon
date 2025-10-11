#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glad/glad.h>

class Shader {
public:
    Shader();
    ~Shader();

    void use();
    void compile(const char* vertexSourcePath, const char* fragmentSourcePath);
    void compileShader(const char* shaderSourcePath); // legacy: compile single fragment-only shader (kept for compatibility)

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;

    bool isValid() const;
    GLuint getID() const;

private:
    GLuint ID_;
};

// helper visible to header users (used in inline isValid)
static inline bool programIsLinked(GLuint id) {
    if (!id) return false;
    if (!glIsProgram(id)) return false;
    GLint status = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    return status == GL_TRUE;
}

#endif // SHADER_H
