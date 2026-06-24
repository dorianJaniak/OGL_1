#include "Log.h"
#include <iostream>
using namespace dj;

void Log::print(std::ostream& stream) const
{
	stream << description;
}