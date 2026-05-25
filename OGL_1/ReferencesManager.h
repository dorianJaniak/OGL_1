#pragma once
#include <vector>

namespace dj
{
	class Handle;
}

namespace dj
{

/*! \class ReferencesManager
* \todo consider removing "friend Handle"
*/
class ReferencesManager
{
	friend Handle;

protected:
	std::vector<unsigned int> referencesCount;
	std::vector<unsigned int> generations;

	void addRef(const Handle& handle);
	void removeRef(const Handle& handle);
};

} // namespace dj;