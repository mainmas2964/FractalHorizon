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
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (!in) {
        std::cerr << "[Shader] openFile: failed to open '" << path << "'\n";
        out.clear();
        return;
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    out = ss.str();

    // Remove UTF-8 BOM if present
    if (out.size() >= 3 &&
        static_cast<unsigned char>(out[0]) == 0xEF &&
        static_cast<unsigned char>(out[1]) == 0xBB &&
        static_cast<unsigned char>(out[2]) == 0xBF) {
        out.erase(0, 3);
        std::cerr << "[Shader] openFile: removed UTF-8 BOM from '" << path << "'\n";
    }

    // Ensure there's a trailing newline (some drivers are picky)
    if (out.empty() || out.back() != '\n') out.push_back('\n');

    std::cerr << "[Shader] openFile: read '" << path << "' (" << out.size() << " bytes)\n";
}

Shader::Shader() : ID_(0) {}

Shader::~Shader() {
    if (ID_ != 0) {
        glDeleteProgram(ID_);
    }
}

static bool programIsLinked(GLuint id) {
    if (!id) return false;
    if (!glIsProgram(id)) return false;
    GLint status = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    return status == GL_TRUE;
}

void Shader::use() {
    if (!programIsLinked(ID_)) {
      //  std::cerr << "[Shader] Attempt to use unlinked program (ID=" << ID_ << ")\n";
        return;
    }
    glUseProgram(ID_);
}

// helper to get uniform location safely
static GLint getUniformLocationSafe(GLuint program, const char* name) {
    if (!programIsLinked(program)) {
        std::cerr << "[Shader] getUniformLocationSafe: program not linked (ID=" << program << ") for uniform '" << name << "'\n";
        return -1;
    }
    GLint loc = glGetUniformLocation(program, name);
    if (loc == -1) {
        std::cerr << "[Shader] Warning: uniform '" << name << "' not found or optimized out in program " << program << "\n";
    }
    return loc;
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
    } else {
        std::cout << "[shader] program linked successfully (ID = " << ID_ << ")\n";
    }

    glDeleteShader(shader);
}

void Shader::compile(const char* vertexSourcePath, const char* fragmentSourcePath) {
    std::string vertexSourceStr, fragmentSourceStr;
    openFile(vertexSourcePath, vertexSourceStr);
    openFile(fragmentSourcePath, fragmentSourceStr);

    if (vertexSourceStr.empty()) {
        std::cerr << "[Shader] compile: vertex shader file empty or not found: '" << vertexSourcePath << "'\n";
        ID_ = 0;
        return;
    }
    if (fragmentSourceStr.empty()) {
        std::cerr << "[Shader] compile: fragment shader file empty or not found: '" << fragmentSourcePath << "'\n";
        ID_ = 0;
        return;
    }

    const char* vptr = vertexSourceStr.c_str();
    const char* fptr = fragmentSourceStr.c_str();
    GLint vlen = static_cast<GLint>(vertexSourceStr.size());
    GLint flen = static_cast<GLint>(fragmentSourceStr.size());

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vptr, &vlen);
    glCompileShader(vertex);

    GLint vertSuccess = GL_FALSE;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertSuccess);
    if (!vertSuccess) {
        GLint len = 0;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &len);
        std::string log(len > 0 ? len : 1, '\0');
        glGetShaderInfoLog(vertex, len, nullptr, &log[0]);
        std::cerr << "[Shader] Vertex compilation failed for '" << vertexSourcePath << "':\n" << log << std::endl;
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fptr, &flen);
    glCompileShader(fragment);

    GLint fragSuccess = GL_FALSE;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &fragSuccess);
    if (!fragSuccess) {
        GLint len = 0;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &len);
        std::string log(len > 0 ? len : 1, '\0');
        glGetShaderInfoLog(fragment, len, nullptr, &log[0]);
        std::cerr << "[Shader] Fragment compilation failed for '" << fragmentSourcePath << "':\n" << log << std::endl;
    }

    if (!vertSuccess || !fragSuccess) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        ID_ = 0;
        return;
    }

    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);

    GLint linkSuccess = GL_FALSE;
    glGetProgramiv(ID_, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        GLint len = 0;
        glGetProgramiv(ID_, GL_INFO_LOG_LENGTH, &len);
        std::string log(len > 0 ? len : 1, '\0');
        glGetProgramInfoLog(ID_, len, nullptr, &log[0]);
        std::cerr << "[Shader] Program linking failed:\n" << log << std::endl;
        glDeleteProgram(ID_);
        ID_ = 0;
    } else {
        std::cout << "[shader] program linked successfully (ID = " << ID_ << ")\n";
    }
    

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::setBool(const std::string& name, bool value) const {
    GLint loc = glGetUniformLocation(ID_, name.c_str());
    if (loc == -1) return;
    glUniform1i(loc, (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    GLint loc = glGetUniformLocation(ID_, name.c_str());
    if (loc == -1) return;
    glUniform1i(loc, value);
}

void Shader::setFloat(const std::string& name, float value) const {
    GLint loc = glGetUniformLocation(ID_, name.c_str());
    if (loc == -1) return;
    glUniform1f(loc, value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    GLint loc = glGetUniformLocation(ID_, name.c_str());
    if (loc == -1) return;
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    GLint loc = glGetUniformLocation(ID_, name.c_str());
    if (loc == -1) return;
    glUniform2fv(loc, 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    GLint loc = glGetUniformLocation(ID_, name.c_str());
    if (loc == -1) return;
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    GLint loc = glGetUniformLocation(ID_, name.c_str());
    if (loc == -1) return;
    glUniform4fv(loc, 1, glm::value_ptr(value));
}
