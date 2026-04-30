#include "TextureContainer.h"
using namespace dj;

TextureContainer::TextureContainer()
{
}

void TextureContainer::tagTexture(TexturePtr tex, TextureTag tag)
{
	taggedTextures[tag].push_back(tex);
}

bool TextureContainer::deleteTextureTag(TexturePtr tex, TextureTag tag)
{
	std::vector<TexturePtr>& tt = taggedTextures[tag];
	auto it = std::find(tt.begin(), tt.end(), tex);

	if (it != tt.end())
	{
		tt.erase(it);
		return true;
	}

	return false;
}

bool TextureContainer::deleteTexture(TexturePtr tex)
{
	for (auto& [tag, tt] : taggedTextures)
	{
		deleteTextureTag(tex, tag);
	}

	auto it = std::find(textures.begin(), textures.end(), tex);

	if (it == textures.end())
	{
		textures.erase(it);
		return true;
	}

	return false;
}

const std::vector<TexturePtr>& TextureContainer::getTextures(TextureTag tag) const
{
	return taggedTextures.at(tag);
}

unsigned int TextureContainer::getTexturesCount(TextureTag tag) const
{
	return static_cast<unsigned int>(taggedTextures.at(tag).size());
}

unsigned int TextureContainer::getTexturesSize()
{
	unsigned int size = 0u;
	for (const TexturePtr& tex : textures)
	{
		size += tex->getSizeInVram();
	}
	return size;
}

unsigned int TextureContainer::getTexturesSize(TextureTag tag) const
{
	unsigned int size = 0u;

	for (const TexturePtr& tex : getTextures(tag))
	{
		size += tex->getSizeInVram();
	}
	return size;
}