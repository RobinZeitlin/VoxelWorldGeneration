#include "Game.h"

#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "src/Transform.h"
#include "camera/Camera.h"

Game::Game(GLFWwindow* window, Renderer* renderer) 
	: window(window), renderer(renderer) 
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Game::update()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	process_input(window);
}

void Game::process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	renderer->get_camera()->inputs(window, deltaTime);
}
