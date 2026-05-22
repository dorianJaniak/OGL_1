#pragma once
#include "ResolutionDesc.h"
#include "TextureSamplingDesc.h"
#include "TextureFormatDesc.h"
#include "Enums/GLTypes.h"
#include "DefinitionsGL.h"

namespace dj
{

/*!
*	\class TextureDesc
*	\todo
*		Change TextureSamplingDesc and TextureFormatDesc to bitfield
*/
struct TextureDesc
{
	ResolutionDesc resolution;
	TextureType glType;			// GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP
	TextureSamplingDesc sampling;
	TextureFormatDesc format;
	bool mipmaps;

	bool operator==(const TextureDesc&) const = default;
};

//! \todo temporary functions: getTextureSamplingDescDefaultsFor2D() and getTextureSamplingDescDefaultsForCube()
inline const TextureSamplingDesc& getTextureSamplingDescDefaultsFor2D()
{
	static TextureSamplingDesc desc = { 
		TextureWrapping::Repeat,
		TextureWrapping::Repeat,
		TextureWrapping::Repeat,
		TextureFilteringMin::LinearMipmapLinear,
		TextureFilteringMag::Linear };

	return desc;
}

inline const TextureSamplingDesc& getTextureSamplingDescDefaultsForCube()
{
	static TextureSamplingDesc desc = { 
		TextureWrapping::ClampToEdge,
		TextureWrapping::ClampToEdge,
		TextureWrapping::ClampToEdge,
		TextureFilteringMin::Linear,
		TextureFilteringMag::Linear };

	return desc;
}

} // namespace dj