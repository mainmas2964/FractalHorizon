#include "Camera.h"

glm::mat4 Camera::getViewMatrix() const {

    glm::mat4 view = glm::lookAt(position, position + lookAt, up);
    return view;
};
glm::mat4 Camera::getProjectionMatrix() const {
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    return projection;
};
glm::vec3 Camera::getPosition() const {
    return position;
};
glm::vec3 Camera::getUp() const {
    return up;
};
glm::vec3 Camera::getDirection() const {
    return lookAt;
};
glm::vec3 Camera::getRight() const {
    return glm::normalize(glm::cross(lookAt, up));
};
void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
};
void Camera::setLookAt(const glm::vec3& dir) {
    lookAt = glm::normalize(dir);
};
void Camera::setUp(const glm::vec3& upVec) {
    up = glm::normalize(upVec);
};
