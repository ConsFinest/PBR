#include "core.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 760


Core::~Core()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

std::shared_ptr<Core> Core::init()
{
	std::shared_ptr<Core> core = std::make_shared<Core>();
	core->self = core;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::exception(); 
	}
	core->window = SDL_CreateWindow("PBR - DEMO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (!SDL_GL_CreateContext(core->window))
	{
		throw std::exception();
	}
	core->resources = Resources::initializeOpenGL();
	return core;
}

SDL_Window * Core::getWindow()
{
	return window;
}
