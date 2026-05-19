#pragma once

namespace dj
{
class TextureManager;
class TextureReferencesManager;
} // namespace dj

namespace dj
{

/*! \class TextureHandle
* \todo Join index and generation to uint32_t
* \todo Consider if it should set Index to max (it is not safe to use assert in case of std::move). Should I set manager to nullptr?
*/
class TextureHandle
{
	friend TextureManager;

	TextureReferencesManager* manager{ nullptr };
	unsigned int index{ 0u };
	unsigned int generation{ 0u };

public:
	TextureHandle() = delete;
	TextureHandle(const TextureHandle& handle) noexcept;
	TextureHandle(TextureHandle&& handle) noexcept;
	TextureHandle& operator=(const TextureHandle& handle) noexcept;
	TextureHandle& operator=(TextureHandle&& handle) noexcept;
	bool operator==(const TextureHandle&) const = default;

	~TextureHandle();

	unsigned int getIndex() const;
	unsigned int getGeneration() const;

private:
	// accessible only for TextureManager
	TextureHandle(TextureReferencesManager* manager, unsigned int index, unsigned int generation);
};

} // namespace dj