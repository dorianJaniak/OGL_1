#pragma once
#include "TextureColorEnums.h"
#include "TextureParameterEnums.h"
#include "GLTypes.h"
#include <optional>

namespace dj
{

// Section: toGLenum functions
template <class T>
constexpr GLenum toGLenum(T value)
{
	return selectEnumMapping<T>()[static_cast<unsigned int>(value)];
}

template <class T>
constexpr unsigned int toBitSize(T value)
{
	return selectBitSizeMapping<T>()[static_cast<unsigned int>(value)];
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
			return static_cast<T>(value);
		}
	}

	return std::nullopt;
}

} // namespace dj