#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 upVec, float yaw_, float pitch_)
    : position(pos),
      worldUp(upVec),
      yaw(yaw_),
      pitch(pitch_),
      fovDeg(45.0f),
      aspect(16.0f/9.0f),
      nearPlane(0.1f),
      farPlane(1000.0f),
      movementSpeed(30.0f),
      mouseSensitivity(0.1f)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fovDeg), aspect, nearPlane, farPlane);
}

glm::vec3 Camera::getPosition() const {
    return position;
}
glm::vec3 Camera::getUp() const {
    return up;
}
glm::vec3 Camera::getDirection() const {
    return front;
}
glm::vec3 Camera::getRight() const {
    return right;
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}
void Camera::setLookAt(const glm::vec3& dir) {

    front = glm::normalize(dir);

    yaw = glm::degrees(atan2(front.z, front.x)) - 90.0f;
    pitch = glm::degrees(asin(front.y));
    updateCameraVectors();
}
void Camera::setUp(const glm::vec3& upVec) {
    worldUp = glm::normalize(upVec);
    updateCameraVectors();
}

void Camera::setPerspective(float fovDeg_, float aspect_, float nearP, float farP) {
    fovDeg = fovDeg_;
    aspect = aspect_;
    nearPlane = nearP;
    farPlane = farP;
}
void Camera::setAspect(float aspect_) {
    aspect = aspect_;
}

void Camera::processKeyboard(Move mv, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (mv == Move::Forward) position += front * velocity;
    if (mv == Move::Backward) position -= front * velocity;
    if (mv == Move::Left) position -= right * velocity;
    if (mv == Move::Right) position += right * velocity;
    if (mv == Move::Up) position += worldUp * velocity;
    if (mv == Move::Down) position -= worldUp * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    fovDeg -= yoffset;
    if (fovDeg < 1.0f) fovDeg = 1.0f;
    if (fovDeg > 90.0f) fovDeg = 90.0f;
}

void Camera::updateCameraVectors() {
    // new front vector
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);

    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}
