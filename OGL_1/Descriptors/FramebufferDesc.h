#pragma once
#include "ResolutionDesc.h"
#include "FramebufferAttachmentDesc.h"
#include <array>
#include <optional>

namespace dj
{

struct FramebufferDesc
{
	ResolutionDesc resolution;
	std::array<std::optional<FramebufferAttachmentDesc>, std::size(framebufferAttachmentsMapping)> attachments;
};

} // namespace dj