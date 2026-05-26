#pragma once
#include "Utils/NonCopyableNonMovable.h"
#include "ReferencesManager.h"
#include "Descriptors/FramebufferDesc.h"
#include "FramebufferHandle.h"
#include "TextureHandle.h"
#include "Framebuffer.h"
#include <vector>
#include <set>
#include <optional>
#include <functional>

namespace dj
{

struct FramebufferHandleSet
{
	FramebufferHandle handle;
	std::vector<TextureHandle> texHandles;
};

class FramebufferManager : public HandleCreator<FramebufferManager>, public ReferencesManager<FramebufferManager>, private NonCopyable, private NonMovable
{
	std::vector<Framebuffer> framebuffers;
	std::set<unsigned int> freeSlots;
public:
	FramebufferManager() noexcept = default;
	~FramebufferManager() = default;

	std::optional<FramebufferHandle> createOnlyFramebuffer(const FramebufferDesc& desc);
	std::optional<FramebufferHandleSet> createFramebufferAndTextures(TextureManager& texMgr, const FramebufferDesc& desc);

	// Operate on Framebuffer
	bool check(const FramebufferHandle& handle, std::function<bool(const Framebuffer&)> fun) const;
	bool execute(const FramebufferHandle& handle, std::function<bool(Framebuffer&)> fun);
	bool bind(const FramebufferHandle& handle) const;

	// Get size
	unsigned int getCount() const;

	/*! \todo Consider other solution to avoid passing OpenGL Framebuffer ID (like RenderBackend class which would be a friend of Framebuffer) */
	std::optional<GLuint> getID(const FramebufferHandle& handle) const;

	// Management methods
	unsigned int getReferencesCount(const FramebufferHandle& handle) const;
	unsigned int forceDeleteUnused();

	// Debugging methods
	bool verifyConsistency() const;
	unsigned int getCellsCount() const;

private:
	unsigned int deleteUnused();

	std::optional<FramebufferHandle> addFramebuffer(Framebuffer&& res, const FramebufferDesc& desc);
	std::optional<unsigned int> popFirstFreeSlot();
};

} // namespace dj