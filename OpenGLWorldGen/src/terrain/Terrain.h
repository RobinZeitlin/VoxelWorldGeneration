#pragma once

#include <vector>

#include <glfw3.h>
#include <glad/glad.h>

#include <glm.hpp>

class Terrain {
public:
	Terrain(glm::vec2 spawnPos);
	~Terrain();

	void generate_terrain(int width, int height);
	void addVoxel(glm::vec2 rawPos, glm::vec3 color);
	float get_height(int x, int y);

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	glm::vec2 spawnPos;

private:
	float voxelSize = 0.5f;
	int width = 50;
	int height = 50;
};