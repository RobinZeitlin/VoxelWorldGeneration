#pragma once

#include "buffers/EBO.h"
#include "buffers/VAO.h"
#include "buffers/VBO.h"

#include "glm.hpp"

class Shader;
class Renderer;

class MeshRenderer {
public:
	MeshRenderer(Shader* shader, GLfloat* vertices, size_t verticesSize, GLuint* indices, size_t indicesSize);
	~MeshRenderer();

	void draw();
	void destroy();

	Renderer* renderer;

protected:
	EBO* ebo = nullptr;
	VAO* vao = nullptr;
	VBO* vbo = nullptr;

	Shader* shader;

	GLfloat* vertices;
	GLuint* indices;

	size_t verticesSize;
	size_t indicesSize;
};