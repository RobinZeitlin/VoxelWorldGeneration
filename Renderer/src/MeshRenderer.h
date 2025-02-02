#pragma once

#include "buffers/EBO.h"
#include "buffers/VAO.h"
#include "buffers/VBO.h"

#include "rendering/Texture.h"

#include "glm.hpp"

class Shader;
class Renderer;

class MeshRenderer {
public:
	MeshRenderer(Shader* shader, GLfloat* vertices, size_t verticesSize, GLuint* indices, size_t indicesSize);
	~MeshRenderer();

	void draw();
	void apply_texture(Texture* newTexture);
	void destroy();

	Renderer* renderer;

	glm::vec3 position;
	void setPosition(glm::vec3 newPosition);

protected:
	void updateModelMatrix();

	EBO* ebo = nullptr;
	VAO* vao = nullptr;
	VBO* vbo = nullptr;

	Shader* shader;
	Texture* texture;

	GLfloat* vertices;
	GLuint* indices;

	size_t verticesSize;
	size_t indicesSize;

	glm::mat4 modelMatrix;
};