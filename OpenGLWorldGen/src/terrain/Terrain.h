#pragma once

#include <glm.hpp>
#include <glfw3.h>
#include <glad/glad.h>

#include <glm.hpp>
#include <vector>

#include "shaders/Shader.h"
#include "Renderer.h"

class Terrain {
public:
	Terrain(glm::vec2 spawnPos, Shader* shader, Renderer* renderer);
	~Terrain();

	void generate_terrain(int width, int height);
	void addVoxel(glm::vec2 rawPos);
	float get_height(int x, int y);

	void spawn_tree(glm::vec2 spawnPos);

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	std::vector<glm::vec3> spawnedTrees;

	glm::vec2 spawnPos;

	Shader* shader;
	Renderer* renderer;

private:
	float voxelSize = 1.0f;
	int width = 50;
	int height = 50;
};