#include "FramebufferManager.h"
#include "Descriptors/TextureDesc.h"
#include "TextureManager.h"
#include <iostream>
using namespace dj;

std::optional<FramebufferHandle> FramebufferManager::createOnlyFramebuffer(const FramebufferDesc& desc)
{
	Framebuffer fbo;
	return addFramebuffer(std::move(fbo), {});
}

std::optional<FramebufferHandleSet> FramebufferManager::createFramebufferAndTextures(TextureManager& texMgr, const FramebufferDesc& desc)
{
	Framebuffer fbo;

	std::vector<TextureHandle> texHandles;
	texHandles.reserve(desc.textureAttachments.size());
		
	for (const TextureAttachmentDesc& attachmentDesc : desc.textureAttachments)
	{
		TextureDesc texDesc{};
		texDesc.glType = attachmentDesc.glType;
		texDesc.resolution = desc.resolution;
		texDesc.sampling = attachmentDesc.sampling;
		texDesc.format = attachmentDesc.format;
		texDesc.mipmaps = false;

		std::optional<TextureHandle> tex = texMgr.createEmptyTexture(texDesc);

		if (!tex)
		{
			std::cerr << "Could not create empty Texture\n";
			return std::nullopt;
		}

		if (!fbo.assignTextureAttachment(texMgr, *tex, attachmentDesc.attachment))
		{
			std::cerr << "Could not assign Texture Attachment\n";
			return std::nullopt;
		}

		texHandles.push_back(*tex);
	}

	for (const RenderBufferAttachmentDesc& attachmentDesc : desc.renderBufferAttachments)
	{
		if (!fbo.genRenderbufferAttachment(attachmentDesc.attachment, attachmentDesc.internalFormat))
		{
			std::cerr << "Could not create Render Buffer Attachment\n";
			return std::nullopt;
		}
	}

	fbo.getFramebufferStatus();
	fbo.unbind();

	std::optional<FramebufferHandle> handle = addFramebuffer(std::move(fbo), desc);

	if (!handle)
	{
		std::cerr << "Could not add created Framebuffer\n";
		return std::nullopt;
	}

	return FramebufferHandleSet { *handle, texHandles };
}

bool FramebufferManager::check(const FramebufferHandle& handle, std::function<bool(const Framebuffer&)> fun) const
{
	if (exists(handle))
	{
		return fun(framebuffers[handle.getIndex()]);
	}

	return false;
}

bool FramebufferManager::execute(const FramebufferHandle& handle, std::function<bool(Framebuffer&)> fun)
{
	if (exists(handle))
	{
		return fun(framebuffers[handle.getIndex()]);
	}

	return false;
}

bool FramebufferManager::bind(const FramebufferHandle& handle) const
{
	if (exists(handle))
	{
		framebuffers[handle.getIndex()].bind();
		return true;
	}

	return false;
}

unsigned int FramebufferManager::getCount() const
{
	assert(freeSlots.size() <= framebuffers.size() && "Incorrect implementation - freeSlots is bigger than framebuffers vector");
	return static_cast<unsigned int>(framebuffers.size() - freeSlots.size());
}

std::optional<GLuint> FramebufferManager::getID(const FramebufferHandle& handle) const
{
	if (exists(handle))
	{
		const Framebuffer& fbo = framebuffers[handle.getIndex()];
		return { fbo.getID() };
	}

	return std::nullopt;
}

unsigned int FramebufferManager::getReferencesCount(const FramebufferHandle& handle) const
{
	if (exists(handle))
	{
		return referencesCount[handle.getIndex()];
	}

	return 0u;
}

unsigned int FramebufferManager::forceDeleteUnused()
{
	return deleteUnused();
}

bool FramebufferManager::verifyConsistency() const
{
	// Verify if vectors have the same size
	bool sameSizeOk = ((framebuffers.size() == referencesCount.size()) &&
		(framebuffers.size() == generations.size()));

	// Verify if freeSlots vector is not bigger than other vectors
	bool freeSlotsSizeOk = freeSlots.size() <= framebuffers.size();

	// Verify if freeSlots points to existing indices
	bool freeSlotsOk = true;
	bool freeSlotsPointOk = true;
	for (unsigned int freeSlot : freeSlots)
	{
		bool indexOk = (freeSlot < framebuffers.size());
		freeSlotsOk &= indexOk;

		// Verify if freeSlots points to cells in which framebuffers truly does not exist
		if (indexOk)
		{
			freeSlotsPointOk &= (referencesCount[freeSlot] == 0u && framebuffers[freeSlot].getID() == 0u);
		}
	}

	// Verify if all framebuffers exists except for freeSlots
	unsigned int framebuffersCount = 0u;
	for (unsigned int i = 0u; i < framebuffers.size(); ++i)
	{
		if (framebuffers[i].getID() != 0u)
		{
			++framebuffersCount;
		}
	}
	bool framebuffersOk = (framebuffersCount + freeSlots.size() == framebuffers.size());

	return (sameSizeOk && freeSlotsSizeOk && freeSlotsOk && freeSlotsPointOk && framebuffersOk);
}

unsigned int FramebufferManager::getCellsCount() const
{
	return static_cast<unsigned int>(framebuffers.size());
}


unsigned int FramebufferManager::deleteUnused()
{
	unsigned int count = 0u;

	for (unsigned int i = 0; i < referencesCount.size(); ++i)
	{
		if (referencesCount[i] == 0u)
		{
			generations[i] += 1u;
			framebuffers[i].clear();
			freeSlots.insert(i);
			++count;
		}
	}

	return count;
}

std::optional<FramebufferHandle> FramebufferManager::addFramebuffer(Framebuffer&& res, const FramebufferDesc& desc)
{
	std::optional<unsigned int> freeSlot = popFirstFreeSlot();
	unsigned int index;

	if (freeSlot)
	{
		index = *freeSlot;
		framebuffers[index] = std::move(res);
		generations[index] += 1u;
	}
	else
	{
		try
		{
			index = static_cast<unsigned int>(framebuffers.size());
			framebuffers.push_back(std::move(res));
			generations.push_back(1u);
			referencesCount.push_back(0u);
		}
		catch (const std::exception&)
		{
			return std::nullopt;
		}
	}

	return createHandle(this, index, generations[index]);
}

std::optional<unsigned int> FramebufferManager::popFirstFreeSlot()
{
	if (!freeSlots.empty())
	{
		unsigned int freeSlot = (*freeSlots.begin());
		freeSlots.erase(freeSlots.begin());

		return freeSlot;
	}

	return std::nullopt;
}