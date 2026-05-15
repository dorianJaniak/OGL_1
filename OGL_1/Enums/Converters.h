#pragma once
#include "TextureColorEnums.h"
#include "TextureParameterEnums.h"

namespace dj
{

// Section: toGLenum functions
template <class T>
inline constexpr GLenum toGLenum(T value)
{
	return selectEnumMapping<T>[static_cast<unsigned int>(value)]);
}

template <class T>
inline constexpr unsigned int toBitSize(T value)
{
	return selectBitSizeMapping[static_cast<unsigned int>(value)]);
}

// Section: toEnum functions
template <class T>
std::optional<T> toEnum(GLenum value)
{
	const auto& mapping = selectEnumMapping<T>();
	const unsigned int size = std::size(mapping);

	for (unsigned int i = 0; i < size; ++i)
	{
		if (mapping[i] == value)
		{
			return value;
		}
	}

	return std::nullopt;
}

} // namespace dj