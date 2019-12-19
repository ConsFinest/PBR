#ifndef _SHADER_H_
#define _SHADER_H_
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "core.h"

class Shader
{
	void pollErrors(unsigned int _shader, std::string _type);



public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec3(const std::string &name, glm::vec3 value) const;
	void setMat4(const std::string &name, glm::mat4 value) const;
};



#endif // !_SHADER_H_

