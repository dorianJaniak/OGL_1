#pragma once
#include "Enums/GLTypes.h"
#include "Descriptors/TextureFormatDesc.h"
#include "Descriptors/TextureSamplingDesc.h"
#include <GL/glew.h>

namespace dj
{

struct TextureAttachmentDesc
{
	GLenum attachment;
	TextureType glType;
	TextureSamplingDesc sampling;
	TextureFormatDesc format;
};

} // namespace dj