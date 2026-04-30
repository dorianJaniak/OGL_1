#pragma once

namespace dj
{

class RenderingResources
{
	unsigned int vramLimit; // In Bytes

public:
	RenderingResources();
	RenderingResources(const RenderingResources&) = delete;
	RenderingResources(RenderingResources&&) = delete;
	~RenderingResources();

	void queryVRAMLimit();
	void setVRAMLimit(unsigned int limit);

};

} // namespace dj