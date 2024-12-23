#include "Renderer.h"

#include "camera/Camera.h"
#include "shaders/Shader.h"

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

	defaultShader = new Shader("vertex.shader", "fragment.shader");
    mesh = new MeshRenderer(defaultShader);

    glEnable(GL_DEPTH_TEST);

    camera = new Camera(500, 500, glm::vec3(1.0f, 0.0f, 2.0f));
}

void Renderer::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera->Matrix(45.0f, 0.1f, 100.0f, *defaultShader, "camMatrix");

    mesh->draw();

    glfwSwapBuffers(window);
}

void Renderer::terminate() {
	defaultShader->Delete();
    delete mesh;
    glfwTerminate();
}