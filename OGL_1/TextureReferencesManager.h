#pragma once
#include <vector>

namespace dj
{
	class Handle;
}

namespace dj
{

/*! \class TextureReferencesManager
* \todo consider removing "friend Handle"
*/
class TextureReferencesManager
{
	friend Handle;

protected:
	std::vector<unsigned int> referencesCount;
	std::vector<unsigned int> generations;

	void addRef(const Handle& handle);
	void removeRef(const Handle& handle);
};

} // namespace dj;