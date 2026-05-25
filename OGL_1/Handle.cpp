#include "Handle.h"
#include "ReferencesManager.h"
#include <cassert>
using namespace dj;

Handle::Handle(const Handle& handle) noexcept
	: manager(handle.manager)
	, index(handle.index)
	, generation(handle.generation)
{
	assert(manager && "Pointer to TextureManager is nullptr");
	manager->addRef(handle);
}

Handle::Handle(Handle&& handle) noexcept
	: manager(handle.manager)
	, index(handle.index)
	, generation(handle.generation)
{
	handle.index = 0u;
	handle.generation = 0u;
	handle.manager = nullptr;
}

Handle& Handle::operator=(const Handle& handle) noexcept
{
	index = handle.index;
	generation = handle.generation;
	manager = handle.manager;

	assert(manager && "Pointer to TextureManager is nullptr");
	manager->addRef(handle);

	return *this;
}

Handle& Handle::operator=(Handle&& handle) noexcept
{
	index = handle.index;
	generation = handle.generation;
	manager = handle.manager;

	handle.index = 0u;
	handle.generation = 0u;
	handle.manager = nullptr;

	return *this;
}

Handle::~Handle()
{
	//assert(manager && "Pointer to TextureManager is nullptr");
	// It is possible when Handle was moved
	if (manager)
	{
		manager->removeRef(*this);

		manager = nullptr;
		index = 0u;
		generation = 0u;
	}
}

unsigned int Handle::getIndex() const
{
	return index;
}

unsigned int Handle::getGeneration() const
{
	return generation;
}

Handle::Handle(ReferencesManager* manager, unsigned int index, unsigned int generation)
	: manager(manager)
	, index(index)
	, generation(generation)
{
	assert(manager && "Pointer to TextureManager is nullptr");
	manager->addRef(*this);
}
