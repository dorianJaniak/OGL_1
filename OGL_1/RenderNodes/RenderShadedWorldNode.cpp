#include "RenderShadedWorldNode.h"
#include "../Program.h"
#include "../EngineKeywords.h"
#include "../Light.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
using namespace dj;

RenderShadedWorldNode::RenderShadedWorldNode(
	const TextureManager& texMgr,
	const FramebufferManager& fboMgr,
	FramebufferHandle output,
	CameraPtr camera, 
	GLuint ebo, 
	std::vector<dj::ObjectInstancePtr>& objectInstances, 
	const std::vector<dj::LightFramebufferBinding>& spotFBOs,
	const std::vector<dj::LightFramebufferBinding>& pointFBOs,
	std::vector<dj::LightPtr>& lights,
	const std::string& name)
	: IRenderWorldNode(texMgr, fboMgr, output, ebo, objectInstances, name)
	, CameraNodeProperty(camera)
	, lights(lights)
	, spotFBOs(spotFBOs)
	, pointFBOs(pointFBOs)
{
}

void RenderShadedWorldNode::uboPerFrame()
{

}

void RenderShadedWorldNode::uniformPerProgram(ConstProgramPtr program)
{
	assert(spotFBOs.size() > 0);
	glUniform3fv(program->getUniformLocation("u_wCamPos"), 1, glm::value_ptr(camera->getPosition()));
	glUniform1f(program->getUniformLocation("u_lightNear"), spotFBOs[0].light->getShadowNearPlane());
	glUniform1f(program->getUniformLocation("u_lightFar"), spotFBOs[0].light->getShadowFarPlane());
	glUniform1ui(program->getUniformLocation("u_lightsCount"), static_cast<GLuint>(lights.size()));

	unsigned int maxLightsCount = program->getUniformLimit(Engine::UniformLimits::maxLightsCount);
	unsigned int maxPointLightsCount = program->getUniformLimit(Engine::UniformLimits::maxPointLightsCount);
	unsigned int maxSpotLightsCount = program->getUniformLimit(Engine::UniformLimits::maxSpotLightsCount);

	int pointShadowsCount = 0;
	int spotShadowsCount = 0;

	// Count shadows budget
	spotShadowsCount = static_cast<int>(spotFBOs.size());
	pointShadowsCount = static_cast<int>(pointFBOs.size());

	// Count shadows
	for (const LightPtr& light : lights)
	{
		if (light->isShadowActive())
		{
			switch (light->getType())
			{
				case dj::Light::Type::Spot: --spotShadowsCount; break;
				case dj::Light::Type::Point: --pointShadowsCount; break;
				default: break;
			}
		}
	}

	// Check if number of shadows and framebuffers are consistent
	assert(pointShadowsCount >= 0 && spotShadowsCount >= 0 && "More active shadows than Framebuffers available");

	unsigned int pointLightNo = 0u;
	unsigned int spotLightNo = 0u;

	for (unsigned int i = 0; i < maxLightsCount && i < lights.size(); ++i)
	{
		const dj::LightPtr& light = lights[i];
		uniformLight(program, i, light);

		if (light->getType() == dj::Light::Type::Spot && spotLightNo < spotFBOs.size())
		{
			dj::Camera cam;
			// The same as ShadowFramebuffer::configureCamera
			cam.setPosition(light->getPosition());
			cam.setRotation(glm::vec3(light->tmpXPitch, light->tmpYYaw, 0.0f));
			cam.setPerspective(light->getSpotlightAngle() + 30.0f, 1.0f, light->getShadowNearPlane(), light->getShadowFarPlane());
			glm::mat4 lightVP = cam.getVPMatrix();
			std::string lightVPUniformName = std::string("u_lightVP[") + std::to_string(spotLightNo) + ']';
			glUniformMatrix4fv(program->getUniformLocation(lightVPUniformName), 1, GL_FALSE, glm::value_ptr(lightVP));

			//if (std::optional<TextureHandle> texHandle = spotFBOs[spotLightNo].fbo->getTextureAttachment(GL_DEPTH_ATTACHMENT); texHandle != std::nullopt)
			if (spotFBOs[spotLightNo].fbo.texHandles.size() > 0u)
			{
				std::string shadowUniformName = std::string("u_shadow[") + std::to_string(spotLightNo) + ']';
				bindAndUniformTexture(spotFBOs[spotLightNo].fbo.texHandles[0u], program->getUniformLocation(shadowUniformName));
			}

			++spotLightNo;
		}
		else if (light->getType() == dj::Light::Type::Point && pointLightNo < pointFBOs.size())
		{
			//if (std::optional<TextureHandle> texHandle = pointFBOs[pointLightNo].fbo->getTextureAttachment(GL_DEPTH_ATTACHMENT); texHandle != std::nullopt)
			if (pointFBOs[pointLightNo].fbo.texHandles.size() > 0u)
			{
				std::string shadowUniformName = std::string("u_cubeShadow[") + std::to_string(pointLightNo) + ']';
				bindAndUniformTexture(pointFBOs[pointLightNo].fbo.texHandles[0u], program->getUniformLocation(shadowUniformName));
			}

			++pointLightNo;
		}
	}
}

