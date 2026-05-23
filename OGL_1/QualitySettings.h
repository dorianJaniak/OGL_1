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

	// Structure for compile-time indexing (by EngineQuality)
	template <EngineQuality engineQuality>
	struct EngineQualityIndex;

	// EngineQuality to engineQualities mapping
	template <> struct EngineQualityIndex<EngineQuality::Shadow2DResolution> : std::integral_constant<std::size_t, 0> {};
	template <> struct EngineQualityIndex<EngineQuality::ShadowCubeResolution> : std::integral_constant<std::size_t, 1> {};
	template <> struct EngineQualityIndex<EngineQuality::MainFBHighPrecision> : std::integral_constant<std::size_t, 2> {};

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

	bool setActiveLevel(unsigned int level)
	{
		if (level < QualityLevels)
		{
			activeLevel = level;
			return true;
		}

		return false;
	}

	unsigned int getActiveLevel() const
	{
		return activeLevel;
	}

	constexpr unsigned int getLevelsCount() const
	{
		return QualityLevels;
	}

	template <EngineQuality engineQuality>
	constexpr auto getActive() const
	{
		return getArray<engineQuality>()[activeLevel];
	}

	template <EngineQuality engineQuality, unsigned int QualityLevel>
	constexpr auto get() const
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		return getArray<engineQuality>()[QualityLevel];
	}

	template <EngineQuality engineQuality>
	constexpr const auto& get()
	{
		return getArray<engineQuality>();
	}

	template <EngineQuality engineQuality, typename Type>
	bool set(unsigned int qualityLevel, Type value)
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

	template <EngineQuality engineQuality>
	constexpr auto& getArray() const
	{
		return std::get<EngineQualityIndex<engineQuality>::value>(engineQualities);
	}
};

} // namespace dj