#pragma once
#include "Utils.h"
#include <GL/glew.h>
#include <cstdint>

namespace dj
{

#define TEXTURE_PARAMETER_WRAPPING_LIST \
	ENTRY_ENUM(Repeat,				GL_REPEAT) \
	ENTRY_ENUM(ClampToEdge,			GL_CLAMP_TO_EDGE) \
	ENTRY_ENUM(ClampToBorder,		GL_CLAMP_TO_BORDER) \
	ENTRY_ENUM(MirroredRepeat,		GL_MIRRORED_REPEAT) \
	ENTRY_ENUM(MirroredClampToEdge, GL_MIRROR_CLAMP_TO_EDGE)

#define TEXTURE_PARAMETER_MAG_FILTER_LIST \
	ENTRY_ENUM(Linear,	GL_LINEAR) \
	ENTRY_ENUM(Nearest,	GL_NEAREST)

#define TEXTURE_PARAMETER_MIN_FILTER_LIST \
	ENTRY_ENUM(Linear,					GL_LINEAR) \
	ENTRY_ENUM(Nearest,					GL_NEAREST) \
	ENTRY_ENUM(NearestMipmapNearest,	GL_NEAREST_MIPMAP_NEAREST) \
	ENTRY_ENUM(LinearMipmapNearest,		GL_LINEAR_MIPMAP_NEAREST) \
	ENTRY_ENUM(NearestMipmapLinear,		GL_NEAREST_MIPMAP_LINEAR) \
	ENTRY_ENUM(LinearMipmapLinear,		GL_LINEAR_MIPMAP_LINEAR)

// Section: Enum classes and their mapping definitions
enum class TextureWrapping : uint8_t
{
#define ENTRY_ENUM(name, glenum) name,
	TEXTURE_PARAMETER_WRAPPING_LIST
#undef ENTRY_ENUM
};

enum class TextureFilteringMin : uint8_t
{
#define ENTRY_ENUM(name, glenum) name,
	TEXTURE_PARAMETER_MIN_FILTER_LIST
#undef ENTRY_ENUM
};

enum class TextureFilteringMag : uint8_t
{
#define ENTRY_ENUM(name, glenum) name,
	TEXTURE_PARAMETER_MIN_FILTER_LIST
#undef ENTRY_ENUM
};

constexpr GLenum textureParameterWrappingMapping[] =
{
#define ENTRY_ENUM(name, glenum) glenum,
	TEXTURE_PARAMETER_WRAPPING_LIST
#undef ENTRY_ENUM
};

constexpr GLenum textureParameterFilteringMinMapping[] =
{
#define ENTRY_ENUM(name, glenum) glenum,
	TEXTURE_PARAMETER_MIN_FILTER_LIST
#undef ENTRY_ENUM
};

constexpr GLenum textureParameterFilteringMagMapping[] =
{
#define ENTRY_ENUM(name, glenum) glenum,
	TEXTURE_PARAMETER_MAG_FILTER_LIST
#undef ENTRY_ENUM
};

// Section: Compile-time mapping
template <>
constexpr auto& selectEnumMapping<TextureWrapping>()
{
	return textureParameterWrappingMapping;
}

template <>
constexpr auto& selectEnumMapping<TextureFilteringMin>()
{
	return textureParameterFilteringMinMapping;
}

template <>
constexpr auto& selectEnumMapping<TextureFilteringMag>()
{
	return textureParameterFilteringMagMapping;
}

} // namespace dj