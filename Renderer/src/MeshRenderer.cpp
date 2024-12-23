#include "MeshRenderer.h"

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
MeshRenderer::MeshRenderer(Shader* shader)
	: shader(shader) {
	vao = new VAO();
	vao->Bind();

	vbo = new VBO(vertices, sizeof(vertices));
	ebo = new EBO(indices, sizeof(indices));

	vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	vao->Unbind();
	ebo->Unbind();
	vbo->Unbind();
}

MeshRenderer::~MeshRenderer() {
	destroy();
}

void MeshRenderer::draw() {
	shader->Activate();

	vao->Bind();
	ebo->Bind();

	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
}

void MeshRenderer::destroy() {
	vao->Delete();
	vbo->Delete();
	ebo->Delete();
}