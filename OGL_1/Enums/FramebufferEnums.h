#pragma once
#include "Utils.h"
#include <GL/glew.h>
#include <cstdint>

namespace dj
{

#define FRAMEBUFFER_ATTACHMENTS_COLOR \
	ENTRY_CFID(Color0,		GL_COLOR_ATTACHMENT0) \
	ENTRY_CFID(Color1,		GL_COLOR_ATTACHMENT1) \
	ENTRY_CFID(Color2,		GL_COLOR_ATTACHMENT2) \
	ENTRY_CFID(Color3,		GL_COLOR_ATTACHMENT3) \
	ENTRY_CFID(Color4,		GL_COLOR_ATTACHMENT4) \
	ENTRY_CFID(Color5,		GL_COLOR_ATTACHMENT5) \
	ENTRY_CFID(Color6,		GL_COLOR_ATTACHMENT6) \
	ENTRY_CFID(Color7,		GL_COLOR_ATTACHMENT7) \
	ENTRY_CFID(Color8,		GL_COLOR_ATTACHMENT8) \
	ENTRY_CFID(Color9,		GL_COLOR_ATTACHMENT9) \
	ENTRY_CFID(Color10,		GL_COLOR_ATTACHMENT10) \
	ENTRY_CFID(Color11,		GL_COLOR_ATTACHMENT11) \
	ENTRY_CFID(Color12,		GL_COLOR_ATTACHMENT12) \
	ENTRY_CFID(Color13,		GL_COLOR_ATTACHMENT13) \
	ENTRY_CFID(Color14,		GL_COLOR_ATTACHMENT14) \
	ENTRY_CFID(Color15,		GL_COLOR_ATTACHMENT15)

#define FRAMEBUFFER_ATTACHMENTS_OTHER \
	ENTRY_CFID(Depth,			GL_DEPTH_ATTACHMENT) \
	ENTRY_CFID(Stencil,			GL_STENCIL_ATTACHMENT) \
	ENTRY_CFID(DepthStencil,	GL_DEPTH_STENCIL_ATTACHMENT)

// Section: Enum classes that are not generated
enum class FramebufferAttachmentType : uint8_t
{
	Texture,
	RenderBuffer
};

// Section: Enum classes and their mapping definitions
enum class FramebufferColorAttachment : uint8_t
{
#define ENTRY_CFID(name, glenum) name,
	FRAMEBUFFER_ATTACHMENTS_COLOR
#undef ENTRY_CFID
};

enum class FramebufferAttachment : uint8_t
{
#define ENTRY_CFID(name, glenum) name,
	FRAMEBUFFER_ATTACHMENTS_COLOR
#undef ENTRY_CFID
#define ENTRY_CFID(name, glenum) name,
	FRAMEBUFFER_ATTACHMENTS_OTHER
#undef ENTRY_CFID
};

constexpr GLenum framebufferColorAttachmentsMapping[] =
{
#define ENTRY_CFID(name, glenum) glenum,
	FRAMEBUFFER_ATTACHMENTS_COLOR
#undef ENTRY_CFID
};

constexpr GLenum framebufferAttachmentsMapping[] =
{
#define ENTRY_CFID(name, glenum) glenum,
	FRAMEBUFFER_ATTACHMENTS_COLOR
#undef ENTRY_CFID
#define ENTRY_CFID(name, glenum) glenum,
	FRAMEBUFFER_ATTACHMENTS_OTHER
#undef ENTRY_CFID
};

// Section: Compile-time mapping
template <>
constexpr auto& selectEnumMapping<FramebufferColorAttachment>()
{
	return framebufferColorAttachmentsMapping;
}

template <>
constexpr auto& selectEnumMapping<FramebufferAttachment>()
{
	return framebufferAttachmentsMapping;
}

} // namespace dj