#include "RenderDepthWorldNode.h"
#include "Program.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace dj;

RenderDepthWorldNode::RenderDepthWorldNode(
	const TextureManager& texMgr,
	const FramebufferManager& fboMgr,
	FramebufferHandle output,
	CameraPtr camera,
	GLuint ebo,
	std::vector<dj::ObjectInstancePtr>& objectInstances,
	const std::string& name)
	: IRenderWorldNode(texMgr, fboMgr, output, ebo, objectInstances, name)
	, CameraNodeProperty(camera)
{
}

void RenderDepthWorldNode::uboPerFrame()
{

}

void RenderDepthWorldNode::uniformPerProgram(ConstProgramPtr program)
{
	glUniformMatrix4fv(program->getUniformLocation("u_camVP"), 1, GL_FALSE, glm::value_ptr(camera->getVPMatrix()));
	glUniform1f(program->getUniformLocation("u_near"), camera->getNearPlane());
	glUniform1f(program->getUniformLocation("u_far"), camera->getFarPlane());
}

void RenderDepthWorldNode::uniformPerMaterial(ConstProgramPtr program, ConstMaterialPtr material)
{

}

void RenderDepthWorldNode::uniformPerObject(ConstProgramPtr program, ObjectInstancePtr obj)
{
	glUniformMatrix4fv(program->getUniformLocation("u_model"), 1, GL_FALSE, glm::value_ptr(obj->getTransformation()));
}