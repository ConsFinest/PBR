#ifndef _RESOURCES_H_
#define _RESOURCES_H_
#include <memory>
#include <GL/glew.h>
#include <exception>

class Shader;

class Resources
{
	int i;
	
public:
	static std::shared_ptr<Resources> initializeOpenGL();
	~Resources();

};







#endif // !_RESOURCES_H_
