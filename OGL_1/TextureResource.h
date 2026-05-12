#pragma once
#include "Utils/NonCopyableNonMovable.h"
#include "Descriptors/TextureDesc.h"
#include "Definitions.h"
#include <GL/glew.h>

namespace dj
{
class TextureManager;
} // namespace dj

namespace dj
{
/*! \todo
		- In future test if TextureDesc should be split into other vector (for optimization)
*/
class TextureResource : private NonCopyable
{
	friend TextureManager;		// for clear() method

	static GLuint globalActiveID;
	GLuint id;
	TextureDesc desc;
	ColorRGBA borderColor;
public:

	TextureResource();
	TextureResource(TextureResource&&) = default;
	TextureResource& operator=(TextureResource&&) = default;
	~TextureResource();

	void bind();
	void unbind();

	// Image data setters
	bool transferData2D(GLenum sourceColorFormat, GLenum sourceValueType, const void* data);
	bool transferCubeSide(GLenum side, GLenum sourceColorFormat, GLenum sourceValueType, const void* data);

	// Format / sampling setters - automatically binds texture and updates in OpenGL
	void setBorderColor(const ColorRGBA& color);
	void setWrapping(GLenum s, GLenum t, GLenum r);
	void setFiltering(GLenum min, GLenum mag);

	// Getters
	TextureGLType getType() const;
	const TextureDesc& getDescriptor() const;
	const ResolutionDesc& getResolution() const;
	const TextureSamplingDesc& getSamplingDesc() const;

	unsigned int estimateSizeInVRAM() const;

private:
	// available only for TextureManager
	bool recreate(const TextureDesc& desc);		// deletes ID, and creates new one	
	void clear();								// deletes ID

	// GL updates
	void setBorderColor();
	void setWrapping();
	void setFiltering();
};

} // namespace dj