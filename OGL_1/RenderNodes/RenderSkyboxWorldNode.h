#pragma once
#include "IRenderWorldNode.h"
#include "CameraNodeProperty.h"

namespace dj
{

/*! \class RenderSkyboxWorldNode
	\brief Node for rendering Skybox Cubemap
*/
class RenderSkyboxWorldNode : public IRenderWorldNode, public CameraNodeProperty
{
public:
	RenderSkyboxWorldNode(FramebufferPtr output, CameraPtr camera, GLuint ebo, std::vector<dj::ObjectInstancePtr>& objectInstances, const std::string& name = "");

protected:
	virtual void uboPerFrame() override;
	virtual void uniformPerProgram(ConstProgramPtr program) override;
	virtual void uniformPerMaterial(ConstProgramPtr program, ConstMaterialPtr material) override;
	virtual void uniformPerObject(ConstProgramPtr program, ObjectInstancePtr obj) override;
};

} // namespace dj