#include <iostream>

#include "Game.h"
#include "Renderer.h"

#include "btBulletDynamicsCommon.h"

extern Game* game = nullptr;

#define BASE_SCREEN_WIDTH 900
#define BASE_SCREEN_HEIGHT 900

int main()
{
	Renderer* renderer = new Renderer(
		BASE_SCREEN_WIDTH, 
		BASE_SCREEN_HEIGHT);

	renderer->init_renderer();

	game = new Game(
		renderer->get_window(), 
		renderer);

	btDefaultCollisionConfiguration collisionConfiguration;

	while (!glfwWindowShouldClose(renderer->get_window())) {
		renderer->render();

		game->update();

		glfwPollEvents();
	}

	renderer->terminate();

	delete game;
	delete renderer;

	return 0;
}
