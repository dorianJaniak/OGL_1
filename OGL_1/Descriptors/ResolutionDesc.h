#pragma once

namespace dj
{

struct ResolutionDesc
{
	unsigned int width;
	unsigned int height;

	bool operator==(const ResolutionDesc&) const = default;
};

} // namespace dj