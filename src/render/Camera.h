#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera
{
    public:
    Camera(){};
    ~Camera(){};
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getUp() const;
    glm::vec3 getDirection() const;
    glm::vec3 getRight() const;
    void setPosition(const glm::vec3& pos);
    void setLookAt(const glm::vec3& dir);
    void setUp(const glm::vec3& upVec);

    private:
    glm::vec3 position;

    glm::vec3 lookAt;

    glm::vec3 up;

  
};