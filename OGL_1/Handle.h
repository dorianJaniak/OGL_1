#pragma once

namespace dj
{
class TextureManager;
class TextureReferencesManager;
} // namespace dj

namespace dj
{

/*! \class Handle
* \todo Join index and generation to uint32_t
* \todo Consider if it should set Index to max (it is not safe to use assert in case of std::move). Should I set manager to nullptr?
*/
class Handle
{
	friend TextureManager;

	TextureReferencesManager* manager{ nullptr };
	unsigned int index{ 0u };
	unsigned int generation{ 0u };

public:
	Handle() = delete;
	Handle(const Handle& handle) noexcept;
	Handle(Handle&& handle) noexcept;
	Handle& operator=(const Handle& handle) noexcept;
	Handle& operator=(Handle&& handle) noexcept;
	bool operator==(const Handle&) const = default;

	~Handle();

	unsigned int getIndex() const;
	unsigned int getGeneration() const;

private:
	// accessible only for TextureManager
	Handle(TextureReferencesManager* manager, unsigned int index, unsigned int generation);
};

} // namespace dj