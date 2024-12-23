#include "Renderer.h"

#include "camera/Camera.h"
#include "shaders/Shader.h"

GLfloat vertices[] = {
    // Positions               // Colors (Green)
    -0.5f, -0.5f, -0.5f,       1.0f, 1.0f, 0.0f,  // Bottom-left-front
     0.5f, -0.5f, -0.5f,       1.0f, 1.0f, 0.0f,  // Bottom-right-front
     0.5f,  0.5f, -0.5f,       0.0f, 1.0f, 0.0f,  // Top-right-front
    -0.5f,  0.5f, -0.5f,       0.0f, 1.0f, 0.0f,  // Top-left-front
    -0.5f, -0.5f,  0.5f,       1.0f, 1.0f, 0.0f,  // Bottom-left-back
     0.5f, -0.5f,  0.5f,       1.0f, 1.0f, 0.0f,  // Bottom-right-back
     0.5f,  0.5f,  0.5f,       0.0f, 1.0f, 0.0f,  // Top-right-back
    -0.5f,  0.5f,  0.5f,       0.0f, 1.0f, 0.0f   // Top-left-back
};

GLuint indices[] = {
    0, 1, 2,  2, 3, 0,   // Front face
    4, 5, 6,  6, 7, 4,   // Back face
    0, 1, 5,  5, 4, 0,   // Bottom face
    2, 3, 7,  7, 6, 2,   // Top face
    0, 3, 7,  7, 4, 0,   // Left face
    1, 2, 6,  6, 5, 1    // Right face

};

Renderer::Renderer(int width, int height) 
    : width(width), height(height) 
{  }

void Renderer::init_renderer() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Basic Renderer", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    defaultShader = new Shader("vertex.shader", "fragment.shader");
    mesh = new MeshRenderer(defaultShader, vertices, sizeof(vertices), indices, sizeof(indices));
    mesh->renderer = this;
    camera = new Camera(500, 500, glm::vec3(1.0f, 0.0f, 2.0f));
}

void Renderer::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera->Matrix(45.0f, 0.1f, 100.0f, *defaultShader, "camMatrix");

    mesh->draw();

    glfwSwapBuffers(window);
}

void Renderer::input() {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        currentDrawMode = DrawType::Fill;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        currentDrawMode = DrawType::Outline;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        currentDrawMode = DrawType::Points;
    }
}

void Renderer::terminate() {
	defaultShader->Delete();
    delete defaultShader;
    delete mesh;
    delete camera;

    glfwTerminate();
}