#include "Framebuffer.h"
#include "Texture.h"
using namespace dj;
#include <iostream>

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

void Framebuffer::assignTextureAttachment(const dj::ConstTexturePtr& tex, GLenum attachment)
{
	bind();
	TextureTypeInfo tti = tex->getTextureTypeInfo();
	if (tti.glType == GL_TEXTURE_CUBE_MAP)
	{
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, tti.id, 0);
	}
	else
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tti.glType, tti.id, 0);
	}
	if ((width == 0u || height == 0u) || width > tex->getWidth() || height > tex->getHeight())
	{
		width = tex->getWidth();
		height = tex->getHeight();
	}

	textures.push_back({ attachment, tex });
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

dj::ConstTexturePtr Framebuffer::getTextureAttachment(GLenum attachment)
{
	for (const AttachmentTextureBinding& attachmentBinding : textures)
	{
		if (attachmentBinding.attachment == attachment)
		{
			return attachmentBinding.texture;
		}
	}

	return nullptr;
}

unsigned int Framebuffer::getWidth() const
{
	return width;
}

unsigned int Framebuffer::getHeight() const
{
	return height;
}