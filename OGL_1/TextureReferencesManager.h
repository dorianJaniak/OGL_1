#pragma once
#include <vector>

namespace dj
{
	class TextureHandle;
}

namespace dj
{

/*! \class TextureReferencesManager
* \todo consider removing "friend TextureHandle"
*/
class TextureReferencesManager
{
	friend TextureHandle;

protected:
	std::vector<unsigned int> referencesCount;
	std::vector<unsigned int> generations;

	void addRef(const TextureHandle& handle);
	void removeRef(const TextureHandle& handle);
};

} // namespace dj;