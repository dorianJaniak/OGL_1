#include "ShadowFramebuffer.h"
#include "Light.h"
#include <iostream>
using namespace dj;

ShadowFramebuffer::ShadowFramebuffer(unsigned width)
	: Camera()
	, width(width)
{ 
	setup();
}

ShadowFramebuffer::~ShadowFramebuffer()
{
	glDeleteTextures(1, &textureIndex);
	glDeleteFramebuffers(1, &index);

	std::clog << "ShadowFramebuffer destructor called\n";
}

void ShadowFramebuffer::configureCamera(const Camera& cam, float fovIncreseDeg)
{
	// copy all data from cam to this camera
	Camera::Camera(cam);
	setFOV(cam.getFOV() + fovIncreseDeg);

	// Shadows are computed only in 1:1 proportion
	setAspectRatio(1.0f);
}

void ShadowFramebuffer::configureCamera(const Light& light, float near, float far, float fovIncreaseDeg)
{
	setPosition(light.getPosition());
	setRotation(glm::vec3(light.tmpXPitch, light.tmpYYaw, 0.0f));
	setPerspective(light.getSpotlightAngle() + fovIncreaseDeg, 1.0f, near, far);
}

void ShadowFramebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, index);
}

FramebufferID ShadowFramebuffer::getIndex() const
{
	return index;
}

TextureID ShadowFramebuffer::getTextureIndex() const
{
	return textureIndex;
}

GLenum ShadowFramebuffer::getFramebufferStatus() const
{
	return setupStatus;
}

unsigned int ShadowFramebuffer::getWidth() const
{
	return width;
}

void ShadowFramebuffer::setup()
{
	static const float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glGenFramebuffers(1, &index);
	glBindFramebuffer(GL_FRAMEBUFFER, index);

	glGenTextures(1, &textureIndex);
	glBindTexture(GL_TEXTURE_2D, textureIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureIndex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	setupStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}