#pragma once
#include "ResourceType.h"
#include <vector>

namespace dj
{
template <ResourceType resourceType>
class Handle;
}

namespace dj
{

/*! \class ReferencesManager
* \todo consider removing "friend Handle"
*/
template <ResourceType resourceType>
class ReferencesManager
{
	friend class Handle<resourceType>;

protected:
	std::vector<unsigned int> referencesCount;
	std::vector<unsigned int> generations;

	void addRef(const Handle<resourceType>& h)
	{
		if (h.getIndex() < referencesCount.size()
			&& h.getGeneration() == generations[h.getIndex()])
		{
			referencesCount[h.getIndex()] += 1u;
		}
	}

	void removeRef(const Handle<resourceType>& h)
	{
		if (h.getIndex() < referencesCount.size()
			&& h.getGeneration() == generations[h.getIndex()])
		{
			referencesCount[h.getIndex()] -= 1u;
		}
	}
};

} // namespace dj;