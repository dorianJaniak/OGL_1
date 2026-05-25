#include "TextureReferencesManager.h"
#include "Handle.h"
using namespace dj;

void TextureReferencesManager::addRef(const Handle& h)
{
	if (h.getIndex() < referencesCount.size() 
		&& h.getGeneration() == generations[h.getIndex()])
	{
		referencesCount[h.getIndex()] += 1u;
	}
}

void TextureReferencesManager::removeRef(const Handle& h)
{
	if (h.getIndex() < referencesCount.size()
		&& h.getGeneration() == generations[h.getIndex()])
	{
		referencesCount[h.getIndex()] -= 1u;
	}
}