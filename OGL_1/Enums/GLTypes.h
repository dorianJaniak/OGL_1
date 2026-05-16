#pragma once
#include "Enums/Utils.h"
#include <GL/glew.h>

namespace dj
{

#define TEXTURE_TYPE_LIST \
	ENTRY_ENUM(Texture2D,	GL_TEXTURE_2D) \
	ENTRY_ENUM(TextureCube,	GL_TEXTURE_CUBE_MAP)

#define TEXTURE_CUBE_SIDE_LIST \
	ENTRY_ENUM(PositiveX,	GL_TEXTURE_CUBE_MAP_POSITIVE_X) \
	ENTRY_ENUM(NegativeX,	GL_TEXTURE_CUBE_MAP_NEGATIVE_X) \
	ENTRY_ENUM(PositiveY,	GL_TEXTURE_CUBE_MAP_POSITIVE_Y) \
	ENTRY_ENUM(NegativeY,	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y) \
	ENTRY_ENUM(PositiveZ,	GL_TEXTURE_CUBE_MAP_POSITIVE_Z) \
	ENTRY_ENUM(NegativeZ,	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)

// Section: Enum classes and their mapping definitions
enum class TextureType
{
#define ENTRY_ENUM(name, glenum) name,
	TEXTURE_TYPE_LIST
#undef ENTRY_ENUM
};

enum class TextureCubeSide
{
#define ENTRY_ENUM(name, glenum) name,
	TEXTURE_CUBE_SIDE_LIST
#undef ENTRY_ENUM
};

constexpr GLenum textureTypeMapping[] =
{
#define ENTRY_ENUM(name, glenum) glenum,
	TEXTURE_TYPE_LIST
#undef ENTRY_ENUM
};

constexpr GLenum textureCubeSideMapping[] =
{
#define ENTRY_ENUM(name, glenum) glenum,
	TEXTURE_CUBE_SIDE_LIST
#undef ENTRY_ENUM
};

// Section: Compile-time mapping
template <>
constexpr auto& selectEnumMapping<TextureType>()
{
	return textureTypeMapping;
}

template <>
constexpr auto& selectEnumMapping<TextureCubeSide>()
{
	return textureCubeSideMapping;
}

} // namespace dj