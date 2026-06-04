#pragma once
#include "Enums/FramebufferEnums.h"
#include "Descriptors/TextureAttachmentDesc.h"
#include "Descriptors/RenderBufferAttachmentDesc.h"
#include <variant>

namespace dj
{

struct FramebufferAttachmentDesc
{
	FramebufferAttachment attachment;
	FramebufferAttachmentType type;
	std::variant<TextureAttachmentDesc, RenderBufferAttachmentDesc> desc;
};

} // namespace dj