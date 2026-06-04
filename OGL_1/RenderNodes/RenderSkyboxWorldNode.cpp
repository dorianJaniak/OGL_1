#include "RenderSkyboxWorldNode.h"
#include "../Program.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace dj;

RenderSkyboxWorldNode::RenderSkyboxWorldNode(
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

void RenderSkyboxWorldNode::uboPerFrame()
{

}

void RenderSkyboxWorldNode::uniformPerProgram(ConstProgramPtr program)
{
	glm::mat4 skyboxVP = camera->getSkyboxMatrix();
	glUniformMatrix4fv(program->getUniformLocation("u_camRP"), 1, GL_FALSE, glm::value_ptr(skyboxVP));
}

void RenderSkyboxWorldNode::uniformPerMaterial(ConstProgramPtr program, ConstMaterialPtr material)
{

}

void RenderSkyboxWorldNode::uniformPerObject(ConstProgramPtr program, ObjectInstancePtr obj)
{

}