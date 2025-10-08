#include "Shader.h"
#include "glad/glad.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

void openFile(const char* path, std::string& out) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Shader] Could not open file: " << path << std::endl;
        return;
    }
    std::stringstream stream;
    stream << file.rdbuf();
    out = stream.str();
    file.close();
}

Shader::Shader() : ID_(0) {}

Shader::~Shader() {
    if (ID_ != 0) {
        glDeleteProgram(ID_);
    }
}

void Shader::use() {
    if (ID_ != 0) {
        glUseProgram(ID_);
    }
}

void Shader::compileShader(const char* shaderSourcePath) {
    std::string shaderSourceStr;
    openFile(shaderSourcePath, shaderSourceStr);
    const char* shaderSource = shaderSourceStr.c_str();

    unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER); // Only fragment for now
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "[Shader] Fragment compilation failed:\n" << infoLog << std::endl;
    }

    ID_ = glCreateProgram();
    glAttachShader(ID_, shader);
    glLinkProgram(ID_);

    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID_, 512, nullptr, infoLog);
        std::cerr << "[Shader] Program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(shader);
}

void Shader::compile(const char* vertexSourcePath, const char* fragmentSourcePath) {
    std::string vertexSourceStr, fragmentSourceStr;
    openFile(vertexSourcePath, vertexSourceStr);
    openFile(fragmentSourcePath, fragmentSourceStr);
    const char* vertexSource = vertexSourceStr.c_str();
    const char* fragmentSource = fragmentSourceStr.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "[Shader] Vertex compilation failed:\n" << infoLog << std::endl;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "[Shader] Fragment compilation failed:\n" << infoLog << std::endl;
    }

    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);

    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID_, 512, nullptr, infoLog);
        std::cerr << "[Shader] Program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}
