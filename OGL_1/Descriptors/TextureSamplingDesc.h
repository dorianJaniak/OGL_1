#pragma once
#include "Enums/TextureParameterEnums.h"

namespace dj
{

struct TextureSamplingDesc
{
	TextureWrapping wrapS;
	TextureWrapping wrapT;
	TextureWrapping wrapR;
	TextureFilteringMin minFilter;
	TextureFilteringMag magFilter;

	void setWrapping(TextureWrapping s, TextureWrapping t, TextureWrapping r = TextureWrapping::Repeat)
	{
		wrapS = s;
		wrapT = t;
		wrapR = r;
	}

	void setFiltering(TextureFilteringMin min, TextureFilteringMag mag)
	{
		minFilter = min;
		magFilter = mag;
	}

	bool operator==(const TextureSamplingDesc&) const = default;
};

} // namespace dj