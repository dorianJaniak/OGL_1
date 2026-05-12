#include "TextureResource.h"
using namespace dj;

TextureResource::TextureResource()
	: id(0u)
	, desc{}
	, borderColor{0.0f, 0.0f, 0.0f, 0.0f}
{
	desc.glType = GL_TEXTURE_2D;
}

TextureResource::~TextureResource()
{
	clear();
}

void TextureResource::bind()
{
	if (id != globalActiveID)
	{
		glBindTexture(desc.glType, id);
		globalActiveID = id;
	}
}

void TextureResource::unbind()
{
	glBindTexture(desc.glType, 0u);
	globalActiveID = 0u;
}

bool TextureResource::transferData2D(GLenum sourceColorFormat, GLenum sourceValueType, const void* data)
{
	const ResolutionDesc& res = desc.resolution;
	if (desc.glType != GL_TEXTURE_2D || res.width == 0u || res.height == 0u)
	{
		return false;
	}

	GLenum error;
	glGetError();

	bind();
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		//toCString(error);
		return false;
	}

	glTexImage2D(desc.glType, 0, desc.format.inGPUColorFormat, res.width, res.height, 0, sourceColorFormat, sourceValueType, data);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		//toCString(error);
		return false;
	}

	//sizeInVram = texturePixelSizeEstimation(inGPUColorFormat) * width * height / 8u + 1;
	if (desc.mipmaps)
	{
		glGenerateMipmap(desc.glType);
		//sizeInVram = static_cast<unsigned int>(static_cast<float>(sizeInVram) * 1.3333f);
	}

	return true;
}

bool TextureResource::transferCubeSide(GLenum side, GLenum sourceColorFormat, GLenum sourceValueType, const void* data)
{
	const ResolutionDesc& res = desc.resolution;

	if (desc.glType != GL_TEXTURE_CUBE_MAP || res.width == 0u || res.width != res.height)
	{
		return false;
	}

	GLenum error;
	glGetError();

	bind();
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		//toCString(error);
		return false;
	}

	glTexImage2D(side, 0, desc.format.inGPUColorFormat, res.width, res.height, 0, sourceColorFormat, sourceValueType, data);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		//toCString(error);
		return false;
	}

	//sizeInVram = 6u * texturePixelSizeEstimation(inGPUColorFormat) * width * height / 8u + 1u;
	if (desc.mipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		//sizeInVram = static_cast<unsigned int>(static_cast<float>(sizeInVram) * 1.3333f);
	}

	return true;
}

void TextureResource::setBorderColor(const ColorRGBA& color)
{
	borderColor = color;
	setBorderColor();
}

void TextureResource::setWrapping(GLenum s, GLenum t, GLenum r)
{
	desc.sampling.wrapS = s;
	desc.sampling.wrapT = t;
	desc.sampling.wrapR = r;
	setWrapping();
}

void TextureResource::setFiltering(GLenum min, GLenum mag)
{
	desc.sampling.minFilter = min;
	desc.sampling.magFilter = mag;
	setFiltering();
}

TextureGLType TextureResource::getType() const
{
	return desc.glType;
}

const TextureDesc& TextureResource::getDescriptor() const
{
	return desc;
}

const ResolutionDesc& TextureResource::getResolution() const
{
	return desc.resolution;
}

const TextureSamplingDesc& TextureResource::getSamplingDesc() const
{
	return desc.sampling;
}

unsigned int TextureResource::estimateSizeInVRAM() const
{
	return 0u;
}

bool TextureResource::recreate(const TextureDesc& desc)
{
	if (id)
	{
		clear();
	}

	this->desc = desc;
	
	glGenTextures(1, &id);
	bind();
	setWrapping();
	setFiltering();
	setBorderColor();
}

void TextureResource::clear()
{
	glDeleteTextures(1, &id);
	desc = TextureDesc{};
	id = 0u;
	borderColor = ColorRGBA{};
}

void TextureResource::setBorderColor()
{
	bind();
	glTexParameterfv(desc.glType, GL_TEXTURE_BORDER_COLOR, borderColor.data());
}

void TextureResource::setWrapping()
{
	bind();
	glTexParameteri(desc.glType, GL_TEXTURE_WRAP_S, desc.sampling.wrapS);
	glTexParameteri(desc.glType, GL_TEXTURE_WRAP_T, desc.sampling.wrapT);
	glTexParameteri(desc.glType, GL_TEXTURE_WRAP_R, desc.sampling.wrapR);
}

void TextureResource::setFiltering()
{
	bind();
	glTexParameteri(desc.glType, GL_TEXTURE_MIN_FILTER, desc.sampling.minFilter);
	glTexParameteri(desc.glType, GL_TEXTURE_MAG_FILTER, desc.sampling.magFilter);
}