#pragma once
#include "IRenderNode.h"
#include "Definitions.h"
#include <memory>

namespace dj
{
class TextureManager;
class FramebufferManager;
class ParticleSystem;
} // namespace dj

namespace dj
{

class RenderParticlesNode : public IRenderNode {

	CameraPtr camera;
	std::shared_ptr<ParticleSystem> particles;
	MaterialPtr material;
public:
	RenderParticlesNode(
		const TextureManager& texMgr,
		const FramebufferManager& fboMgr,
		FramebufferHandle output,
		CameraPtr camera,
		std::shared_ptr<ParticleSystem> particles,
		MaterialPtr material,
		const std::string& name = ""
	);

	virtual void draw() final;
};

} // namespace dj