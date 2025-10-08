#ifndef SHADER_H_
#define SHADER_H_
#include "glad/glad.h"
#include <string>
#include <glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
class Shader {
public:
    Shader();
    ~Shader();
    void use();
    void compile(const char* vertexSource, const char* fragmentSource);
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    

private:
    unsigned int ID_;

};


#endif