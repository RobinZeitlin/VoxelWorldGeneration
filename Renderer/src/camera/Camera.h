#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <glfw3.h>
#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>

class Shader;

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    int width;
    int height;

    float speed = 10.0f;
    float sensitivity = 0.1f;

    float yaw = -90.0f;
    float pitch = 0.0f;

    Camera(int width, int height, glm::vec3 position);

    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
    void inputs(GLFWwindow* window, float dt);
    void update_orientation(GLFWwindow* window, float dt);
};
