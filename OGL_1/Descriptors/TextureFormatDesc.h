#pragma once
#include "Enums/TextureColorEnums.h"

namespace dj
{

struct TextureFormatDesc
{
	ColorFormatInDevice inGPUColorFormat;
	ColorFormatInSource sourceColorFormat;
	PixelDataTypeInSource sourceValueType;

	bool operator==(const TextureFormatDesc&) const = default;
};

} // namespace dj