#include "TextureHandle.h"
#include "TextureReferencesManager.h"
#include <cassert>
using namespace dj;

TextureHandle::TextureHandle(const TextureHandle& handle) noexcept
	: manager(handle.manager)
	, index(handle.index)
	, generation(handle.generation)
{
	assert(manager && "Pointer to TextureManager is nullptr");
	manager->addRef(handle);
}

TextureHandle::TextureHandle(TextureHandle&& handle) noexcept
	: manager(handle.manager)
	, index(handle.index)
	, generation(handle.generation)
{
	handle.index = 0u;
	handle.generation = 0u;
	handle.manager = nullptr;
}

TextureHandle& TextureHandle::operator=(const TextureHandle& handle) noexcept
{
	index = handle.index;
	generation = handle.generation;
	manager = handle.manager;

	assert(manager && "Pointer to TextureManager is nullptr");
	manager->addRef(handle);

	return *this;
}

TextureHandle& TextureHandle::operator=(TextureHandle&& handle) noexcept
{
	index = handle.index;
	generation = handle.generation;
	manager = handle.manager;

	handle.index = 0u;
	handle.generation = 0u;
	handle.manager = nullptr;

	return *this;
}

TextureHandle::~TextureHandle()
{
	//assert(manager && "Pointer to TextureManager is nullptr");
	// It is possible when TextureHandle was moved
	if (manager)
	{
		manager->removeRef(*this);

		manager = nullptr;
		index = 0u;
		generation = 0u;
	}
}

unsigned int TextureHandle::getIndex() const
{
	return index;
}

unsigned int TextureHandle::getGeneration() const
{
	return generation;
}

TextureHandle::TextureHandle(TextureReferencesManager* manager, unsigned int index, unsigned int generation)
	: manager(manager)
	, index(index)
	, generation(generation)
{
	assert(manager && "Pointer to TextureManager is nullptr");
	manager->addRef(*this);
}
