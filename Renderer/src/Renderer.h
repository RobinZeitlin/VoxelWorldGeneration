#pragma once

#include <iostream>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <glfw3.h>

#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtc/matrix_transform.hpp> 
#include <gtc/type_ptr.hpp>

#include "buffers/EBO.h"
#include "buffers/VAO.h"
#include "buffers/VBO.h"

#include "MeshRenderer.h"

class Shader;
class Camera;

class Renderer {
public:
    Renderer(int width, int height);

    void render();

    void init_renderer();
    void terminate();

    GLFWwindow* get_window() { return window; }
    Camera* get_camera() { return camera; }

private:
    MeshRenderer* mesh;

    GLFWwindow* window = nullptr;

    const int width, height;

    Shader* defaultShader;
    EBO* EBO1;
    VAO* VAO1;
    VBO* VBO1;

    Camera* camera;
};
