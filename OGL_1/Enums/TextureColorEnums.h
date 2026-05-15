#pragma once
#include "Utils.h"
#include <GL/glew.h>
#include <tuple>
#include <optional>

namespace dj
{

#define COLOR_FORMAT_IN_DEVICE_LIST \
	ENTRY_CFID(R,			GL_RED,				8u) \
	ENTRY_CFID(RG,			GL_RG,				16u) \
	ENTRY_CFID(RGB,			GL_RGB,				24u) \
	ENTRY_CFID(SRGB8,		GL_SRGB8,			24u) \
	ENTRY_CFID(RGB8_SNORM,	GL_RGB8_SNORM,		24u) \
	ENTRY_CFID(RGBA,		GL_RGBA,			32u) \
	ENTRY_CFID(SRGB8_ALPHA8,GL_SRGB8_ALPHA8,	32u) \
	ENTRY_CFID(SRGB_ALPHA,	GL_SRGB_ALPHA,		32u)

#define COLOR_FORMAT_IN_SOURCE_LIST \
	ENTRY_CFIS(R,		GL_RED) \
	ENTRY_CFIS(RG,		GL_RG) \
	ENTRY_CFIS(RGB,		GL_RGB) \
	ENTRY_CFIS(BGR,		GL_BGR) \
	ENTRY_CFIS(RGBA,		GL_RGBA) \
	ENTRY_CFIS(BGRA,		GL_BGRA) \
	ENTRY_CFIS(R_Int,	GL_RED_INTEGER) \
	ENTRY_CFIS(RG_Int,	GL_RG_INTEGER)	\
	ENTRY_CFIS(RGB_Int,	GL_RGB_INTEGER) \
	ENTRY_CFIS(BGR_Int,	GL_BGR_INTEGER) \
	ENTRY_CFIS(RGBA_Int,	GL_RGBA_INTEGER) \
	ENTRY_CFIS(BGRA_Int,	GL_BGRA_INTEGER) \
	ENTRY_CFIS(Stencil,	GL_STENCIL_INDEX) \
	ENTRY_CFIS(Depth,	GL_DEPTH_COMPONENT) \
	ENTRY_CFIS(DepthStencil,	GL_DEPTH_STENCIL)

#define PIXEL_DATA_TYPE_IN_SOURCE \
	ENTRY_PDTIS(UnsignedByte,	GL_UNSIGNED_BYTE) \
	ENTRY_PDTIS(Float,		GL_FLOAT)

// Section: Enum classes and their mapping definitions
enum class ColorFormatInDevice
{
#define ENTRY_CFID(name, glenum, bitSize) name,
	COLOR_FORMAT_IN_DEVICE_LIST
#undef ENTRY_CFID
};

constexpr GLenum colorFormatInDeviceMapping[] =
{
#define ENTRY_CFID(name, glenum, bitSize) glenum,
	COLOR_FORMAT_IN_DEVICE_LIST
#undef ENTRY_CFID
};

constexpr unsigned int colorFormatInDeviceBitSizeMapping[] =
{
#define ENTRY_CFID(name, glenum, bitSize) bitSize,
	COLOR_FORMAT_IN_DEVICE_LIST
#undef ENTRY_CFID
};

enum class ColorFormatInSource
{
#define ENTRY_CFIS(name, glenum) name,
	COLOR_FORMAT_IN_SOURCE_LIST
#undef ENTRY_CFIS
};

constexpr GLenum colorFormatInSourceMapping[] =
{
#define ENTRY_CFIS(name, glenum) glenum,
	COLOR_FORMAT_IN_SOURCE_LIST
#undef ENTRY_CFIS
};

enum class PixelDataTypeInSource
{
#define ENTRY_PDTIS(name, glenum) name,
	PIXEL_DATA_TYPE_IN_SOURCE
#undef ENTRY_PDTIS
};

constexpr GLenum pixelDataTypeInSourceMapping[] =
{
#define ENTRY_PDTIS(name, glenum) glenum,
	PIXEL_DATA_TYPE_IN_SOURCE
#undef ENTRY_PDTIS
};

// Section: Compile-time mapping
template <>
constexpr auto& selectEnumMapping<ColorFormatInDevice>()
{
	return colorFormatInDeviceMapping;
}

template <>
constexpr auto& selectEnumMapping<ColorFormatInSource>()
{
	return colorFormatInSourceMapping;
}

template <>
constexpr auto& selectEnumMapping<PixelDataTypeInSource>()
{
	return pixelDataTypeInSourceMapping;
}

template <>
constexpr auto& selectBitSizeMapping<ColorFormatInDevice>()
{
	return colorFormatInDeviceBitSizeMapping;
}

} // namespace dj