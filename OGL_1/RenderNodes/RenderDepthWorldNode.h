#pragma once
#include "IRenderWorldNode.h"
#include "CameraNodeProperty.h"

namespace dj
{

/*! \class RenderDepthWorldNode
	\brief Not Used / Not implemented
*/
class RenderDepthWorldNode : public IRenderWorldNode, public CameraNodeProperty
{
public:
	RenderDepthWorldNode(FramebufferPtr output, CameraPtr camera, GLuint ebo, std::vector<dj::ObjectInstancePtr>& objectInstances, const std::string& name = "");

protected:
	virtual void uboPerFrame() override;
	virtual void uniformPerProgram(ConstProgramPtr program) override;
	virtual void uniformPerMaterial(ConstProgramPtr program, ConstMaterialPtr material) override;
	virtual void uniformPerObject(ConstProgramPtr program, ObjectInstancePtr obj) override;
};
} // namespace dj