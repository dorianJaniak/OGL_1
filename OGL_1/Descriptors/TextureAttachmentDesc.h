#pragma once
#include "Enums/GLTypes.h"
#include "Descriptors/TextureFormatDesc.h"
#include "Descriptors/TextureSamplingDesc.h"

namespace dj
{

struct TextureAttachmentDesc
{
	TextureType glType;
	TextureSamplingDesc sampling;
	TextureFormatDesc format;
};

} // namespace dj