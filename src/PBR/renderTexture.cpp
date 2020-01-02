#include "renderTexture.h"


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 760

RenderTexture::RenderTexture()
{
	glGenTextures(1, &ID);
	glGenFramebuffers(1, &Fbo);
	glGenRenderbuffers(1, &Rbo);
}

RenderTexture::~RenderTexture()
{
}

unsigned int RenderTexture::GetFbo(glm::vec2 _storage)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, Rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _storage.x, _storage.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	return Fbo;
}

void RenderTexture::setViewPort(glm::vec2 _view)
{
	glViewport(0,0,_view.x, _view.y);
}

void RenderTexture::resetViewPort()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void RenderTexture::setTexture(unsigned int _ID)
{
	ID = _ID;
}

unsigned int RenderTexture::getTexture()
{
	return ID;
}
