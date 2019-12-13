#include "shader.h"
#include "exception.h"

Shader::Shader(const std::string& source)
{
	GLuint vertId = 0;
	GLuint fragId = 0;
	int success = 0;
	const GLchar* src = NULL;

	std::string vertSrc = "";
	vertSrc += "#version 120\n";
	vertSrc += "#define VERTEX\n";
	vertSrc += source;
	src = vertSrc.c_str();

	vertId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertId, 1, &src, NULL);
	glCompileShader(vertId);
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length = 0;
		glGetShaderiv(vertId, GL_INFO_LOG_LENGTH, &length);

		std::vector<char> infoLog(length);
		glGetShaderInfoLog(vertId, length, NULL, &infoLog.at(0));

		glDeleteShader(vertId);

		std::string msg = &infoLog.at(0);
		std::cout << msg << std::endl;
		throw Exception(msg);
	}

	std::string fragSrc = "";
	fragSrc += "#version 120\n";
	fragSrc += "#define FRAGMENT\n";
	fragSrc += source;
	src = fragSrc.c_str();

	fragId = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragId, 1, &src, NULL);

	glCompileShader(fragId);

	glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		int length = 0;
		glGetShaderiv(fragId, GL_INFO_LOG_LENGTH, &length);

		std::vector<char> infoLog(length);
		glGetShaderInfoLog(fragId, length, NULL, &infoLog.at(0));

		glDeleteShader(fragId);

		std::string msg = &infoLog.at(0);
		std::cout << msg << std::endl;
		throw Exception(msg);
	}

	glAttachShader(ID, vertId);
	glAttachShader(ID, fragId);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);


	if (!success)
	{
		int length = 0;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);

		std::vector<char> infoLog(length);
		glGetProgramInfoLog(ID, length, NULL, &infoLog.at(0));
	
		std::string msg = &infoLog.at(0);
		throw Exception(msg);
	}

	glDetachShader(ID, vertId);

	glDetachShader(ID, fragId);

	glDeleteShader(vertId);
	
	glDeleteShader(fragId);
	
}


Shader::~Shader()
{
}

void Shader::active()
{
	glUseProgram(ID);
}

void Shader::setInt(const std::string & _name, int _value)
{
	glUniform1i(glGetUniformLocation(ID, _name.c_str()), (int)_value);
}

void Shader::setFloat(const std::string & _name, float _value)
{
	glUniform1i(glGetUniformLocation(ID, _name.c_str()), _value);
}

void Shader::setBool(const std::string & _name, bool _value)
{
	glUniform1f(glGetUniformLocation(ID, _name.c_str()), _value);
}

void Shader::setVec2(const std::string & _name, glm::vec2 _value)
{
	glUniform2fv(glGetUniformLocation(ID, _name.c_str()), _value);
}

void Shader::setVec3(const std::string & _name, glm::vec3 _value)
{
	glUniform3fv(glGetUniformLocation(ID, _name.c_str()), _value);
}

void Shader::setVec4(const std::string & _name, glm::vec4 _value)
{
	glUniform4fv(glGetUniformLocation(ID, _name.c_str()), _value);

}


