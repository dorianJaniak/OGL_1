#pragma once
#include "DefinitionsGL.h"
#include <GL/glew.h>
#include <string>

namespace dj
{
using TextureGLType = GLenum;

struct TextureTypeInfo
{
	TextureID id;
	TextureGLType glType;

	TextureTypeInfo(TextureID id = 0u, TextureGLType glType = GL_TEXTURE_2D)
		: id(id)
		, glType(glType)
	{ }

	bool operator==(const TextureTypeInfo& tti) const
	{
		return (id == tti.id && glType == tti.glType);
	}
};

struct TextureBinding
{
	TextureTypeInfo typeInfo;
	std::string uniformName;
};

class Texture
{
	TextureID id;
	unsigned int width;
	unsigned int height;
	TextureGLType glType;
	GLenum wrapS, wrapT, wrapR;
	GLenum minFilter, magFilter;
	unsigned int sizeInVram; // bytes
	float borderColor[4];

public:
	constexpr static unsigned int invalidID = 0u;

	Texture(TextureGLType type);
	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept;
	~Texture();

	void bind();
	void unbind();
	void setSize(unsigned int width, unsigned int height);
	void setDefaultsFor2D();
	void setDefaultsForCubemap();
	void setWrapping(GLenum wrapS, GLenum wrapT, GLenum wrapR = GL_REPEAT);
	void setFiltering(GLenum minFilter, GLenum magFilter);
	void setBorderColor(float r, float g, float b, float a);
	void setBorderColor(const float color[4]);
	bool transferData2D(GLenum inGPUColorFormat, GLenum sourceColorFormat, GLenum sourceValueType, const void* data, bool genMipMaps = false);
	bool transferDataCubeSide(GLenum side, GLenum inGPUColorFormat, GLenum sourceColorFormat, GLenum sourceValueType, const void* data, bool genMipMaps = false);

	TextureID getID() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getSizeInVram() const;
	TextureTypeInfo getTextureTypeInfo() const;

private:
	void setWrapping();
	void setFiltering();
	void setBorderColor();
	inline unsigned int texturePixelSizeEstimation(GLenum inGPUColorFormat);
};

} // namespace dj