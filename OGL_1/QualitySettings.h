#pragma once
#include <array>
#include <tuple>

namespace dj
{

enum class EngineQuality
{
	Shadow2DResolution,
	ShadowCubeResolution,
	MainFBHighPrecision,
};

// 0 - highest quality
template <unsigned int QualityLevels>
class QualitySettings
{
	unsigned int activeLevel;

	// Arrays that stores Engine Quality Settings (for each Quality Level)
	using TupleType = std::tuple<
		std::array<unsigned int, QualityLevels>,	//Shadow2DResolution
		std::array<unsigned int, QualityLevels>,	//ShadowCubeResolution
		std::array<bool, QualityLevels>				//MainFBHighPrecision
	>;
	TupleType engineQualities;
	constexpr static size_t engineQualitiesCount = std::tuple_size<TupleType>::value;

	// Mapping of the names of Engine Quality Settings (it is needed for runtime mapping)
	constexpr static std::array<EngineQuality, engineQualitiesCount> namesMapping = {
		EngineQuality::Shadow2DResolution,
		EngineQuality::ShadowCubeResolution,
		EngineQuality::MainFBHighPrecision
	};

	// Structure for compile-time indexing (by EngineQuality)
	template <EngineQuality engineQuality>
	struct EngineQualityIndex;

	// EngineQuality to engineQualities mapping
	template <> struct EngineQualityIndex<namesMapping[0]> : std::integral_constant<std::size_t, 0> {};
	template <> struct EngineQualityIndex<namesMapping[1]> : std::integral_constant<std::size_t, 1> {};
	template <> struct EngineQualityIndex<namesMapping[2]> : std::integral_constant<std::size_t, 2> {};

	// Needed for runtime mapping
	template <EngineQuality engineQuality>
	consteval static auto getIndex()
	{
		for (size_t i = 0; i < engineQualitiesCount; ++i)
		{
			if (namesMapping[i] == engineQuality)
			{
				return i;
			}
		}

		return static_cast<size_t>(0);
	}

public:

	QualitySettings()
		: activeLevel(0)
	{
	}

	QualitySettings(const QualitySettings& gs)
		: activeLevel(gs.activeLevel)
		, engineQualities(gs.engineQualities)
	{
	}

	QualitySettings(QualitySettings&&) = delete;

	bool setQualityLevel(unsigned int level)
	{
		if (level < QualityLevels)
		{
			activeLevel = level;
			return true;
		}

		return false;
	}

	unsigned int getQualityLevel() const
	{
		return activeLevel;
	}

	constexpr unsigned int getQualityLevels() const
	{
		return QualityLevels;
	}

	template <EngineQuality engineQuality>
	constexpr auto& get() const
	{
		return getArray<engineQuality>()[activeLevel];
	}

	template <EngineQuality engineQuality, unsigned int QualityLevel>
	constexpr auto& get() const
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		return getArray<engineQuality>()[QualityLevel];
	}

	template <typename Type>
	std::optional<Type> get(EngineQuality engineQuality, unsigned int index)
	{
		if (index < QualityLevels)
		{
			for (size_t i = 0; i < engineQualitiesCount; ++i)
			{
				if (engineQuality == namesMapping[i])
				{
					return engineQualities
				}
			}
		}
	}

	template <EngineQuality engineQuality, typename Type>
	bool set(Type value, unsigned int qualityLevel)
	{
		if (qualityLevel < QualityLevels)
		{
			getArray<engineQuality>()[qualityLevel] = value;
			return true;
		}

		return false;
	}

	template <EngineQuality engineQuality, unsigned int qualityLevel, typename Type>
	void set(Type value)
	{
		static_assert(qualityLevel < QualityLevels, "Quality Level out of bounds");
		getArray<engineQuality>()[qualityLevel] = value;
	}

	template <EngineQuality engineQuality, typename Type>
	void set(const std::array<Type, QualityLevels>& values)
	{
		getArray<engineQuality>() = values;
	}

private:
	template <EngineQuality engineQuality>
	constexpr auto& getArray()
	{
		return std::get<EngineQualityIndex<engineQuality>::value>(engineQualities);
	}
};

} // namespace dj