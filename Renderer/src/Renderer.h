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
#include "DrawType.h"
#include "rendering/TextureLoader.h"
#include "rendering/OBJLoader.h"

class Shader;
class Camera;

class Renderer {
public:
    Renderer(int width, int height);

    void render();

    void init_renderer();
    void terminate();
    void input();

    TextureLoader* textureManager;

    std::vector<MeshRenderer*> meshes;

    GLFWwindow* get_window() { return window; }
    Camera* get_camera() { return camera; }

    DrawType currentDrawMode = DrawType::Fill;

    Shader* defaultShader;
private:

    GLFWwindow* window = nullptr;

    const int width, height;

    Camera* camera;
};
