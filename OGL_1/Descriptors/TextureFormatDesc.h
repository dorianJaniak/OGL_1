#pragma once
#include <GL/glew.h>

namespace dj
{

struct TextureFormatDesc
{
	GLenum inGPUColorFormat;
	GLenum sourceColorFormat;
	GLenum sourceValueType;
};

} // namespace dj