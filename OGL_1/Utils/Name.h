#pragma once
#include <string>

namespace dj
{

class Name
{
	std::string name;
public:
	Name(const std::string& name = "") noexcept;

	const std::string& getName() const;
	void setName(const std::string& name);
};

} // namespace dj