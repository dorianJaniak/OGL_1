#pragma once
#include <array>
#include <tuple>

namespace dj
{

enum class EngineSetting
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
	> engineSettings;

	template <EngineSetting engineSetting>
	struct EngineSettingIndex;

	template <> struct EngineSettingIndex<EngineSetting::Shadow2DResolution> : std::integral_constant<std::size_t, 0> {};
	template <> struct EngineSettingIndex<EngineSetting::ShadowCubeResolution> : std::integral_constant<std::size_t, 1> {};
	template <> struct EngineSettingIndex<EngineSetting::MainFBHighPrecision> : std::integral_constant<std::size_t, 2> {};

public:

	QualitySettings()
		: activeLevel(0)
	{
	}

	QualitySettings(const QualitySettings& gs)
		: activeLevel(gs.activeLevel)
		, engineSettings(gs.engineSettings)
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

	template <EngineSetting engineSetting>
	constexpr auto get()
	{
		return getArray<engineSetting>()[activeLevel];
	}

	template <EngineSetting engineSetting, unsigned int QualityLevel>
	constexpr auto get()
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		return getArray<engineSetting>()[QualityLevel];
	}

	template <EngineSetting engineSetting, unsigned int QualityLevel, typename Type>
	void set(Type value)
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		getArray<engineSetting>()[QualityLevel] = value;
	}

	template <EngineSetting engineSetting, typename Type>
	void set(const std::array<Type, QualityLevels>& values)
	{
		getArray<engineSetting>() = values;
	}

private:
	template <EngineSetting engineSetting>
	constexpr auto& getArray()
	{
		return std::get<EngineSettingIndex<engineSetting>::value>(engineSettings);
	}
};

} // namespace dj