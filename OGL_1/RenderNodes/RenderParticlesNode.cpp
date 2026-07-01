#include "RenderParticlesNode.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "Material.h"
#include "Program.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace dj;

RenderParticlesNode::RenderParticlesNode(
	const TextureManager& texMgr,
	const FramebufferManager& fboMgr,
	FramebufferHandle output,
	CameraPtr camera,
	std::shared_ptr<ParticleSystem> particles,
	MaterialPtr material,
	const std::string& name
)
	: IRenderNode(texMgr, fboMgr, output, name)
	, camera(camera)
	, particles(particles)
	, material(material)
{
}

void RenderParticlesNode::draw()
{
	if (particles && camera && material)
	{
		if (!material->getProgram().expired())
		{
			ProgramPtrConst program = material->getProgram().lock();
			program->use();

			static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.1f, 0.1f));
			glUniformMatrix4fv(program->getUniformLocation("u_scale"), 1, GL_FALSE, glm::value_ptr(scale));
			glUniformMatrix4fv(program->getUniformLocation("u_camVP"), 1, GL_FALSE, glm::value_ptr(camera->getVPMatrix()));
		
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles->count());
		}
	}
}