#include "Game.h"

#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "src/Transform.h"
#include "camera/Camera.h"

#include "MeshRenderer.h"
#include "src/terrain/Terrain.h"
#include "src/terrain/TerrainManager.h"

Game::Game(GLFWwindow* window, Renderer* renderer) 
	: window(window), renderer(renderer) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	terrainManager = new TerrainManager(renderer, renderer->get_camera());
	objLoader = new OBJLoader();
}
Game::~Game() {
}
void Game::update() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	process_input(window);
	terrainManager->check_nearby_chunks();
}

void Game::process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		Terrain* newTerrain = new Terrain(glm::vec2(0), renderer->defaultShader, renderer);
		MeshRenderer* newMesh = new MeshRenderer(
			renderer->defaultShader, 
			newTerrain->vertices.data(),
			newTerrain->vertices.size() * sizeof(GLfloat),
			newTerrain->indices.data(),
			newTerrain->indices.size() * sizeof(GLuint)
		);

		newMesh->apply_texture(renderer->textureManager->get_texture("grass.png"));
		
		newMesh->renderer = renderer;
		renderer->meshes.push_back(newMesh);

		terrainManager->chunksSpawned[glm::vec2(0)] = newTerrain;
	}

	renderer->get_camera()->inputs(window, deltaTime);
	renderer->input();
}
