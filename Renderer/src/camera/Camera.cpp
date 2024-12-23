#include "Camera.h"
#include "../shaders/Shader.h"

Camera::Camera(int width, int height, glm::vec3 position) {
    this->width = width;
    this->height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, worldUp);
    projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, uniform),
        1,
        GL_FALSE,
        glm::value_ptr(projection * view));
}

void Camera::inputs(GLFWwindow* window, float dt) {
    float speedWithDt = speed * dt;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += forward * speedWithDt;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Position -= forward * speedWithDt;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Position -= right * speedWithDt;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Position += right * speedWithDt;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Position += worldUp * speedWithDt;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        Position -= worldUp * speedWithDt;
    }

    update_orientation(window, dt);
}

void Camera::update_orientation(GLFWwindow* window, float dt) {
    static double lastX = width / 2.0, lastY = height / 2.0;
    double mouseX, mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);

    float deltaX = mouseX - lastX;
    float deltaY = lastY - mouseY;

    yaw += deltaX * sensitivity;
    pitch += deltaY * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Orientation = glm::normalize(forward);

    right = glm::normalize(glm::cross(Orientation, worldUp));
    up = glm::normalize(glm::cross(right, Orientation));

    glfwSetCursorPos(window, width / 2.0, height / 2.0);

    lastX = width / 2.0;
    lastY = height / 2.0;
}

