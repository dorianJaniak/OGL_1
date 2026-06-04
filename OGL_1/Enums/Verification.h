#pragma once
#include <GL/glew.h>
#include <array>

namespace dj
{

// Section: Context
class FramebufferStatusContext;


#define VERIFICATION_FRAMEBUFFER_STATUS_LIST \
	STATUS_GLENUM(GL_FRAMEBUFFER_COMPLETE,						true) \
	STATUS_GLENUM(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,			false) \
	STATUS_GLENUM(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,	false) \
	STATUS_GLENUM(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,		false) \
	STATUS_GLENUM(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,		false) \
	STATUS_GLENUM(GL_FRAMEBUFFER_UNSUPPORTED,					false) \
	STATUS_GLENUM(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,		false) \
	STATUS_GLENUM(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,		false) \
	STATUS_GLENUM(GL_FRAMEBUFFER_UNDEFINED,						false) \
	STATUS_GLENUM(GL_INVALID_ENUM,								false)


// Section: Mapping functions
#define STATUS_GLENUM(glen, status) case glen: return status;

constexpr bool verifyFramebufferStatus(GLenum status)
{
	switch (status) {
		VERIFICATION_FRAMEBUFFER_STATUS_LIST
	};
	return false;
}

#undef STATUS_GLENUM

// Section: Compile-time mapping
template <class StatusContext>
constexpr bool toBool(GLenum status);

template <>
constexpr bool toBool<FramebufferStatusContext>(GLenum status)
{
	return verifyFramebufferStatus(status);
}

} // namespace dj