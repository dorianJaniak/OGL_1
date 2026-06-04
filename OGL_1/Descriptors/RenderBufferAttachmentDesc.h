#pragma once
#include "Enums/TextureColorEnums.h"
#include <GL/glew.h>

namespace dj
{

struct RenderBufferAttachmentDesc
{
	ColorFormatInDevice internalFormat;
};

} // namespace dj