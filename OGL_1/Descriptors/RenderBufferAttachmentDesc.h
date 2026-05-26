#pragma once
#include <GL/glew.h>

namespace dj
{

struct RenderBufferAttachmentDesc
{
	GLenum attachment;
	GLenum internalFormat;
};

} // namespace dj