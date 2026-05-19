#include "TextureManager.h"
#include "TextureData.h"
#include "Enums/Converters.h"
using namespace dj;

std::optional<TextureHandle> TextureManager::createEmptyTexture(const TextureDesc& desc)
{
	if (desc.resolution.width == 0u || desc.resolution.height == 0u)
	{
		return std::nullopt;
	}

	TextureResource res;
	res.recreate(desc);
	res.bind();
	res.nullify();

	return addTexture(std::move(res), desc, "");
}

std::optional<TextureHandle> TextureManager::create2DFromFile(const TextureSamplingDesc& sampling, const char* path, bool generateMipMaps, bool flip)
{
	TextureData data(path, flip);

	if (!data.isOk())
	{
		// std::cerr << dj::Log::failPrefix() << "Could not load texture: " << path << std::endl;;
		return std::nullopt;
	}

	if (data.getWidth() == 0u || data.getHeight() == 0u)
	{
		// std::cerr << dj::Log::failPrefix() << "Wrong resolution of loaded texture: " << path << std::endl;
		return std::nullopt;
	}

	std::optional<TextureFormatDesc> sourceColorFormat = channelsCountToColorFormat(data.getChannelsCount());
	if (!sourceColorFormat)
	{
		// std::cerr << dj::Log::failPrefix() << "Unknown texture color format: " << path << std::endl;
		return std::nullopt;
	}

	TextureDesc desc;
	desc.glType = TextureType::Texture2D;
	desc.resolution.width = data.getWidth();
	desc.resolution.height = data.getHeight();
	desc.format = *sourceColorFormat;
	desc.sampling = sampling;
	desc.mipmaps = generateMipMaps;

	TextureResource res;
	res.bind();
	res.recreate(desc);
	res.transferData2D(desc.format.sourceColorFormat, desc.format.sourceValueType, data.getData());

	if (desc.mipmaps)
	{
		glGenerateMipmap(toGLenum(desc.glType));
	}

	return addTexture(std::move(res), desc, path);
}

std::optional<TextureHandle> TextureManager::createCubeMapFromFile(const TextureSamplingDesc& sampling, const char* pathPrefix, const char* pathSuffixes[6], bool generateMipMaps, bool flip)
{
	constexpr unsigned int sidesCount = 6u;

	auto genSidePath = [pathPrefix, pathSuffixes](unsigned int side) {
		return (std::string(pathPrefix) + std::string(pathSuffixes[side]));
	};

	// Load first data to get info
	TextureData data(genSidePath(0u).c_str(), flip);

	if (!data.isOk())
	{
		// std::cerr << dj::Log::failPrefix() << "Could not load texture: " << path << std::endl;;
		return std::nullopt;
	}

	if (data.getWidth() == 0u || data.getHeight() == 0u)
	{
		// std::cerr << dj::Log::failPrefix() << "Wrong resolution of loaded texture: " << path << std::endl;
		return std::nullopt;
	}

	if (data.getWidth() != data.getHeight())
	{
		// std::cerr << dj::Log::failPrefix() << "Resolution of cube map side is incorrect. Width should equal height. Textures: " << pathPrefix << std::endl;
		return std::nullopt;
	}

	std::optional<TextureFormatDesc> sourceColorFormat = channelsCountToColorFormat(data.getChannelsCount());
	if (!sourceColorFormat)
	{
		// std::cerr << dj::Log::failPrefix() << "Unknown texture color format: " << path << std::endl;
		return std::nullopt;
	}

	TextureDesc desc;
	desc.glType = TextureType::TextureCube;
	desc.resolution.width = data.getWidth();
	desc.resolution.height = data.getHeight();
	desc.format = *sourceColorFormat;
	desc.sampling = sampling;
	desc.mipmaps = generateMipMaps;

	TextureResource res;
	res.bind();
	res.recreate(desc);

	std::optional<TextureCubeSide> sideEnum = toEnum<TextureCubeSide>(0u);
	assert(sideEnum && "Could not map index to TextureCubeSide");

	res.transferCubeSide(*sideEnum, desc.format.sourceColorFormat, desc.format.sourceValueType, data.getData());

	// Load another sides
	for (unsigned int i = 1u; i < sidesCount; ++i)
	{
		TextureData data(genSidePath(i).c_str(), flip);

		if (!data.isOk())
		{
			// std::cerr << dj::Log::failPrefix() << "Could not load texture: " << path << std::endl;;
			return std::nullopt;
		}

		if (data.getWidth() != desc.resolution.width || data.getHeight() != desc.resolution.height)
		{
			// std::cerr << dj::Log::failPrefix() << "Side: " << i << " has different resolution than side 0. Could not load texture: " << pathPrefix << std::endl;;
			return std::nullopt;
		}

		std::optional<TextureFormatDesc> sourceColorFormat = channelsCountToColorFormat(data.getChannelsCount());
		if (!sourceColorFormat)
		{
			// std::cerr << dj::Log::failPrefix() << "Unknown texture color format: " << pathPrefix << std::endl;
			return std::nullopt;
		}

		if (*sourceColorFormat != desc.format)
		{
			// std::cerr << dj::Log::failPrefix() << "Side: " << i << " has different color format than side 0. Could not load texture: " << pathPrefix << std::endl;
			return std::nullopt;
		}

		std::optional<TextureCubeSide> sideEnum = toEnum<TextureCubeSide>(i);
		assert(sideEnum && "Could not map index to TextureCubeSide");

		res.transferCubeSide(*sideEnum, desc.format.sourceColorFormat, desc.format.sourceValueType, data.getData());
	}

	return addTexture(std::move(res), desc, pathPrefix);
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
	return static_cast<unsigned int>(textures.size());
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

std::optional<TextureHandle> TextureManager::addTexture(TextureResource&& res, const TextureDesc& desc, const char* path)
{
	std::optional<unsigned int> freeSlot = popFirstFreeSlot();
	unsigned int index;

	if (freeSlot)
	{
		index = *freeSlot;
		textures[index] = std::move(res);
		paths[index] = path;
		generations[index] += 1u;
		// referencesCount[*freeSlot] = 0u;			// it should always equal to 0
	}
	else
	{
		try
		{
			index = static_cast<unsigned int>(textures.size());
			textures.push_back(std::move(res));
			paths.push_back(path);
			generations.push_back(1u);
			referencesCount.push_back(0u);
		}
		catch (const std::exception&)
		{
			// std::cerr << dj::Log::failPrefix() << "Could not add new texture to vectors - exception: " << e.what() << std::endl;
			return std::nullopt;
		}
	}

	return TextureHandle(this, index, generations[index]);
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

std::optional<TextureFormatDesc> TextureManager::channelsCountToColorFormat(unsigned int count) const
{
	switch (count)
	{
	case 1u: return TextureFormatDesc{ ColorFormatInDevice::R, ColorFormatInSource::R, PixelDataTypeInSource::UnsignedByte };
	case 2u: return TextureFormatDesc{ ColorFormatInDevice::RG, ColorFormatInSource::RG, PixelDataTypeInSource::UnsignedByte };
	case 3u: return TextureFormatDesc{ ColorFormatInDevice::RGB, ColorFormatInSource::RGB, PixelDataTypeInSource::UnsignedByte };
	case 4u: return TextureFormatDesc{ ColorFormatInDevice::RGBA, ColorFormatInSource::RGBA, PixelDataTypeInSource::UnsignedByte };
	}

	return std::nullopt;
}