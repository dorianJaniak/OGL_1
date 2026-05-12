#pragma once
#include "ResolutionDesc.h"
#include "TextureSamplingDesc.h"
#include "TextureFormatDesc.h"
#include "../DefinitionsGL.h"

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
	TextureGLType glType;			// GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP
	TextureSamplingDesc sampling;
	TextureFormatDesc format;
	bool mipmaps;
};

//! \todo temporary functions: getTextureSamplingDescDefaultsFor2D() and getTextureSamplingDescDefaultsForCube()
const TextureSamplingDesc& getTextureSamplingDescDefaultsFor2D()
{
	static TextureSamplingDesc desc = { 
		GL_REPEAT,
		GL_REPEAT,
		GL_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR };

	return desc;
}

const TextureSamplingDesc& getTextureSamplingDescDefaultsForCube()
{
	static TextureSamplingDesc desc = { 
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_EDGE,
		GL_LINEAR,
		GL_LINEAR };

	return desc;
}

} // namespace dj