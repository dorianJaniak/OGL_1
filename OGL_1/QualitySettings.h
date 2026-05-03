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

	std::tuple<
		std::array<unsigned int, QualityLevels>,	//Shadow2DResolution
		std::array<unsigned int, QualityLevels>,	//ShadowCubeResolution
		std::array<bool, QualityLevels>				//MainFBHighPrecision
	> engineQualities;

	template <EngineQuality engineQuality>
	struct EngineQualityIndex;

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
	constexpr auto get()
	{
		return getArray<engineQuality>()[activeLevel];
	}

	template <EngineQuality engineQuality, unsigned int QualityLevel>
	constexpr auto get()
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		return getArray<engineQuality>()[QualityLevel];
	}

	template <EngineQuality engineQuality, unsigned int QualityLevel, typename Type>
	void set(Type value)
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		getArray<engineQuality>()[QualityLevel] = value;
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