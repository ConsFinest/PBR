#include "resources.h"

std::shared_ptr<Resources> Resources::initializeOpenGL()
{
	std::shared_ptr<Resources> rtn = std::make_shared<Resources>();
	if (glewInit() != GLEW_OK)
	{
		throw std::exception();
	}
	return rtn;
}

Resources::~Resources()
{
}
