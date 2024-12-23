#pragma once

#include "buffers/EBO.h"
#include "buffers/VAO.h"
#include "buffers/VBO.h"

class Shader;

class MeshRenderer {
public:
	MeshRenderer(Shader* shader);
	~MeshRenderer();

	void draw();
	void destroy();

protected:
	EBO* ebo = nullptr;
	VAO* vao = nullptr;
	VBO* vbo = nullptr;

	Shader* shader;
};