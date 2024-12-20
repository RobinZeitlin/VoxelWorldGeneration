#include "Renderer.h"

#include "camera/Camera.h"
#include "shaders/Shader.h"

GLfloat vertices[] = {
    // Positions               // Colors (Green)
    -0.5f, -0.5f, -0.5f,       0.0f, 1.0f, 0.0f,  // Bottom-left-front
     0.5f, -0.5f, -0.5f,       0.0f, 1.0f, 0.0f,  // Bottom-right-front
     0.5f,  0.5f, -0.5f,       0.0f, 1.0f, 0.0f,  // Top-right-front
    -0.5f,  0.5f, -0.5f,       0.0f, 1.0f, 0.0f,  // Top-left-front
    -0.5f, -0.5f,  0.5f,       0.0f, 1.0f, 0.0f,  // Bottom-left-back
     0.5f, -0.5f,  0.5f,       0.0f, 1.0f, 0.0f,  // Bottom-right-back
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

	defaultShader = new Shader("vertex.shader", "fragment.shader");

	VAO1 = new VAO();
	VAO1->Bind();

	VBO1 = new VBO(vertices, sizeof(vertices));
	EBO1 = new EBO(indices, sizeof(indices));

	VAO1->LinkAttrib(*VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1->LinkAttrib(*VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO1->Unbind();
	EBO1->Unbind();
	VBO1->Unbind();

    glEnable(GL_DEPTH_TEST);

    camera = new Camera(500, 500, glm::vec3(0.0f, 0.0f, 2.0f));
}

void Renderer::render() {
	defaultShader->Activate();

    camera->Matrix(45.0f, 0.1f, 100.0f, *defaultShader, "camMatrix");

	VAO1->Bind();
	EBO1->Bind();
	glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
}

void Renderer::terminate() {
	VAO1->Delete();
	VBO1->Delete();
	EBO1->Delete();
	defaultShader->Delete();
    glfwTerminate();
}