#include "Name.h"
using namespace dj;

Name::Name(const std::string& name) noexcept
	: name(name)
{
}

const std::string& Name::getName() const
{
	return name;
}

void Name::setName(const std::string& name)
{
	this->name = name;
}