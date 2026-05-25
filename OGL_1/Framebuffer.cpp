#include "Framebuffer.h"
#include "TextureManager.h"
#include <iostream>
using namespace dj;

Framebuffer::Framebuffer() noexcept
	: rboID(0u)
	, width(0u)
	, height(0u)
{
	glGenFramebuffers(1, &id);
}

Framebuffer::Framebuffer(Framebuffer&&) noexcept
	: id(id)
	, rboID(rboID)
	, width(width)
	, height(height)
{
}

Framebuffer::~Framebuffer()
{
	textures.clear();
	glDeleteFramebuffers(1, &id);
	glDeleteRenderbuffers(1, &rboID);
	std::cout << "Framebuffer destructed\n";
}

bool Framebuffer::assignTextureAttachment(const TextureManager& texMgr, const Handle& handle, GLenum attachment)
{
	const std::optional<GLuint> texID = texMgr.getID(handle);
	if (!texID)
	{
		return false;
	}

	std::optional<ResolutionDesc> res = texMgr.getResolution(handle);
	assert(res && "In this case ResolutionDesc always should be returned");

	if (width == 0u || height == 0u || width > res->width || height > res->height)
	{
		width = res->width;
		height = res->height;
	}

	bind();
	glFramebufferTexture(GL_FRAMEBUFFER, attachment, *texID, 0);

	textures.push_back({ attachment, handle });

	return true;
}

bool Framebuffer::genRenderbufferAttachment(GLenum attachment, GLenum internalFormat)
{
	if (rboID != 0u || width == 0u || height == 0u)
	{
		return false;
	}
	bind();

	glGenRenderbuffers(1, &rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rboID);

	return true;
}

void Framebuffer::nullifyData()
{
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}

GLenum Framebuffer::getFramebufferStatus()
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

std::optional<Handle> Framebuffer::getTextureAttachment(GLenum attachment)
{
	for (const AttachmentTextureBinding& attachmentBinding : textures)
	{
		if (attachmentBinding.attachment == attachment)
		{
			return attachmentBinding.texHandle;
		}
	}

	return std::nullopt;
}

unsigned int Framebuffer::getWidth() const
{
	return width;
}

unsigned int Framebuffer::getHeight() const
{
	return height;
}