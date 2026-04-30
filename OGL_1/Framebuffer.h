#pragma once
#include "Definitions.h"
#include <GL/glew.h>
#include <vector>

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
class Framebuffer
{
	struct AttachmentTextureBinding
	{
		GLenum attachment;
		dj::ConstTexturePtr texture;
	};

	GLuint id;
	GLuint rboID;
	unsigned int width;
	unsigned int height;
	std::vector<AttachmentTextureBinding> textures;
	// std::vector<const dj::ConstRenderBuffersWeakPtr> &renderbuffers;
public:
	Framebuffer() noexcept;
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) noexcept;
	~Framebuffer();

	/*! \brief Adds new texture attachment to Framebuffer.
	
		\param[in] tex Texture attachment (supports: GL_TEXTURE_CUBE_MAP and GL_TEXTURE_2D)
		\param[in] attachment Attachment point in FBO (for example: GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT)
	*/
	void assignTextureAttachment(const dj::ConstTexturePtr& tex, GLenum attachment);
	//bool addRenderBufferAttachment(GLenum attachment);

	/*! \brief Adds new Render Buffer Attachment to Framebuffer.
	
		Currently it supports only single RBO assigned to Framebuffer.
		It won't add when trying to add another RBO object. 
		Needs to be called after \ref assignTextureAttachment method, because
		it requires resolution settings.

		\param[in] attachment Attachment point in FBO (for example: GL_DEPTH_STENCIL_ATTACHMENT)
		\param[in] internalFormat Attachment format for RBO (for example: GL_DEPTH24_STENCIL)
	*/
	bool genRenderbufferAttachment(GLenum attachment, GLenum internalFormat);

	void nullifyData();
	void bind();
	void unbind();

	/*! \brief Checks Framebuffer status.
	
		It needs to be executed directly after adding attachments to this, specific Framebuffer
		instance. Otherwise, it may receive status of another Framebuffer. 
	*/
	static GLenum getFramebufferStatus();
	dj::ConstTexturePtr getTextureAttachment(GLenum attachment);
	unsigned int getWidth() const;
	unsigned int getHeight() const;
};

} // namespace dj