#pragma once
#include "ReferencesManager.h"
#include <cassert>

namespace dj
{

template <class ResourceManager>
class ReferencesManager;

template <class ResourceManager>
class HandleCreator;

template <class ResourceManager>
class Handle;

} // namespace dj

namespace dj
{

template <class ResourceManager>
class HandleCreator
{
protected:
	Handle<ResourceManager> createHandle(ReferencesManager<ResourceManager>* manager, unsigned int index, unsigned int generation)
	{
		return { manager, index, generation };
	}
};

/*! \class Handle
* \todo Join index and generation to uint32_t
* \todo Consider if it should set Index to max (it is not safe to use assert in case of std::move). Should I set manager to nullptr?
*/
template <class ResourceManager>
class Handle
{
	friend class HandleCreator<ResourceManager>;

	ReferencesManager<ResourceManager>* manager{ nullptr };
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
		assert(manager && "Pointer to Resource Manager is nullptr");
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
		// Avoid reassignment of the same values
		if (index == handle.index && generation == handle.generation)
		{
			return *this;
		}

		// Remove reference only if this Handle points at something (generation > 0 && manager != nullptr)
		if (generation != 0u && index != handle.index)
		{
			// There should not be a case when manager is nullptr while handle points at something (generation > 0)
			assert(manager && "Pointer to Resource Manager is nullptr");
			manager->removeRef(*this);
		}

		index = handle.index;
		generation = handle.generation;
		manager = handle.manager;

		assert(manager && "Pointer to Resource Manager is nullptr");
		manager->addRef(*this);

		return *this;
	}

	Handle& operator=(Handle&& handle) noexcept
	{
		// Remove reference only if this Handle points at something (generation > 0 && manager != nullptr)
		if (generation != 0u && index != handle.index)
		{
			// There should not be a case when manager is nullptr while handle points at something (generation > 0)
			assert(manager && "Pointer to Resource Manager is nullptr");
			manager->removeRef(*this);
		}

		index = handle.index;
		generation = handle.generation;
		manager = handle.manager;

		// Not-calling addRef(), because fields are nullified in a passed argument - references count does not change 
		handle.index = 0u;
		handle.generation = 0u;
		handle.manager = nullptr;

		return *this;
	}

	~Handle()
	{
		// manager == nullptr, when Handle was moved
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

	bool belongsTo(const ReferencesManager<ResourceManager>* mgr) const
	{
		return (mgr == manager);
	}

private:
	// accessible only for ResourceManager
	Handle(ReferencesManager<ResourceManager>* manager, unsigned int index, unsigned int generation)
		: manager(manager)
		, index(index)
		, generation(generation)
	{
		assert(manager && "Created Handle with undefined manager (nullptr)");
		manager->addRef(*this);
	}
};

} // namespace dj