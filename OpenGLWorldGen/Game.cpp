#include "Game.h"

#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "src/Transform.h"
#include "camera/Camera.h"

#include "MeshRenderer.h"
#include "src/terrain/Terrain.h"

Game::Game(GLFWwindow* window, Renderer* renderer) 
	: window(window), renderer(renderer) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
Game::~Game() {
}
void Game::update() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	process_input(window);
}

void Game::process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		Terrain* newTerrain = new Terrain();
		MeshRenderer* newMesh = new MeshRenderer(
			renderer->defaultShader, 
			newTerrain->vertices.data(),
			newTerrain->vertices.size() * sizeof(GLfloat),
			newTerrain->indices.data(),
			newTerrain->indices.size() * sizeof(GLuint)
		);

		renderer->mesh = newMesh;
		renderer->mesh->renderer = renderer;
	}

	renderer->get_camera()->inputs(window, deltaTime);
	renderer->input();
}
