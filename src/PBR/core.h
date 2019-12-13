#ifndef _CORE_H_
#define _CORE_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <exception>
#include "resources.h"
#include "shader.h"

class Shader;
class Resources;

class Core
{
	SDL_Window *window;
	std::shared_ptr <Resources> resources;
public:
	std::weak_ptr<Core> self;
	~Core();
	static std::shared_ptr<Core> init();
	

};





#endif _CORE_H_