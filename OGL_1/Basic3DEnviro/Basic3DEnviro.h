#pragma once
#include <string_view>
#include <array>

namespace dj_basicEnviro
{
inline constexpr unsigned int pbrMaterialsCount = 2u;
extern const std::array<std::string_view, pbrMaterialsCount> pbrMaterialPaths;
extern const std::array<std::string_view, pbrMaterialsCount> pbrMaterialFileExtensions;

inline constexpr unsigned int skyboxMaterialsCount = 1u;
extern const std::array<std::string_view, skyboxMaterialsCount> skyboxMaterialPaths;
extern const std::array<std::string_view, skyboxMaterialsCount> skyboxMaterialFileExtensions;
} // namescpae dj_basicEnviro