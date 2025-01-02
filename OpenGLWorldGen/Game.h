#pragma once

#include "Renderer.h"

#include <glm.hpp>

class Transform;
class TerrainManager;

class Game {
public:
	Game(GLFWwindow* window, Renderer* renderer);
	~Game();

	void update();
	
	void process_input(GLFWwindow* window);

private:
	GLFWwindow* window = nullptr;
	Renderer* renderer = nullptr;
	TerrainManager* terrainManager = nullptr;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	bool spawned = false;
};