#include "Basic3DEnviro.h"

namespace dj_basicEnviro {

extern const std::array<std::string_view, pbrMaterialsCount> pbrMaterialPaths = {
	"res/textures/peeling-painted-metal-bl/peeling-painted-metal_",
	//"res/textures/alien-panels-bl/alien-panels_",
	//"res/textures/worn-painted-metal-bl/worn-painted-metal_",
	//"res/textures/bricks-mortar-bl/bricks-mortar-",
	//"res/textures/dirty-flat-stonework-bl/dirty-flat-stonework_",
	"res/textures/windswept-wasteland-bl/windswept-wasteland_",
	//"res/textures/windswept-wasteland-bl_512/windswept-wasteland_",
	//"res/textures/square-block-vegetation-bl/square-blocks-vegetation_",
	//"res/textures/rough-wet-cobble-bl/rough-wet-cobble-",
};

extern const std::array<std::string_view, pbrMaterialsCount> pbrMaterialFileExtensions = {
	".png",
	".png",
};

extern const std::array<std::string_view, skyboxMaterialsCount> skyboxMaterialPaths = {
	"res/textures/skybox/",
};

extern const std::array<std::string_view, skyboxMaterialsCount> skyboxMaterialFileExtensions = {
	".jpg",
};

} // namespace dj_basicEnviro