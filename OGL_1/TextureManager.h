#pragma once
#include "ReferencesManager.h"
#include "Utils/NonCopyableNonMovable.h"
#include "Handle.h"
#include "TextureResource.h"
#include "Enums/TextureParameterEnums.h"
#include "Definitions.h"
#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <set>

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
class TextureManager : public ReferencesManager, private NonCopyable, private NonMovable
{
	//! \todo consider if TextureManager would benefit from having State (OnlyRead / Modifying)?
	//enum class State
	//{
	//	OnlyRead,
	//	Modifying
	//};

	std::vector<std::string> paths;
	std::vector<TextureResource> textures;
	std::set<unsigned int> freeSlots;

public:
	struct CubeSideMapping
	{
		TextureCubeSide side;
		std::string suffix;
	};

	TextureManager() noexcept = default;
	~TextureManager() noexcept = default;

	std::optional<Handle> createEmptyTexture(const TextureDesc& desc);
	std::optional<Handle> create2DFromFile(const TextureSamplingDesc& sampling, const char* path, bool generateMipMaps = false, bool flip = false, bool internalSRGB = false);
	std::optional<Handle> createCubeMapFromFile(const TextureSamplingDesc& sampling, const char* pathPrefix, const std::array<CubeSideMapping, 6>& pathSuffixes, bool generateMipMaps = false, bool flip = false, bool internalSRGB = false);

	// Operate on Texture
	bool check(const Handle& handle, std::function<bool(const TextureResource&)> fun) const;
	bool execute(const Handle& handle, std::function<bool(TextureResource&)> fun);
	bool bind(const Handle& handle) const;

	// Get size
	unsigned int getCount() const;
	unsigned int getSizeInVRAM() const;

	// Check if Texture exists / check if Handle is correct
	bool exists(const Handle& handle) const;
	static bool verifyTextureDescriptor(const TextureDesc& desc);

	// Simplified access to TextureResources
	bool setBorderColor(const Handle& handle, const ColorRGBA& color);
	bool setWrapping(const Handle& handle, TextureWrapping s, TextureWrapping t, TextureWrapping r);
	bool setFiltering(const Handle& handle, TextureFilteringMin min, TextureFilteringMag mag);
	std::optional<TextureType> getType(const Handle& handle) const;
	std::optional<TextureDesc> getDescriptor(const Handle& handle) const;
	std::optional<ResolutionDesc> getResolution(const Handle& handle) const;
	std::optional<TextureSamplingDesc> getSamplingDesc(const Handle& handle) const;

	/*! \todo Consider other solution to avoid passing OpenGL Texture ID (like RenderBackend class which would be a friend of TextureResource) */
	std::optional<GLuint> getID(const Handle& handle) const;

	// Management methods
	unsigned int getReferencesCount(const Handle& handle) const;
	unsigned int forceDeleteUnused();

	// Debugging methods
	bool verifyConsistency() const;
	unsigned int getCellsCount() const;
	const std::vector<unsigned int>& getReferencesCountVector() const;
	const std::vector<unsigned int>& getGenerationsVector() const;

private:
	unsigned int deleteUnused();

	std::optional<Handle> addTexture(TextureResource&& res, const TextureDesc& desc, const char* path);
	std::optional<unsigned int> popFirstFreeSlot();

	std::optional<TextureFormatDesc> channelsCountToColorFormat(unsigned int count) const;
};

} // namespace dj