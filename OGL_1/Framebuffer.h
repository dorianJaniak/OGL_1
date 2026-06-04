#pragma once
#include "Utils/NonCopyableNonMovable.h"
#include "Definitions.h"
#include "TextureHandle.h"
#include "Descriptors/FramebufferDesc.h"
#include <GL/glew.h>
#include <vector>
#include <optional>

namespace dj
{
class TextureManager;
class FramebufferManager;
} // namespace dj

namespace dj
{

/*! \class Framebuffer
	\brief OpenGL Framebuffer and Render Buffer Objects class

	By creating object of this class, new Framebuffer ID (OpenGL handler) is being created.
	It is related to this OpenGL ID for its whole lifetime.
	It does not reserve memory for Texture Attachments, Textures needs to be created earlier
	and need to exist for the whole lifetime of this object.
	Framebuffer class supports multiple Texture attachments, but only one Render Buffer Object.
	It is important to first add Texture attachments, and then Render Buffer Object, because
	the latter one depends on highest resolution specified in Texture attachments.

	\todo 
	- check if for RBO it should take resolution from Texture Attachments 
	- check if Texture Resolutions works correctly
*/
class Framebuffer : private NonCopyable
{
	friend FramebufferManager;

	struct AttachmentTextureBinding
	{
		TextureHandle texHandle;
	};

	struct RenderBufferBinding
	{
		GLuint rboID;
	};

	struct AttachmentBinding
	{
		FramebufferAttachment attachment;
		FramebufferAttachmentType type;
		std::variant<RenderBufferBinding, AttachmentTextureBinding> data;
	};

	static GLuint globalActiveID;
	GLuint id;
	std::array<std::optional<AttachmentBinding>, std::size(framebufferAttachmentsMapping)> attachments;
	FramebufferDesc desc;

public:
	Framebuffer(const FramebufferDesc& desc) noexcept;
	Framebuffer(const ResolutionDesc& desc) noexcept;
	Framebuffer(Framebuffer&&) noexcept;
	Framebuffer& operator=(Framebuffer&&) noexcept;
	~Framebuffer();

	bool assignTextureAttachment(const TextureManager& texMgr, const TextureHandle& handle, FramebufferAttachment attachment);
	bool assignRenderbufferAttachment(FramebufferAttachment attachment, ColorFormatInDevice internalFormat);

	void nullifyData();
	void bind() const;
	static void unbind();

	/*! \brief Checks Framebuffer status.
	
		It needs to be executed directly after adding attachments to this, specific Framebuffer
		instance. Otherwise, it may receive status of another Framebuffer. 
	*/
	static GLenum getFramebufferStatus();
	std::optional<TextureHandle> getTextureHandle(FramebufferAttachment attachment);
	unsigned int getWidth() const;
	unsigned int getHeight() const;

private:
	// Methods for FramebufferManager
	void clear();								// deletes ID

	/*! \todo
	- Consider other solution to avoid passing OpenGL Framebuffer ID (like RenderBackend class which is declared here as a friend)
	*/
	GLuint getID() const;

	bool verifyResolutionConsistency(const TextureManager& texMgr, const TextureHandle& handle, GLenum attachment) const;
};

} // namespace dj