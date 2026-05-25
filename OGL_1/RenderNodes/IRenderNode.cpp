#include "IRenderNode.h"
#include "../Program.h"
#include "TextureManager.h"
using namespace dj;

void IRenderNode::beginBindingTextures()
{
	textureUnitNo = 0u;
}

void IRenderNode::saveTextureUnitNo(unsigned int partIndex)
{
	partIndex = (partIndex < c_maxTextureUnitOffsets ? partIndex : c_maxTextureUnitOffsets - 1);
	textureUnitNoOffsets[partIndex] = textureUnitNo;
}

void IRenderNode::restoreTextureUnitNo(unsigned int partIndex)
{
	partIndex = (partIndex < c_maxTextureUnitOffsets ? partIndex : c_maxTextureUnitOffsets - 1);
	textureUnitNo = textureUnitNoOffsets[partIndex];
}

bool IRenderNode::bindAndUniformTexture(const Handle& texHandle, GLint uniformLocation)
{
	if (textureUnitNo >= c_maxTextureUnitsCount)
	{
		return false;
	}

	glActiveTexture(GL_TEXTURE0 + textureUnitNo);
	texMgr.bind(texHandle);
	glUniform1i(uniformLocation, textureUnitNo);
	++textureUnitNo;

	return true;
}

bool IRenderNode::bindTexture(const Handle& texHandle)
{
	if (textureUnitNo >= c_maxTextureUnitsCount)
	{
		return false;
	}

	glActiveTexture(GL_TEXTURE0 + textureUnitNo);
	texMgr.bind(texHandle);

	++textureUnitNo;

	return true;
}

bool IRenderNode::bindAndUniformNodeTextures(ConstProgramPtr program)
{
	bool ok = true;

	for (const auto tb : nodeInputTextures)
	{
		if (GLint uniformLocation = program->getUniformLocation(tb.first); uniformLocation != -1 && ok)
		{
			ok = bindAndUniformTexture(tb.second, uniformLocation);
		}
		else
		{
			ok = false;
		}
	}

	return ok;
}

unsigned int IRenderNode::getTextureUnitNo() const
{
	return textureUnitNo;
}

unsigned int IRenderNode::getFreeTextureUnitsCount() const
{
	return (textureUnitNo > c_maxTextureUnitsCount ? 0u : c_maxTextureUnitsCount - textureUnitNo);
}

IRenderNode::IRenderNode(const TextureManager& texMgr, FramebufferPtr output, const std::string& name)
	: texMgr(texMgr)
	, output(output)
	, clearFlags(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
	, depthTestEnabled(true)
	, depthUpdateEnabled(true)
	, culling(GL_BACK)
	, textureUnitNo(0u)
	, textureUnitNoOffsets{ 0u }
{
}

void IRenderNode::run()
{
	preFrame();
	draw();
}

void IRenderNode::setClearFlags(GLuint flags)
{
	clearFlags = flags;
}

void IRenderNode::enableDepthTest(bool enable)
{
	depthTestEnabled = enable;
}

void IRenderNode::enableDepthUpdate(bool enable)
{
	depthUpdateEnabled = enable;
}

void IRenderNode::setFaceCulling(GLuint culling)
{
	this->culling = culling;
}

void IRenderNode::setConfiguration(GLuint clearFlags, bool depthTestEnabled, bool depthUpdateEnabled, GLuint faceCulling)
{
	setClearFlags(clearFlags);
	enableDepthTest(depthTestEnabled);
	enableDepthUpdate(depthUpdateEnabled);
	setFaceCulling(faceCulling);
}

void IRenderNode::addTexture(const std::string& samplerName, const Handle& texHandle)
{
	nodeInputTextures.insert({ samplerName, texHandle });
}

void IRenderNode::preFrame()
{
	// Needs to unbind all texture types, in order to clear Texture state between nodes
	// Example: Suppose previous Node bound Texture A to Texture Unit #3 (it was last bound texture)
	//	While in this Node Texture A should be bound to Texture Unit #0 (it is first texture to be bound)
	//	TextureResource mechanism that protects from rebinding the same texture won't call glBindTexture in this case.
	TextureResource::unbindAllTypes();
	output->bind();
	glViewport(0u, 0u, output->getWidth(), output->getHeight());

	if (clearFlags != GL_NONE)
	{
		// All masks needs to be set in order to clear buffers (to allow its modifications)
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilMask(GL_TRUE);
		glClear(clearFlags);
	}
	
	// glDepthMask needs to be below glClear, because it can disable clearing depth buffer
	glDepthMask(depthUpdateEnabled ? GL_TRUE : GL_FALSE);

	if (culling == GL_FRONT || culling == GL_BACK || culling == GL_FRONT_AND_BACK)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(culling);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	if (depthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}