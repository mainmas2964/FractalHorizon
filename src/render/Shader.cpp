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
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "could not open file: " << path << std::endl;
        return;
    }
    std::stringstream stream;
    stream << file.rdbuf();
    out = stream.str();
    file.close();
};
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
    unsigned int shader;
    int success;
    char infoLog[512];
    const char* shaderSource;
    std::string shaderSourceStr_;
    openFile(shaderSourcePath, shaderSourceStr_);
    shaderSource = shaderSourceStr_.c_str();
    shader = glCreateShader(GL_FRAGMENT_SHADER); // for now only fragment shader
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    // check for compile errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glAttachShader(ID_, shader);
    glLinkProgram(ID_);
    glDeleteShader(shader);

}
void Shader::compile(const char* vertexSourcePath, const char* fragmentSourcePath) {
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    const char* vertexSource;
    const char* fragmentSource;
    std::string vertexSourceStr_;
    std::string fragmentSourceStr_;
    openFile(vertexSourcePath, vertexSourceStr_);
    openFile(fragmentSourcePath, fragmentSourceStr_);
    vertexSource = vertexSourceStr_.c_str();
    fragmentSource = fragmentSourceStr_.c_str();


    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    // check for compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    // check for compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader Program
    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);
    // check for linking errors
    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID_, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as theyre linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID_, name.c_str()), 1, glm::value_ptr(value));
}
