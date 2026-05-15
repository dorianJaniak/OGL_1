#pragma once
#include <GL/glew.h>
#include "Enums/TextureColorEnums.h"

namespace dj
{

struct TextureFormatDesc
{
	ColorFormatInDevice inGPUColorFormat;
	ColorFormatInSource sourceColorFormat;
	PixelDataTypeInSource sourceValueType;
};

} // namespace dj