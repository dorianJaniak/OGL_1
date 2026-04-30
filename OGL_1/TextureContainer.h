#pragma once
#include "Texture.h"
#include "Definitions.h"
#include <vector>
#include <type_traits>

namespace dj
{

enum class TextureTag
{
	Texture2D,
	TextureCube,
	Depth2D,
	DepthCube,
	Framebuffer,
	File,
	Runtime,
	Other,
};

/*! \class TextureContainer
*	\brief Stores pointers to Texture handles.
*	It does not remove pointers automaticaly.
*	Currently it is used only for storing textures from files (2D and cubemaps)
* 
*	\todo
*	- Store Depth Textures (?)
*	- Store Framebuffer Textures (?)
*	- Manage textures lifetime
*/
class TextureContainer
{
	const std::vector<TexturePtr> empty;

	std::vector<TexturePtr> textures;
	std::unordered_map<dj::TextureTag, std::vector<TexturePtr>> taggedTextures;
public:

	TextureContainer();
	TextureContainer(const TextureContainer&) = delete;
	TextureContainer(TextureContainer&&) = delete;

	void tagTexture(TexturePtr tex, TextureTag tag);

	template <typename ...Tags>
	bool addTexture(Texture&& tex, Tags... texTags)
	{
		static_assert((std::is_same_v<Tags, TextureTag> && ...), "texTags needs to be TextureTag type");
		static_assert(sizeof... (texTags) > 0);

		TexturePtr texPtr = std::make_shared<Texture>(std::move(tex));
		textures.push_back(texPtr);

		for (TextureTag tag : {texTags...})
		{
			tagTexture(texPtr, tag);
		}

		return true;
	}

	bool deleteTextureTag(TexturePtr tex, TextureTag tag);
	bool deleteTexture(TexturePtr tex);

	const std::vector<TexturePtr>& getTextures(TextureTag tag) const;
	unsigned int getTexturesCount(TextureTag tag) const;
	
	// In B
	unsigned int getTexturesSize();
	unsigned int getTexturesSize(TextureTag tag) const;
};
} // namespace dj