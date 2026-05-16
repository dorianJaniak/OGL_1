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

}

bool TextureManager::execute(const TextureHandle& handle, std::function<bool(TextureResource&)> fun)
{

}

unsigned int TextureManager::getCount() const
{

}

unsigned int TextureManager::getSizeInVRAM() const
{

}

bool TextureManager::exists(const TextureHandle& handle) const
{

}

bool TextureManager::verifyTextureDescriptor(const TextureDesc& desc) const
{
	if (desc.resolution.width == 0u || desc.resolution.height == 0u)
	{
		return false;
	}

}

bool TextureManager::setBorderColor(const TextureHandle& handle, const ColorRGBA& color)
{

}

bool TextureManager::setWrapping(const TextureHandle& handle, GLenum s, GLenum t, GLenum r)
{

}

bool TextureManager::setFiltering(const TextureHandle& handle, GLenum min, GLenum far)
{

}

std::optional<TextureGLType> TextureManager::getType(const TextureHandle& handle) const
{

}

std::optional<TextureDesc> TextureManager::getDescriptor(const TextureHandle& handle) const
{

}
std::optional<ResolutionDesc> TextureManager::getResolution(const TextureHandle& handle) const
{

}
std::optional<TextureSamplingDesc> TextureManager::getSamplingDesc(const TextureHandle& handle) const
{

}

bool TextureManager::isOk(const TextureHandle& handle) const
{

}

void TextureManager::deleteUnused()
{

}

unsigned int TextureManager::getFirstFreeSlot() const
{
	return (freeSlots.size() > 0u ? freeSlots[0] : textures.size());
}