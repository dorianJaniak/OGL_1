#include "ReferencesManager.h"
#include "Handle.h"
using namespace dj;

void ReferencesManager::addRef(const Handle& h)
{
	if (h.getIndex() < referencesCount.size() 
		&& h.getGeneration() == generations[h.getIndex()])
	{
		referencesCount[h.getIndex()] += 1u;
	}
}

void ReferencesManager::removeRef(const Handle& h)
{
	if (h.getIndex() < referencesCount.size()
		&& h.getGeneration() == generations[h.getIndex()])
	{
		referencesCount[h.getIndex()] -= 1u;
	}
}