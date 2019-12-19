#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

#include <iostream>
#include "resources.h"

class RenderTexture
{
	unsigned int Fbo; //frameBufferObject
	unsigned int Rbo; //renderBufferObject
	unsigned int ID;


public:
	RenderTexture();
	~RenderTexture();
	unsigned int GetFbo();
	void setViewPort(glm::vec4 _view);
	void resetViewPort();
};

#endif // !
