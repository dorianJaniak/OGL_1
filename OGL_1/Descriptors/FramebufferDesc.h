#pragma once
#include "TextureAttachmentDesc.h"
#include "RenderBufferAttachmentDesc.h"
#include "ResolutionDesc.h"
#include <vector>

namespace dj
{

struct FramebufferDesc
{
	ResolutionDesc resolution;
	std::vector<TextureAttachmentDesc> textureAttachments;
	std::vector<RenderBufferAttachmentDesc> renderBufferAttachments;
};

} // namespace dj