void RenderShadedWorldNode::uniformPerMaterial(ConstProgramPtr program, ConstMaterialPtr material)
{

}

void RenderShadedWorldNode::uniformPerObject(ConstProgramPtr program, ObjectInstancePtr obj)
{
	glm::mat4 mvpMat = camera->getVPMatrix() * obj->getTransformation();
	glm::mat3 normMat = obj->getNormalToModelSpace();

	glUniformMatrix4fv(program->getUniformLocation("u_model"), 1, GL_FALSE, glm::value_ptr(obj->getTransformation()));
	glUniformMatrix4fv(program->getUniformLocation("u_mvp"), 1, GL_FALSE, glm::value_ptr(mvpMat));
	glUniformMatrix3fv(program->getUniformLocation("u_normalMat"), 1, GL_FALSE, glm::value_ptr(normMat));
}

void RenderShadedWorldNode::bindAndUniformTextures()
{

}

void RenderShadedWorldNode::uniformLight(ConstProgramPtr program, unsigned int index, const LightPtr& light)
{
	std::string lightName = std::string("u_light[") + std::to_string(index) + "].";

	std::string posName = lightName + std::string("pos");
	std::string colorName = lightName + std::string("color");
	std::string rangeName = lightName + std::string("range");
	std::string typeName = lightName + std::string("type");
	std::string dirName = lightName + std::string("dir");
	std::string spotExtCosName = lightName + std::string("spotExtCos");
	std::string spotIntCosName = lightName + std::string("spotIntCos");
	std::string shadowActiveName = lightName + std::string("shadowActive");

	glUniform3fv(program->getUniformLocation(posName.c_str()), 1, glm::value_ptr(light->getPosition()));
	glUniform3fv(program->getUniformLocation(colorName.c_str()), 1, glm::value_ptr(light->getColor() * light->getIntensity()));
	glUniform1f(program->getUniformLocation(rangeName.c_str()), light->getRange());
	glUniform1ui(program->getUniformLocation(typeName.c_str()), static_cast<GLuint>(light->getType()));
	glUniform1i(program->getUniformLocation(shadowActiveName.c_str()), (light->isShadowActive() ? 1 : 0));

	if (light->getType() != dj::Light::Type::Point)
	{
		glUniform3fv(program->getUniformLocation(dirName.c_str()), 1, glm::value_ptr(light->getDirectionVector()));

		if (light->getType() == dj::Light::Type::Spot)
		{
			glUniform1f(program->getUniformLocation(spotExtCosName.c_str()), glm::cos(glm::radians(light->getSpotlightAngle() / 2.0f)));
			glUniform1f(program->getUniformLocation(spotIntCosName.c_str()), glm::cos(glm::radians(light->getSpotlightInternalAngle() / 2.0f)));
		}
	}
}