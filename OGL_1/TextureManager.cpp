#include "TextureManager.h"
using namespace dj;

std::optional<TextureHandle> TextureManager::createEmptyTexture(const TextureDesc& desc)
{
	TextureResource tex;
	tex.recreate(desc);
}

std::optional<TextureHandle> TextureManager::create2DFromFile(const TextureDesc& desc, const char* path, bool flip)
{

}

std::optional<TextureHandle> TextureManager::createCubeMapFromFile(const TextureDesc& desc, const char* path, bool flip)
{

}

bool TextureManager::check(const TextureHandle& handle, std::function<bool(const TextureResource&)> fun) const
{
	if (exists(handle))
	{
		return fun(textures[handle.getIndex()]);
	}

	return false;
}

bool TextureManager::execute(const TextureHandle& handle, std::function<bool(TextureResource&)> fun)
{
	if (exists(handle))
	{
		return fun(textures[handle.getIndex()]);
	}

	return false;
}

unsigned int TextureManager::getCount() const
{
	return textures.size();
}

unsigned int TextureManager::getSizeInVRAM() const
{
	return 0u;
}

bool TextureManager::exists(const TextureHandle& handle) const
{
	return (handle.getIndex() < textures.size() && handle.getGeneration() == generations[handle.getIndex()]);
}

bool TextureManager::verifyTextureDescriptor(const TextureDesc& desc)
{
	return (desc.resolution.width > 0u && desc.resolution.height > 0u);
}

bool TextureManager::setBorderColor(const TextureHandle& handle, const ColorRGBA& color)
{
	if (exists(handle))
	{
		TextureResource& tex = textures[handle.getIndex()];
		tex.bind();
		tex.setBorderColor(color);

		return true;
	}

	return false;
}

bool TextureManager::setWrapping(const TextureHandle& handle, TextureWrapping s, TextureWrapping t, TextureWrapping r)
{
	if (exists(handle))
	{
		TextureResource& tex = textures[handle.getIndex()];
		tex.bind();
		tex.setWrapping(s, t, r);

		return true;
	}

	return false;
}

bool TextureManager::setFiltering(const TextureHandle& handle, TextureFilteringMin min, TextureFilteringMag mag)
{
	if (exists(handle))
	{
		TextureResource& tex = textures[handle.getIndex()];
		tex.bind();
		tex.setFiltering(min, mag);

		return true;
	}

	return false;
}

std::optional<TextureType> TextureManager::getType(const TextureHandle& handle) const
{
	if (exists(handle))
	{
		const TextureResource& tex = textures[handle.getIndex()];
		return { tex.getDescriptor().glType };
	}

	return std::nullopt;
}

std::optional<TextureDesc> TextureManager::getDescriptor(const TextureHandle& handle) const
{
	if (exists(handle))
	{
		const TextureResource& tex = textures[handle.getIndex()];
		return { tex.getDescriptor() };
	}

	return std::nullopt;
}
std::optional<ResolutionDesc> TextureManager::getResolution(const TextureHandle& handle) const
{
	if (exists(handle))
	{
		const TextureResource& tex = textures[handle.getIndex()];
		return { tex.getDescriptor().resolution };
	}

	return std::nullopt;
}
std::optional<TextureSamplingDesc> TextureManager::getSamplingDesc(const TextureHandle& handle) const
{
	if (exists(handle))
	{
		const TextureResource& tex = textures[handle.getIndex()];
		return { tex.getDescriptor().sampling };
	}

	return std::nullopt;
}

unsigned int TextureManager::deleteUnused()
{
	unsigned int count = 0u;

	for (unsigned int i = 0; i < referencesCount.size(); ++i)
	{
		if (referencesCount[i] == 0u)
		{
			generations[i] += 1u;
			textures[i].clear();
			paths[i] = "";
			freeSlots.insert(i);
			++count;
		}
	}

	return count;
}

std::optional<unsigned int> TextureManager::popFirstFreeSlot()
{
	if (!freeSlots.empty())
	{
		unsigned int freeSlot = (*freeSlots.begin());
		freeSlots.erase(freeSlots.begin());
	}

	return std::nullopt;
}