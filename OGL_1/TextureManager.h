#pragma once
#include "TextureReferencesManager.h"
#include "Utils/NonCopyableNonMovable.h"
#include "TextureHandle.h"
#include "TextureResource.h"
#include "Definitions.h"
#include <vector>
#include <string>
#include <functional>
#include <optional>

namespace dj
{
struct TextureDesc;
} // namespace dj

namespace dj
{

/*!	\class TextureManager
* \todo consider if it is worth to track how many TextureManager objects are created 
*	(NOTE: Current implementation is not safe for more than 1 instance)
*/
class TextureManager : public TextureReferencesManager, private NonCopyable, private NonMovable
{
	//! \todo consider if TextureManager would benefit from having State (OnlyRead / Modifying)?
	//enum class State
	//{
	//	OnlyRead,
	//	Modifying
	//};

	std::vector<std::string> paths;
	std::vector<TextureResource> textures;
	std::vector<unsigned int> freeSlots;

public:
	TextureManager() noexcept = default;
	~TextureManager() noexcept = default;

	std::optional<TextureHandle> createEmptyTexture(const TextureDesc& desc);
	std::optional<TextureHandle> create2DFromFile(const TextureDesc& desc, const char* path, bool flip = false);
	std::optional<TextureHandle> createCubeMapFromFile(const TextureDesc& desc, const char* path, bool flip = false);

	// Operate on Texture
	bool check(const TextureHandle& handle, std::function<bool(const TextureResource&)> fun) const;
	bool execute(const TextureHandle& handle, std::function<bool(TextureResource&)> fun);

	// Get size
	unsigned int getCount() const;
	unsigned int getSizeInVRAM() const;

	// Check if Texture exists / check if TextureHandle is correct
	bool exists(const TextureHandle& handle) const;
	bool verifyTextureDescriptor(const TextureDesc& desc) const;

	// Simplified access to TextureResources
	bool setBorderColor(const TextureHandle& handle, const ColorRGBA& color);
	bool setWrapping(const TextureHandle& handle, GLenum s, GLenum t, GLenum r);
	bool setFiltering(const TextureHandle& handle, GLenum min, GLenum far);
	std::optional<TextureGLType> getType(const TextureHandle& handle) const;
	std::optional<TextureDesc> getDescriptor(const TextureHandle& handle) const;
	std::optional<ResolutionDesc> getResolution(const TextureHandle& handle) const;
	std::optional<TextureSamplingDesc> getSamplingDesc(const TextureHandle& handle) const;

private:
	bool isOk(const TextureHandle& handle) const;
	void deleteUnused();

	unsigned int getFirstFreeSlot() const;
};

} // namespace dj