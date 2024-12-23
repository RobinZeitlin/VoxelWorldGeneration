#include "MeshRenderer.h"

#include "shaders/Shader.h"
#include "Renderer.h"

MeshRenderer::MeshRenderer(Shader* shader, GLfloat* vertices, size_t verticesSize, GLuint* indices, size_t indicesSize)
	: shader(shader), vertices(vertices), indices(indices), verticesSize(verticesSize), indicesSize(indicesSize) {
	vao = new VAO();
	vao->Bind();

	vbo = new VBO(vertices, verticesSize);
	ebo = new EBO(indices, indicesSize);

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

	glDrawElements(renderer->currentDrawMode, indicesSize / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

void MeshRenderer::destroy() {
	delete[] vertices;
	delete[] indices;

	vao->Delete();
	vbo->Delete();
	ebo->Delete();
}