#include "Game.h"

#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "src/Transform.h"
#include "camera/Camera.h"

Game::Game(GLFWwindow* window, Renderer* renderer) 
	: window(window), renderer(renderer) 
{

}
void Game::update()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	processInput(window);
}

void Game::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	renderer->get_camera()->Inputs(window, deltaTime);
}
