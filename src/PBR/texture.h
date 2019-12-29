#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include "resources.h"
#include <stb_image/stb_image.h>
#include <vector>

class Texture
{
	unsigned int textureID;
	int sizeX;
	int sizeY;
	unsigned char* data;
	GLenum format;
	bool cubeMap;
public:
	Texture(glm::vec2 _viewport, int _mipmaps); 
	Texture(const std::string _text, bool _HDR);
	Texture(const std::vector<std::string> _text); //empty
	~Texture();
	void loadTexture(const std::string _text, bool _HDR);
	unsigned int getID();
	bool getCubeMap();

};
#endif