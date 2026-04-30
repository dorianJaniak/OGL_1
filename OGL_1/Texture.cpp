#include "Texture.h"
#include <string>
#include <cassert>
using namespace dj;

Texture::Texture(TextureGLType type)
	: width(0u)
	, height(0u)
	, glType(type)
	, wrapS(GL_REPEAT)
	, wrapT(GL_REPEAT)
	, wrapR(GL_REPEAT)
	, minFilter(GL_LINEAR)
	, magFilter(GL_LINEAR)
	, sizeInVram(0u)
{
	borderColor[0] = 0.0f;
	borderColor[1] = 0.0f;
	borderColor[2] = 0.0f;
	borderColor[3] = 0.0f;

	if (type == GL_TEXTURE_2D)
	{
		setDefaultsFor2D();
	}
	else if (type == GL_TEXTURE_CUBE_MAP)
	{
		setDefaultsForCubemap();
	}

	glGenTextures(1, &id);
}

Texture::Texture(Texture&& tex) noexcept
	: id(tex.id)
	, width(tex.width)
	, height(tex.height)
	, glType(tex.glType)
	, wrapS(tex.wrapS)
	, wrapT(tex.wrapT)
	, wrapR(tex.wrapR)
	, minFilter(tex.minFilter)
	, magFilter(tex.magFilter)
	, sizeInVram(tex.sizeInVram)
{
	tex.id = 0u;
	tex.width = 0u;
	tex.height = 0u;
	tex.glType = GL_NONE;
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::bind()
{
	glBindTexture(glType, id);
}

void Texture::unbind()
{
	glBindTexture(glType, 0u);
}

void Texture::setSize(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
}

void Texture::setDefaultsFor2D()
{
	wrapS = GL_REPEAT;
	wrapT = GL_REPEAT;
	minFilter = GL_LINEAR_MIPMAP_LINEAR;
	magFilter = GL_LINEAR;
}

void Texture::setDefaultsForCubemap()
{
	wrapS = GL_CLAMP_TO_EDGE;
	wrapT = GL_CLAMP_TO_EDGE;
	wrapR = GL_CLAMP_TO_EDGE;
	minFilter = GL_LINEAR;
	magFilter = GL_LINEAR;
}

void Texture::setWrapping(GLenum wrapS, GLenum wrapT, GLenum wrapR)
{
	this->wrapS = wrapS;
	this->wrapT = wrapT;
	this->wrapR = wrapR;
	setWrapping();
}

void Texture::setFiltering(GLenum minFilter, GLenum magFilter)
{
	this->minFilter = minFilter;
	this->magFilter = magFilter;
	setFiltering();
}

void Texture::setBorderColor(float r, float g, float b, float a)
{
	borderColor[0] = r;
	borderColor[1] = g;
	borderColor[2] = b;
	borderColor[3] = a;
	setBorderColor();
}

void Texture::setBorderColor(const float color[4])
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		borderColor[i] = color[i];
	}
	setBorderColor();
}

bool Texture::transferData2D(GLenum inGPUColorFormat, GLenum sourceColorFormat, GLenum sourceValueType, const void* data, bool genMipMaps)
{
	assert(glType == GL_TEXTURE_2D);
	if (glType != GL_TEXTURE_2D || width == 0u || height == 0u)
	{
		return false;
	}

	GLenum error;
	glGetError();

	bind();
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		toCString(error);
		return false;
	}

	glTexImage2D(glType, 0, inGPUColorFormat, width, height, 0, sourceColorFormat, sourceValueType, data);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		toCString(error);
		return false;
	}

	sizeInVram = texturePixelSizeEstimation(inGPUColorFormat) * width * height / 8u + 1;
	if (genMipMaps)
	{
		glGenerateMipmap(glType);
		sizeInVram = static_cast<unsigned int>(static_cast<float>(sizeInVram) * 1.3333f);
	}

	return true;
}

bool Texture::transferDataCubeSide(GLenum side, GLenum inGPUColorFormat, GLenum sourceColorFormat, GLenum sourceValueType, const void* data, bool genMipMaps)
{
	if (glType != GL_TEXTURE_CUBE_MAP || width == 0u || width != height)
	{
		return false;
	}

	GLenum error;
	glGetError();

	bind();
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		toCString(error);
		return false;
	}

	glTexImage2D(side, 0, inGPUColorFormat, width, height, 0, sourceColorFormat, sourceValueType, data);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		toCString(error);
		return false;
	}

	sizeInVram = 6u * texturePixelSizeEstimation(inGPUColorFormat) * width * height / 8u + 1u;
	if (genMipMaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		sizeInVram = static_cast<unsigned int>(static_cast<float>(sizeInVram) * 1.3333f);
	}

	return true;
}

TextureID Texture::getID() const
{
	return id;
}

unsigned int Texture::getWidth() const
{
	return width;
}
unsigned int Texture::getHeight() const
{
	return height;
}

unsigned int Texture::getSizeInVram() const
{
	return sizeInVram;
}

TextureTypeInfo Texture::getTextureTypeInfo() const
{
	return { id, glType };
}

void Texture::setWrapping()
{
	bind();
	glTexParameteri(glType, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(glType, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(glType, GL_TEXTURE_WRAP_R, wrapR);
}

void Texture::setFiltering()
{
	bind();
	glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::setBorderColor()
{
	bind();
	glTexParameterfv(glType, GL_TEXTURE_BORDER_COLOR, borderColor);
}

inline unsigned int Texture::texturePixelSizeEstimation(GLenum inGPUColorFormat)
{
	switch (inGPUColorFormat)
	{
	case GL_RED:
		return 8u;
	case GL_RG:
		return 16u;
	case GL_RGB:
	case GL_SRGB8:
	case GL_RGB8_SNORM:
		return 24u;
	case GL_RGBA:
	case GL_SRGB8_ALPHA8:
	case GL_SRGB_ALPHA:
		return 32u;
	}

	return 32u;
}