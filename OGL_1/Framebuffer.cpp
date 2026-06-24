#include "Framebuffer.h"
#include "TextureManager.h"
#include "Enums/Converters.h"
#include "Logging/ILogger.h"
#include "Logging/Log.h"
using namespace dj;

GLuint Framebuffer::globalActiveID = 0u;

Framebuffer::Framebuffer(const FramebufferDesc& desc, std::shared_ptr<ILogger> logger) noexcept
	: id(0u)
	, desc(desc)
	, attachments{}
	, logger(logger)
{
	glGenFramebuffers(1, &id);
}

Framebuffer::Framebuffer(const ResolutionDesc& desc, std::shared_ptr<ILogger> logger) noexcept
	: id(0u)
	, desc{}
	, attachments{}
	, logger(logger)
{
	this->desc.resolution = desc;
	glGenFramebuffers(1, &id);
}

Framebuffer::Framebuffer(Framebuffer&& f) noexcept
	: id(f.id)
	, desc{f.desc}
	, attachments{f.attachments}
	, logger(f.logger)
{
	f.id = 0u;
	f.desc = FramebufferDesc{};
}

Framebuffer& Framebuffer::operator=(Framebuffer&& f) noexcept
{
	id = f.id;
	desc = f.desc;
	attachments = f.attachments;
	logger = f.logger;

	f.id = 0u;
	f.desc = FramebufferDesc{};
	f.attachments = {};

	return *this;
}

Framebuffer::~Framebuffer()
{
	clear();
}

bool Framebuffer::assignTextureAttachment(const TextureManager& texMgr, const TextureHandle& handle, FramebufferAttachment attachment)
{
	unsigned int index = toUnsigned(attachment);

	assert(index < attachments.size() && "Enum converted to index should not be greater than attachments size");
	if (attachments[index].has_value())
	{
		//! \todo Implement variant where texture attachment is reassigned
		return false;
	}

	std::optional<TextureDesc> texDesc = texMgr.getDescriptor(handle);
	if (!texDesc)
	{
		return false;
	}

	//! \todo Consider support for multiresolution attachments
	assert(texDesc->resolution == desc.resolution && "Resolution mismatch between Texture and Framebuffer - it is not supported");

	TextureAttachmentDesc texAttachDesc{};
	texAttachDesc.glType = texDesc->glType;
	texAttachDesc.sampling = texDesc->sampling;
	texAttachDesc.format = texDesc->format;

	FramebufferAttachmentDesc attachmentDesc{};
	attachmentDesc.attachment = attachment;
	attachmentDesc.type = FramebufferAttachmentType::Texture;
	attachmentDesc.desc = texAttachDesc;

	AttachmentBinding binding{};
	binding.attachment = attachment;
	binding.type = FramebufferAttachmentType::Texture;
	binding.data = AttachmentTextureBinding{ handle };

	std::optional<GLuint> texID = texMgr.getID(handle);
	if (!texID)
	{
		return false;
	}

	bind();
	texMgr.bind(handle);
	glFramebufferTexture(GL_FRAMEBUFFER, toGLenum(attachment), *texID, 0);

	desc.attachments[index] = attachmentDesc;
	attachments[index] = binding;

	return true;
}

bool Framebuffer::assignRenderbufferAttachment(FramebufferAttachment attachment, ColorFormatInDevice internalFormat)
{
	unsigned int index = toUnsigned(attachment);

	assert(index < attachments.size() && "Enum converted to index should not be greater than attachments size");
	if (attachments[index].has_value())
	{
		//! \todo Implement variant where render buffer is reassigned
		return false;
	}

	bind();
	GLuint rboID = 0u;
	glGenRenderbuffers(1, &rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, toGLenum(internalFormat), desc.resolution.width, desc.resolution.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, toGLenum(attachment), GL_RENDERBUFFER, rboID);

	RenderBufferAttachmentDesc rboDesc{};
	rboDesc.internalFormat = internalFormat;

	FramebufferAttachmentDesc attachmentDesc{};
	attachmentDesc.attachment = attachment;
	attachmentDesc.type = FramebufferAttachmentType::RenderBuffer;
	attachmentDesc.desc = rboDesc;
	desc.attachments[index] = attachmentDesc;

	AttachmentBinding binding{};
	binding.attachment = attachment;
	binding.type = FramebufferAttachmentType::RenderBuffer;
	binding.data = RenderBufferBinding{ rboID };
	attachments[index] = binding;

	return true;
}

void Framebuffer::nullifyData()
{
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void Framebuffer::bind() const
{
	if (id != globalActiveID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		globalActiveID = id;
	}
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0u);
	globalActiveID = 0u;
}

GLenum Framebuffer::getFramebufferStatus()
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

std::optional<TextureHandle> Framebuffer::getTextureHandle(FramebufferAttachment attachment)
{
	unsigned int index = toUnsigned(attachment);
	assert(index < attachments.size() && "Enum converted to index should not be greater than attachments size");

	if (!attachments[index].has_value())
	{
		return std::nullopt;
	}

	if (attachments[index]->type != FramebufferAttachmentType::Texture)
	{
		return std::nullopt;
	}

	return std::get<AttachmentTextureBinding>(attachments[index]->data).texHandle;
}

unsigned int Framebuffer::getWidth() const
{
	return desc.resolution.width;
}

unsigned int Framebuffer::getHeight() const
{
	return desc.resolution.height;
}

void Framebuffer::clear()
{
	glDeleteFramebuffers(1, &id);

	for (std::optional<AttachmentBinding>& att : attachments)
	{
		if (att)
		{
			if (att->type == FramebufferAttachmentType::RenderBuffer)
			{
				glDeleteRenderbuffers(1, &std::get<RenderBufferBinding>(att->data).rboID);
			}
		}

		att = std::nullopt;
	}

	id = 0u;
}

GLuint Framebuffer::getID() const
{
	return id;
}

bool Framebuffer::verifyResolutionConsistency(const TextureManager& texMgr, const TextureHandle& handle, GLenum attachment) const
{
	std::optional<ResolutionDesc> res = texMgr.getResolution(handle);
	assert(res && "In this case ResolutionDesc always should be returned");

	bool resolutionConsistency = (res->width == desc.resolution.width
		&& res->height != desc.resolution.height);

	if (!resolutionConsistency && logger)
	{
		logger->log(Log(LogLevel::Warning, 0u, "Framebuffer", "Resolution of attachment: {} has different resolution than Framebuffer", attachment));
	}

	return resolutionConsistency;
}