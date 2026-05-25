#pragma once
#include "ResourceType.h"
#include "ReferencesManager.h"
#include <cassert>

namespace dj
{

class TextureManager;

template <ResourceType resourceType>
class ReferencesManager;

template <ResourceType resourceType>
class HandleCreator;

template <ResourceType resourceType>
class Handle;
} // namespace dj

namespace dj
{

template <ResourceType resourceType>
class HandleCreator
{
protected:
	Handle<resourceType> createHandle(ReferencesManager<resourceType>* manager, unsigned int index, unsigned int generation)
	{
		return { manager, index, generation };
	}
};


/*! \class Handle
* \todo Join index and generation to uint32_t
* \todo Consider if it should set Index to max (it is not safe to use assert in case of std::move). Should I set manager to nullptr?
*/
template <ResourceType resourceType>
class Handle
{
	friend class HandleCreator<resourceType>;

	ReferencesManager<resourceType>* manager{ nullptr };
	unsigned int index{ 0u };
	unsigned int generation{ 0u };

public:
	Handle() = delete;
	bool operator==(const Handle&) const = default;

	Handle(const Handle& handle) noexcept
		: manager(handle.manager)
		, index(handle.index)
		, generation(handle.generation)
	{
		assert(manager && "Pointer to TextureManager is nullptr");
		manager->addRef(handle);
	}

	Handle(Handle&& handle) noexcept
		: manager(handle.manager)
		, index(handle.index)
		, generation(handle.generation)
	{
		handle.index = 0u;
		handle.generation = 0u;
		handle.manager = nullptr;
	}

	Handle& operator=(const Handle& handle) noexcept
	{
		index = handle.index;
		generation = handle.generation;
		manager = handle.manager;

		assert(manager && "Pointer to TextureManager is nullptr");
		manager->addRef(handle);

		return *this;
	}

	Handle& operator=(Handle&& handle) noexcept
	{
		index = handle.index;
		generation = handle.generation;
		manager = handle.manager;

		handle.index = 0u;
		handle.generation = 0u;
		handle.manager = nullptr;

		return *this;
	}

	~Handle()
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

	unsigned int getIndex() const
	{
		return index;
	}

	unsigned int getGeneration() const
	{
		return generation;
	}

private:
	// accessible only for TextureManager
	Handle(ReferencesManager<resourceType>* manager, unsigned int index, unsigned int generation)
		: manager(manager)
		, index(index)
		, generation(generation)
	{
		assert(manager && "Pointer to TextureManager is nullptr");
		manager->addRef(*this);
	}
};

} // namespace dj