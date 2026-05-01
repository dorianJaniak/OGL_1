#pragma once
#include <array>
#include <tuple>

namespace dj
{

enum class EngineSetting
{
	Shadow2DResolution,
	ShadowCubeResolution,
};

// 0 - highest quality
template <unsigned int QualityLevels>
class QualitySettings
{
	std::tuple<
		std::array<unsigned int, QualityLevels>, //Shadow2DResolution;
		std::array<unsigned int, QualityLevels> //ShadowCubeResolution;
	> engineSettings;

	template <EngineSetting engineSetting>
	struct EngineSettingIndex;

	template <> struct EngineSettingIndex<EngineSetting::Shadow2DResolution> : std::integral_constant<std::size_t, 0> {};
	template <> struct EngineSettingIndex<EngineSetting::ShadowCubeResolution> : std::integral_constant<std::size_t, 1> {};

public:

	QualitySettings()
	{
	}

	QualitySettings(const QualitySettings& gs)
		: engineSettings(gs.engineSettings)
	{
	}

	QualitySettings(QualitySettings&&) = delete;

	template <EngineSetting engineSetting, unsigned int QualityLevel>
	constexpr auto get()
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		return get<engineSetting>()[QualityLevel];
	}

	template <EngineSetting engineSetting, unsigned int QualityLevel, typename Type>
	void set(Type value)
	{
		static_assert(QualityLevel < QualityLevels, "Quality Level out of bounds");
		get<engineSetting>()[QualityLevel] = value;
	}

	template <EngineSetting engineSetting, typename Type>
	void set(const std::array<Type, QualityLevels>& values)
	{
		get<engineSetting>() = values;
	}

	constexpr unsigned int getQualityLevels()
	{
		return QualityLevels;
	}

private:
	template <EngineSetting engineSetting>
	constexpr auto& get()
	{
		return std::get<EngineSettingIndex<engineSetting>::value>(engineSettings);
	}
};

} // namespace dj