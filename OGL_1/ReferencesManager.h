#pragma once
#include <vector>

namespace dj
{
template <class ResourceManager>
class Handle;
}

namespace dj
{

/*! \class ReferencesManager
* \todo consider removing "friend Handle"
*/
template <class ResourceManager>
class ReferencesManager
{
	friend class Handle<ResourceManager>;

protected:
	std::vector<unsigned int> referencesCount;
	std::vector<unsigned int> generations;

	void addRef(const Handle<ResourceManager>& h)
	{
		if (h.getIndex() < referencesCount.size()
			&& h.getGeneration() == generations[h.getIndex()])
		{
			referencesCount[h.getIndex()] += 1u;
		}
	}

	void removeRef(const Handle<ResourceManager>& h)
	{
		if (h.getIndex() < referencesCount.size()
			&& h.getGeneration() == generations[h.getIndex()])
		{
			referencesCount[h.getIndex()] -= 1u;
		}
	}

public:

	bool exists(const Handle<ResourceManager>& h) const
	{
		return h.belongsTo(this) && (h.getIndex() < generations.size() && h.getGeneration() == generations[h.getIndex()]);
	}
};

} // namespace dj;