#ifndef _SHADER_H_
#define _SHADER_H_
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <iostream>


class Shader
{
	unsigned int ID;
public:
	Shader(const std::string& source);
	~Shader();
	void active();
	void setInt(const std::string& _name, int _value);
	void setFloat(const std::string &_name, float _value);

	void setBool(const std::string &_name, bool _value);

	void setVec2(const std::string &_name, glm::vec2 _value);
	void setVec3(const std::string &_name, glm::vec3 _value);
	void setVec4(const std::string &_name, glm::vec4 _value);
	
	

};


#endif // !_SHADER_H_

