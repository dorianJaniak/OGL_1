#pragma once
#include "IRenderWorldNode.h"
#include "CameraNodeProperty.h"
#include "..\Definitions.h"

namespace dj
{
class TextureManager;
} // namespace dj

namespace dj
{

/*! \class RenderShadedWorldNode
	\brief Currently supports only specific PBR Shader

	\todo
	- Support custom Programs / Shaders
*/
class RenderShadedWorldNode : public IRenderWorldNode, public CameraNodeProperty {

	std::vector<dj::LightPtr>& lights;
	const std::vector<dj::LightFramebufferBinding>& spotFBOs;
	const std::vector<dj::LightFramebufferBinding>& pointFBOs;
public:
	RenderShadedWorldNode(const TextureManager& texMgr, FramebufferPtr output, CameraPtr camera, GLuint ebo, std::vector<dj::ObjectInstancePtr>& objectInstances,
		const std::vector<dj::LightFramebufferBinding>& spotFBOs,
		const std::vector<dj::LightFramebufferBinding>& pointFBOs,
		std::vector<dj::LightPtr>& lights,
		const std::string& name = "");

protected:
	virtual void uboPerFrame() override;
	virtual void uniformPerProgram(ConstProgramPtr program) override;
	virtual void uniformPerMaterial(ConstProgramPtr program, ConstMaterialPtr material) override;
	virtual void uniformPerObject(ConstProgramPtr program, ObjectInstancePtr obj) override;

	virtual void bindAndUniformTextures() override;

private:
	void uniformLight(ConstProgramPtr program, unsigned int index, const LightPtr& light);
};

} // namespace dj