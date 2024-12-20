#include <iostream>

#include "Game.h"
#include "Renderer.h"

extern Game* game = nullptr;

#define BASE_SCREEN_WIDTH 500
#define BASE_SCREEN_HEIGHT 500

int main()
{
	Renderer* renderer = new Renderer(
		BASE_SCREEN_WIDTH, 
		BASE_SCREEN_HEIGHT);

	renderer->init_renderer();

	game = new Game(
		renderer->get_window(), 
		renderer);


	while (!glfwWindowShouldClose(renderer->get_window())) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->render();
		game->update();

		glfwSwapBuffers(renderer->get_window());
		glfwPollEvents();
	}

	renderer->terminate();

	delete game;
	delete renderer;

	return 0;
}
