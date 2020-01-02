#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include <iostream>
#include "resources.h"

class RenderTexture
{
	unsigned int Fbo; //frameBufferObject
	unsigned int Rbo; //renderBufferObject
	unsigned int ID; //textureID


public:
	RenderTexture();
	~RenderTexture();
	unsigned int GetFbo(glm::vec2 _storage);
	void setViewPort(glm::vec2 _view);
	void resetViewPort();
	void setTexture(unsigned int _ID);
	unsigned int getTexture();
};

#endif // !
