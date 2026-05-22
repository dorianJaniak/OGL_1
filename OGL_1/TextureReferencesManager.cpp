#include "TextureReferencesManager.h"
#include "TextureHandle.h"
using namespace dj;

void TextureReferencesManager::addRef(const TextureHandle& h)
{
	if (h.getIndex() < referencesCount.size() 
		&& h.getGeneration() == generations[h.getIndex()])
	{
		referencesCount[h.getIndex()] += 1u;
	}
}

void TextureReferencesManager::removeRef(const TextureHandle& h)
{
	if (h.getIndex() < referencesCount.size()
		&& h.getGeneration() == generations[h.getIndex()])
	{
		referencesCount[h.getIndex()] -= 1u;
	}
}