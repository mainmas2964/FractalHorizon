#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    // defaults: position, up, yaw, pitch
    Camera(
        glm::vec3 pos = glm::vec3(0.0f, 10.0f, 30.0f),
        glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f
    );

    ~Camera(){};

    // matrices / vectors
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getUp() const;
    glm::vec3 getDirection() const;
    glm::vec3 getRight() const;

    // setters
    void setPosition(const glm::vec3& pos);
    void setLookAt(const glm::vec3& dir); // dir = target - position (normalized inside)
    void setUp(const glm::vec3& upVec);

    // perspective control
    void setPerspective(float fovDeg, float aspect, float nearPlane, float farPlane);
    void setAspect(float aspect);

    // input-style controls
    enum class Move { Forward, Backward, Left, Right, Up, Down };
    void processKeyboard(Move mv, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);

private:
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front;   // normalized look direction
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler angles
    float yaw;
    float pitch;

    // perspective params
    float fovDeg;
    float aspect;
    float nearPlane;
    float farPlane;

    // control params
    float movementSpeed;
    float mouseSensitivity;
};