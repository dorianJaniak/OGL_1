#pragma once
#include "DefinitionsGL.h"
#include "Camera.h"
#include <GL/glew.h>

namespace dj {
	class Light;
} // namespace dj

namespace dj {

class ShadowFramebuffer : public Camera {
	FramebufferID index;
	TextureID textureIndex;
	unsigned int width;
	GLenum setupStatus;
public:
	ShadowFramebuffer(unsigned width);
	~ShadowFramebuffer();

	void configureCamera(const Camera& camera, float fovIncreseDeg = 0.0f);
	void configureCamera(const Light& light, float near, float far, float fovIncreaseDeg = 0.0f);

	void bind() const;

	FramebufferID getIndex() const;
	TextureID getTextureIndex() const;
	GLenum getFramebufferStatus() const;
	unsigned int getWidth() const;

private:
	void setup();
};

} // namespace dj