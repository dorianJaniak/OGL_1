#pragma once
#include "Utils/NonCopyableNonMovable.h"
#include "Descriptors/TextureDesc.h"
#include "Definitions.h"

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

	TextureResource() noexcept;
	TextureResource(TextureResource&&) noexcept;
	TextureResource& operator=(TextureResource&&) noexcept;
	~TextureResource();

	void bind();
	void unbind();
	static void unbindAllTypes();

	// Image data setters
	bool nullify();
	bool transferData2D(ColorFormatInSource sourceColorFormat, PixelDataTypeInSource sourceValueType, const void* data);
	bool transferCubeSide(TextureCubeSide side, ColorFormatInSource sourceColorFormat, PixelDataTypeInSource sourceValueType, const void* data);

	// Format / sampling setters - automatically binds texture and updates in OpenGL
	void setBorderColor(const ColorRGBA& color);
	void setWrapping(TextureWrapping s, TextureWrapping t, TextureWrapping r);
	void setFiltering(TextureFilteringMin min, TextureFilteringMag mag);

	// Getters
	TextureType getType() const;
	const TextureDesc& getDescriptor() const;
	const ResolutionDesc& getResolution() const;
	const TextureSamplingDesc& getSamplingDesc() const;

	unsigned int estimateSizeInVRAM() const;

private:
	// available only for TextureManager
	void recreate(const TextureDesc& desc);		// deletes ID, and creates new one
	void clear();								// deletes ID

	// GL updates
	void setBorderColor();
	void setWrapping();
	void setFiltering();
};

} // namespace dj