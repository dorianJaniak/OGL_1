#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "MainWindow.h"
#include "QualitySettings.h"

#include "RenderNodes/RenderShadedWorldNode.h"
#include "RenderNodes/RenderDepthWorldNode.h"
#include "RenderNodes/RenderSkyboxWorldNode.h"
#include "RenderNodes/IRenderNode.h"
#include "TextureManager.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Object.h"
#include "Material.h"
#include "Shader.h"
#include "Program.h"
#include "Camera.h"
#include "CameraCube.h"
#include "Light.h"
#include "ShadowFramebuffer.h"
#include "Framebuffer.h"
#include "TGLTFLoader.h"
#include "DefinitionsGL.h"
#include "EngineKeywords.h"
#include "Predefinitions\PredefinedShaders.h"
#include "Predefinitions\PredefinedMeshes.h"
#include "Basic3DEnviro/Basic3DEnviro.h"
#include "Basic3DEnviro/DebugTweaks.h"
#include "Time/TimeDrivenMovement.h"
#include "Time/PerFrameUpdates.h"
#include "Time/OccurrenceFrequency.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <memory>
#include <array>

// Helpers - Loaders
bool loadEnginePrograms(std::map<dj::EngineProgramID, dj::ProgramPtr>& enginePrograms);
bool setupEnginePrograms(std::map<dj::EngineProgramID, dj::ProgramPtr>& enginePrograms);

bool loadTextures(dj::TextureManager& texMgr, std::vector<dj::TextureHandle>& pbrTextures, std::vector<dj::TextureHandle>& skyboxTextures);
bool loadTexturesPBR(dj::TextureManager& texMgr, std::vector<dj::TextureHandle>& pbrTextures, const char* path, const char* extension);
bool loadTextureCube(dj::TextureManager& texMgr, std::vector<dj::TextureHandle>& cubeTextures, const char* path, const char* extension);

void configureRasterization();
void loadObjects(dj::MeshData& meshData, std::vector<dj::ObjectPtr>& objects);
void setDefaultMaterials(std::vector<dj::ObjectPtr>& objects, dj::MaterialPtr mat);
void setMaterials(std::vector<dj::ObjectInstancePtr>& objectInstances, const std::vector<dj::MaterialPtr>& mat);
void createObjectInstances(const std::vector<dj::ObjectPtr>& objects, std::vector<dj::ObjectInstancePtr>& objectInstances);
void loadLights(std::vector<dj::LightPtr>& lights);

// Helpers - Relations
bool createShadows(dj::TextureManager& texMgr,
	const std::vector<dj::LightPtr>& lights,
	std::vector<dj::LightFramebufferBinding>& spotFBOs,
	std::vector<dj::LightFramebufferBinding>& pointFBOs,
	const dj::QualitySettings<4u>& quality);
bool createMaterials(std::vector<dj::MaterialPtr>& materials,
	const std::map<dj::EngineProgramID, dj::ProgramPtr>& enginePrograms,
	const std::vector<dj::TextureHandle>& texturesPBR,
	const std::vector<dj::TextureHandle>& texturesCube);

// Helpers - Transformations
void maualObjectsPreTransformations(std::vector<dj::ObjectInstancePtr>& instances);
void manualObjectsTransformations(std::vector<dj::ObjectInstancePtr>& instances, const dj::TimeDrivenMovement& tdm);
void updateCamera(GLFWwindow* window, dj::Camera& camera, const dj::TimeDrivenMovement& tdm);
glm::mat3 calcNormalMatrixToViewSpace(const glm::mat4& view, const glm::mat4& model);

// Helpers - Bindings
void uniformLights(dj::ProgramPtr program, const std::vector<dj::LightPtr>& lights);

// Helpers - Debug
bool checkFramebufferStatus(GLenum status);
bool verifyFramebufferStatus(GLenum status);
void reportFPSCallback(unsigned int framesCount, std::chrono::milliseconds period);

// Helpers - Render
void renderWorldForDepthTest(std::vector<dj::ObjectInstancePtr>& instances, dj::ProgramPtr program);
void renderWorldSingleProgram(std::vector<dj::ObjectInstancePtr>& instances, dj::ProgramPtr program, dj::Camera& camera);

/*! \page futureToDo Future Improvements List
*
*   -# TODO: Debug - Artificial Objects for Debugging - Axes
*   -# TODO: Debug - class for debugging various things
*   -# TODO: Organization - Default values and assignments (like for example GL_TEXTURE31 for Skybox)
*   -# TODO: Organization - Template for dirty based updates
*   -# TODO: Organization - Game visual settings file
*   -# TODO: Organization - Default material when Material is not assigned
*   -# TODO: Objects - Container for Object Instances that is sorted by Program
*   -#	It will be useful for drawing objects to minimize Program bindings
*   -# TODO: Objects - Object Instances class to allow drawing the same object multiple times
*   -#	or maybe? Object is in fact the Object Instance?
*   -# TODO: Objects Queue
*   -# TODO: Objects Queue - Objects Layers
*   -# TODO: Objects Queue - Sorting
*   -# TODO: Objects Queue - Rooms
*   -# TODO: Control - Mouse to look
*   -# TODO: Loading - meshes
*   -# TODO: Loading - map (gltf most probably)
*   -# TODO: Loading - materials
*   -# TODO: Searching for lights affecting meshes (uniform only lights in range)
*   -# TODO: Framebuffer effects - Bloom
*   -# TODO: Framebuffer effects - Depth of Field
*   -# TODO: Framebuffer effects - Motion Blur
*   -# TODO: Framebuffer effects - SSAO
*   -# TODO: Framebuffer effects - Distortion (fire)
*   -# TODO: Anti-aliasing
*   -# TODO: Textures - Anisotropic
*   -# TODO: Textures - Resizeable Main Window
*   -# TODO: Alpha Blending
*   -# TODO: Shaders - Shinning
*   -# TODO: Shaders - Transparent Objects
*   -# TODO: Shaders - Accepting light quality parameter
*   -# TODO: Shaders - Nodes
*   -# TODO: Shaders - Geometry Shader
*   -# TODO: Shaders - Tesselation
*   -# TODO: Shaders - Transform shader to PBR Shader
*   -# TODO: Shaders - UBO for Lights?
*   -# TODO: Cubemap - Reflection Probes
*   -# TODO: Shadows - static lights
*   -#  Shadows would be generated only once
*   -# TODO: Shadows - shadows generated only if something has changed nearby
*   -# TODO: Shadows Queue - shadows that needs to be rerendered after change in the world
*   -# TODO: Optimization - Depth Test before Rendering
*   -# TODO: Optimization - Shadows with some Pattern
*   -# TODO: Optimization - Depth of Field with some Pattern
*   -# TODO: Optimization - Skipped Depth Tests for some shadows for some frames
*   -# TODO: Optimization - First check shadow map before calculating light
*   -# TODO: Additionals - Basic Objects (cube, pyramid, sphere, plane, torus, circle)
*   -# TODO: Additionals - Basic Maps to load it to see an example
*   -# TODO: Additionals - Output log
*/

/*! \page renderingLoopOrder Rendering Loop Stages
*   STAGE 1 :::: Window and Context Init \n
*   STAGE 2 :::: Shader Programs Creation \n
*   STAGE 3 :::: Objects Loading \n
*   STAGE 4 :::: FBO Creation \n
*   STAGE 5 :::: FBO for shadow maps \n
*   STAGE 6 :::: Vertex related Buffers Creation \n
*   STAGE 7 :::: Materials Creation and Configuration \n
*   STAGE 8 :::: Pre-loop Settings \n
*   STAGE 8.1 :::: Simple movement definitions \n
*   STAGE 9 :::: Render Loop \n
*   STAGE 9.1 :::: Internal logging \n
*   STAGE 9.2 :::: Update \n
*   STAGE 9.3 :::: Rendering \n
*   STAGE 9.3.1 :::: Render Shadow Maps (Depth Tests) \n
*   STAGE 9.3.1.1 :::: Render Spot Shadow Maps (Depth Tests) \n
*   STAGE 9.3.1.2 :::: Render Cubemap Shadows \n
*   STAGE 9.3.2 :::: Render Debug Cubemap \n
*   STAGE 9.3.3 :::: Render Scene \n
*   STAGE 9.3.3.1 :::: Render World Node \n
*   STAGE 9.3.3.2 :::: Render Skybox Node \n
*   STAGE 9.3.3.3 :::: Render Debug Edges \n
*   STAGE 9.3.4 :::: Draw Main FBO \n
*/

void basicSetupQualitySettings(dj::QualitySettings<4u>& quality)
{
	quality.set<dj::EngineQuality::Shadow2DResolution>(std::array{ 2048u, 1024u, 512u, 256u });
	quality.set<dj::EngineQuality::ShadowCubeResolution>(std::array{ 1024u, 1024u, 512u, 256u });
	quality.set<dj::EngineQuality::MainFBHighPrecision>(std::array{ true, true, true, false });
	quality.setActiveLevel(1u);
}

int main()
{
	dj::QualitySettings<4u> quality;
	basicSetupQualitySettings(quality);

	std::shared_ptr<dj::DebugTweaks> dt = std::make_shared<dj::DebugTweaks>();
	dj::TimeDrivenMovement tdm;

	dj::TextureManager texMgr;

	std::vector<dj::ObjectPtr> objects;
	std::vector<dj::ObjectInstancePtr> objectInstances;
	std::vector<dj::MaterialPtr> materials;
	std::map<dj::EngineProgramID, dj::ProgramPtr> enginePrograms;
	std::vector<dj::LightPtr> lights;
	std::vector<dj::LightFramebufferBinding> spotFBOs;
	std::vector<dj::LightFramebufferBinding> pointFBOs;

	std::optional<dj::TextureHandle> cameraIDs[3];		// Temporary - active cameras
	dj::CameraPtr camera = std::make_shared<dj::Camera>();

	//dj::TGLTFLoader tgltfLoader;
	//return tgltfLoader.load("res/gltfs/twoCubes_altC_rot_texture_customProp/untitled.gltf");

	// STAGE 1 :::: Window and Context Init
	dj::MainWindow mw(1200, 800, (1200.0f / 800.0f));
	if (!mw.initGLFW(3, 3, "Prosty silnik"))
	{
		return -1;
	}
	mw.addCallback(dt);

	// STAGE 2 :::: Shader Programs Creation
	// Loads, compiles and links shaders and generates Program ID
	if (!loadEnginePrograms(enginePrograms))
	{
		mw.terminate();
		return -1;
	}

	// Setups Texture Units
	if (!setupEnginePrograms(enginePrograms))
	{
		mw.terminate();
		return -1;
	}

	// STAGE 3 :::: Objects Loading
	// Load objects and init their transformations
	dj::MeshData meshData;
	loadObjects(meshData, objects);
	createObjectInstances(objects, objectInstances);
	loadLights(lights);

	// STAGE 4 :::: Cameras Configuration
	camera->setPerspective(30.0f, mw.getAspectRatio(), 0.1f, 300.0f);

	// STAGE 5 :::: FBO Creation
	dj::FramebufferPtr fbo = std::make_shared<dj::Framebuffer>();
	fbo->bind();
	{
		dj::TextureDesc desc{};
		desc.glType = dj::TextureType::Texture2D;
		desc.resolution.width = mw.getWidth();
		desc.resolution.height = mw.getHeight();
		desc.sampling.minFilter = dj::TextureFilteringMin::Nearest;
		desc.sampling.magFilter = dj::TextureFilteringMag::Nearest;
		desc.sampling.wrapS = dj::TextureWrapping::ClampToEdge;
		desc.sampling.wrapT = dj::TextureWrapping::ClampToEdge;
		desc.sampling.wrapR = dj::TextureWrapping::Repeat;
		desc.format.inGPUColorFormat = (quality.getActive<dj::EngineQuality::MainFBHighPrecision>() ?
			dj::ColorFormatInDevice::RGB16F : dj::ColorFormatInDevice::RGB);
		desc.format.sourceColorFormat = dj::ColorFormatInSource::RGB;
		desc.format.sourceValueType = dj::PixelDataTypeInSource::UnsignedByte;

		std::optional<dj::TextureHandle> fboTexture = texMgr.createEmptyTexture(desc);

		if (!fboTexture)
		{
			std::cerr << "Could not create main FBO Texture\n";
			mw.terminate();
			return -1;
		}

		fbo->assignTextureAttachment(texMgr, *fboTexture, GL_COLOR_ATTACHMENT0);
		fbo->genRenderbufferAttachment(GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8);
		if (!verifyFramebufferStatus(fbo->getFramebufferStatus()))
		{
			mw.terminate();
			return -1;
		}
		fbo->unbind();

		cameraIDs[0] = fboTexture;
	}

	GLuint svbo, svao;
	glGenBuffers(1, &svbo);
	glBindBuffer(GL_ARRAY_BUFFER, svbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dj::fullscreenVerts), dj::fullscreenVerts, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &svao);
	glBindVertexArray(svao);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);				// position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));	// UV
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// STAGE 6 :::: FBO for shadow maps
	if (!createShadows(texMgr, lights, spotFBOs, pointFBOs, quality))
	{
		mw.terminate();
		return -1;
	}

	// STAGE 7 :::: Vertex related Buffers Creation
	// VBO - just buffer / data of vertices. GPU would not know what to do with it
	// VAO - informs GPU how to interpret VBO data. It is like data structure definition
	// EBO - stores faces (indices)
	GLuint vbo, vao, ebo;
	
	// It is unnecessary to bind VAO, because glBindBuffer and glBufferData does not
	// modify VAO at all. VAO is informed only by glVertexAttribPointer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, meshData.getVerticesDataSize(), &meshData.getAllVertices()[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);	// It is important to bind it before glVertexAttribPointer
	// glVertexAttribPointer defines structure of the single "line" (single vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, dj::MeshData::vertexComponentsCount * sizeof(float), (void*)NULL);					// position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, dj::MeshData::vertexComponentsCount * sizeof(float), (void*)(3 * sizeof(float)));	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, dj::MeshData::vertexComponentsCount * sizeof(float), (void*)(6 * sizeof(float)));	// tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, dj::MeshData::vertexComponentsCount * sizeof(float), (void*)(9 * sizeof(float)));	// bitangent
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, dj::MeshData::vertexComponentsCount * sizeof(float), (void*)(12 * sizeof(float)));	// UV
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.getIndicesDataSize(), &meshData.getAllIndices()[0], GL_STATIC_DRAW);

	// STAGE 7 :::: Materials Creation
	std::vector<dj::TextureHandle> pbrTextures;
	std::vector<dj::TextureHandle> skyboxTextures;

	if (!loadTextures(texMgr, pbrTextures, skyboxTextures))
	{
		mw.terminate();
		return -1;
	}

	// Materials configuration
	if (!createMaterials(materials, enginePrograms, pbrTextures, skyboxTextures))
	{
		mw.terminate();
		return -1;
	}
	setDefaultMaterials(objects, materials[0]);
	setMaterials(objectInstances, materials);

	// STAGE 8 :::: Pre-loop Settings
	// Configure Rasterization - culling, depth test, etc.
	configureRasterization();

	assert(spotFBOs.size() >= 2);
	for (unsigned int i = 0u; i < 2u; ++i)
	{
		cameraIDs[i + 1u] = spotFBOs[i].fbo->getTextureAttachment(GL_DEPTH_ATTACHMENT);
	}

	// Define some color
	glm::vec3 color(0.4f, 0.6f, 1.0f);

	// Test Pos1
	//camera.setPosition({ 0.0f, 1.0f, 0.0f });

	// Test Pos2
	camera->setPosition({ 20.0f, 4.5f, 8.0f });
	camera->setRotation({-15.0f, 60.0f, 0.0f });

	maualObjectsPreTransformations(objectInstances);

	// STAGE 8.1 :::: Simple movement definitions
	dj::TimeFlow<std::chrono::seconds> tfLightIntensity(std::chrono::seconds(15));
	dj::WahwahSinUpdate light0Move(tdm, lights[0]->getPosition().x, 10.0f, 30.0f);
	dj::LinearUpdate light0Range(tdm, 1.0f, 3.0f);
	dj::WahwahSinUpdate light2RotX(tdm, -40.0f, 30.0f, 90.0f);
	dj::LinearUpdate light3RotY(tdm, 180.0f, 120.0f);

	// STAGE XX :::: Render Nodes creation
	std::vector<dj::ObjectInstancePtr> skyboxObjectInstances;
	{
		dj::ObjectInstancePtr skyboxCubeObjInst = std::make_shared<dj::ObjectInstance>(objects.at(3u), true);
		skyboxCubeObjInst->setMaterial(materials.at(2u));
		skyboxObjectInstances.push_back(skyboxCubeObjInst);
	}
	// VARIANT 1
	//dj::RenderSkyboxWorldNode skyboxNode{ texMgr, fbo, camera, ebo, skyboxObjectInstances, "Main Color - Skybox" };
	//skyboxNode.setConfiguration(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, false, false, GL_FRONT);
	//dj::RenderShadedWorldNode worldNode{ texMgr, fbo, camera, ebo, objectInstances, shadows, pointShadows, lights, std::string("Main Color - World") };
	//worldNode.setConfiguration(GL_NONE, true, true, GL_NONE);

	// VARIANT 2
	dj::RenderShadedWorldNode worldNode{ texMgr, fbo, camera, ebo, objectInstances, spotFBOs, pointFBOs, lights, std::string("Main Color - World") };
	worldNode.setConfiguration(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, true, true, GL_NONE);
	dj::RenderSkyboxWorldNode skyboxNode{ texMgr, fbo, camera, ebo, skyboxObjectInstances, "Main Color - Skybox" };
	skyboxNode.setConfiguration(GL_NONE, true, false, GL_NONE);

	std::vector<dj::ObjectInstancePtr> cubeMapDebugObjectInstances;
	{
		dj::ObjectInstancePtr cubeObjInst = std::make_shared<dj::ObjectInstance>(objects.at(3u), true);
		cubeObjInst->setMaterial(materials.at(3u));
		cubeMapDebugObjectInstances.push_back(cubeObjInst);
	}
	dj::RenderSkyboxWorldNode cubeMapDebugNode{ texMgr, fbo, camera, ebo, cubeMapDebugObjectInstances, "Debug CubeMap" };
	cubeMapDebugNode.setConfiguration(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, false, false, GL_FRONT);

	dj::OccurrenceFrequency fps(std::chrono::milliseconds(5000u));
	fps.setReportCallback(reportFPSCallback);
	fps.start();

	// STAGE 9 :::: Render Loop
	while (!mw.getShouldClose())
	{
		// STAGE 9.1 :::: Internal logging
		fps.increment();
		tdm.nextFrame();

		// STAGE 9.2 :::: Update
		assert(lights.size() >= 4);
		if (tfLightIntensity.expirationCount())
		{
			tfLightIntensity.start();
			light0Range.resetValue(1.0f);
			lights[0]->setIntensity(lights[0]->getIntensity() + 5.0f);
			std::cout << "Light intensity: " << lights[0]->getIntensity() << std::endl;
		}

		lights[0]->setPosition(light0Move.getVal(), lights[0]->getPosition().y, lights[0]->getPosition().z);
		lights[0]->setRange(light0Range.getVal());
		
		lights[2]->setDirection(light2RotX.getVal(), -60.0f);
		lights[3]->setDirection(-10.0f, light3RotY.getVal());

		updateCamera(mw.getWindow(), *camera, tdm);
		manualObjectsTransformations(objectInstances, tdm);

		// Stage 9.3 :::: Rendering
		glBindVertexArray(vao);

		// Stage 9.3.1 :::: Render Shadow Maps (Depth Tests)
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		enginePrograms[dj::EngineProgramID::depth]->use();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// Stage 9.3.1.1 :::: Render Spot Shadow Maps (Depth Tests)
		for (auto& shadow : spotFBOs)
		{
			// Pointer to active program
			dj::ProgramPtr activeProgram = enginePrograms.at(dj::EngineProgramID::depth);

			// Bind Framebuffer
			shadow.fbo->bind();

			// Copy lights data to camera
			// The same as ShadowFramebuffer::configureCamera
			dj::Camera cam;
			cam.setPosition(shadow.light->getPosition());
			cam.setRotation(glm::vec3(shadow.light->tmpXPitch, shadow.light->tmpYYaw, 0.0f));
			cam.setPerspective(shadow.light->getSpotlightAngle() + 30.0f, 1.0f, shadow.light->getShadowNearPlane(), shadow.light->getShadowFarPlane());

			// Setup rendering
			glViewport(0, 0, shadow.fbo->getWidth(), shadow.fbo->getHeight());
			glClear(GL_DEPTH_BUFFER_BIT);

			// Setup light projection
			glm::mat4 lightVP = cam.getVPMatrix();
			glUniformMatrix4fv(activeProgram->getUniformLocation("u_camVP"), 1, GL_FALSE, glm::value_ptr(lightVP));
			glUniform1f(activeProgram->getUniformLocation("u_near"), shadow.light->getShadowNearPlane());
			glUniform1f(activeProgram->getUniformLocation("u_far"), shadow.light->getShadowFarPlane());

			// Render vertices for Shadows
			renderWorldForDepthTest(objectInstances, activeProgram);
		}

		// Stage 9.3.1.2 :::: Render Cubemap Shadows
		// use shader
		glEnable(GL_DEPTH_TEST);
		enginePrograms.at(dj::EngineProgramID::depthCube)->use();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		for (auto& shadow : pointFBOs)
		{
			// Pointer to active program
			dj::ProgramPtr activeProgram = enginePrograms.at(dj::EngineProgramID::depthCube);

			shadow.fbo->bind();
			dj::CameraCube cameraCube;
			cameraCube.setPlanes(1.0f, 25.0f);
			cameraCube.setPosition(shadow.light->getPosition());

			glUniform1f(activeProgram->getUniformLocation("u_far"), cameraCube.getFar());
			glUniform3fv(activeProgram->getUniformLocation("u_lightPos"), 1, glm::value_ptr(cameraCube.getPosition()));

			for (unsigned int i = 0u; i < 6u; ++i)
			{
				std::string uniformName = std::string("u_cameraMatrices[") + std::to_string(i) + std::string("]");
				glUniformMatrix4fv(activeProgram->getUniformLocation(uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(cameraCube.getVPMatrix(i)));
			}

			glViewport(0, 0, shadow.fbo->getWidth(), shadow.fbo->getHeight());
			glClear(GL_DEPTH_BUFFER_BIT);
			
			// Render vertices for Shadows (duplicated)
			renderWorldForDepthTest(objectInstances, activeProgram);
		}

		// Stage 9.3.2 :::: Render Debug Cubemap
		if (dt->getActiveCameraIndex() == 3u)
		{
			std::optional<dj::TextureHandle> shTex = pointFBOs.at(1).fbo->getTextureAttachment(GL_DEPTH_ATTACHMENT);
			if (shTex)
			{
				cubeMapDebugNode.addTexture("u_skybox", *shTex);
			}
			cubeMapDebugNode.run();

			glDepthMask(GL_TRUE);
			glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		// Stage 9.3.3 :::: Render Scene
		else
		{
			// Stage 9.3.3.1 :::: Render World Node
			dj::IRenderNode* worldNodeG = &worldNode;
			worldNodeG->run();

			// Stage 9.3.3.2 :::: Render Skybox Node
			glDepthFunc(GL_LEQUAL);
			skyboxNode.run();

			// Stage 9.3.3.3 :::: Render Debug Edges
			glDepthFunc(GL_LESS);
			if (dt->getDebugVertices())
			{
				assert(enginePrograms.size() >= 5);
				dj::ProgramPtr programDebugMesh = enginePrograms.at(dj::EngineProgramID::debugMesh);
				programDebugMesh->use();
				glUniform1f(programDebugMesh->getUniformLocation("u_axisLength"), 0.3f);
				glUniform1ui(programDebugMesh->getUniformLocation("u_debugSelection"), dt->getDebugVertices());
				renderWorldSingleProgram(objectInstances, programDebugMesh, *camera);
			}
		}

		// Stage 9.3.4 :::: Draw Main FBO
		if (dt->isGammaCorrected())
		{
			glEnable(GL_FRAMEBUFFER_SRGB);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, mw.getWidth(), mw.getHeight());
		enginePrograms[dj::EngineProgramID::postProcessing]->use();
		glBindVertexArray(svbo);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE11);

		if (dt->getActiveCameraIndex() == 3u)
		{
			if (cameraIDs[0u])
			{
				texMgr.bind(*cameraIDs[0u]);
			}
		}
		else
		{
			if (cameraIDs[dt->getActiveCameraIndex()])
			{
				texMgr.bind(*cameraIDs[dt->getActiveCameraIndex()]);
			}
		}
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisable(GL_FRAMEBUFFER_SRGB);

		// Display
		mw.swapBuffers();

		// Poll Events
		mw.pollEvents();
	}

	/* Clear */
	mw.terminate();
	return 0;
}

glm::mat3 calcNormalMatrixToViewSpace(const glm::mat4& view, const glm::mat4& model)
{
	//mat3(transpose(inverse(u_model)))
	return glm::mat3(glm::transpose(glm::inverse(view * model)));
}

void renderWorldForDepthTest(std::vector<dj::ObjectInstancePtr>& instances, dj::ProgramPtr activeProgram)
{
	for (dj::ObjectInstancePtr& obj : instances)
	{
		if (obj->isVisible())
		{
			glUniformMatrix4fv(activeProgram->getUniformLocation("u_model"), 1, GL_FALSE, glm::value_ptr(obj->getTransformation()));
			unsigned int indicesIndexStart = obj->getMeshAlignment().indicesStart;
			glDrawRangeElements(
				GL_TRIANGLES,
				obj->getMeshAlignment().verticesStart,
				obj->getMeshAlignment().verticesEnd,
				obj->getMeshAlignment().indicesCount, GL_UNSIGNED_INT,
				(void*)(indicesIndexStart * sizeof(unsigned int)));
		}
	}
}

void renderWorldSingleProgram(std::vector<dj::ObjectInstancePtr> &instances, dj::ProgramPtr program, dj::Camera &camera)
{
	glUniformMatrix4fv(program->getUniformLocation("u_proj"), 1, GL_FALSE, glm::value_ptr(camera.getPerspectiveMatrix()));
	glUniformMatrix4fv(program->getUniformLocation("u_view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	
	for (dj::ObjectInstancePtr& obj : instances)
	{
		if (obj->isVisible())
		{
			glm::mat3 normalMat = calcNormalMatrixToViewSpace(camera.getViewMatrix(), obj->getTransformation());
			glUniformMatrix4fv(program->getUniformLocation("u_model"), 1, GL_FALSE, glm::value_ptr(obj->getTransformation()));
			glUniformMatrix3fv(program->getUniformLocation("u_normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));
			unsigned int indicesIndexStart = obj->getMeshAlignment().indicesStart;
			glDrawRangeElements(
				GL_TRIANGLES,
				obj->getMeshAlignment().verticesStart,
				obj->getMeshAlignment().verticesEnd,
				obj->getMeshAlignment().indicesCount, GL_UNSIGNED_INT,
				(void*)(indicesIndexStart * sizeof(unsigned int)));
		}
	}
}

bool loadEnginePrograms(std::map<dj::EngineProgramID, dj::ProgramPtr>& programs)
{
	dj::Log log;

	dj::ProgramPtr program = std::make_shared<dj::Program>();
	dj::ProgramPtr programFBO = std::make_shared<dj::Program>();
	dj::ProgramPtr programDepthTest = std::make_shared<dj::Program>();
	dj::ProgramPtr programSkybox = std::make_shared<dj::Program>();
	dj::ProgramPtr programDebugMesh = std::make_shared<dj::Program>();
	dj::ProgramPtr programDepthTestCube = std::make_shared<dj::Program>();

	if (!program->prepare(dj::prePBR_vsSource, dj::pbr_fsSource, log, std::string("PBR Shader")))
	{
		return false;
	}

	if (!programFBO->prepare(dj::fullscreenFBO_vsSource, dj::fullscreenFBO_fsSource, log, std::string("FBO Shader")))
	{
		return false;
	}

	if (!programDepthTest->prepare(dj::depthTestMat_vsSource, dj::depthTestMat_fsSource, log, std::string("Depth Test Shader")))
	{
		return false;
	}

	if (!programSkybox->prepare(dj::skybox_vsSource, dj::skybox_fsSource, log, std::string("Skybox Shader")))
	{
		return false;
	}

	if (!programDebugMesh->prepare(dj::debugMesh_vsSource, dj::debugMesh_fsSource, log, std::string("Debug Mesh Shader"), dj::debugMesh_gsSource))
	{
		return false;
	}

	if (!programDepthTestCube->prepare(dj::depthTestCube_vsSource, dj::depthTestCube_fsSource, log, std::string("Depth Test Cube"), dj::depthTestCube_gsSource))
	{
		return false;
	}

	programs.insert({ dj::EngineProgramID::prePBR, program });
	programs.insert({ dj::EngineProgramID::postProcessing, programFBO });
	programs.insert({ dj::EngineProgramID::depth, programDepthTest });
	programs.insert({ dj::EngineProgramID::skybox, programSkybox });
	programs.insert({ dj::EngineProgramID::debugMesh, programDebugMesh });
	programs.insert({ dj::EngineProgramID::depthCube, programDepthTestCube });
	return true;
}

bool setupEnginePrograms(std::map<dj::EngineProgramID, dj::ProgramPtr>& enginePrograms)
{
	unsigned int uniformsCount = 0u;
	unsigned int uniformsOkCount = 0u;
	auto check = [&uniformsCount, &uniformsOkCount](bool success)
		{
			++uniformsCount;
			if (success)
			{
				++uniformsOkCount;
			}
		};

	auto registerLightsUniforms = [&check](dj::ProgramPtr& program, const unsigned int lightsCount, const char* postfix)
		{
			for (unsigned int i = 0; i < lightsCount; ++i)
			{
				std::string name = "u_light[" + std::to_string(i) + "]." + postfix;
				check(program->registerUniform(name));
			}
		};

	try
	{
		dj::ProgramPtr prePBRProgram = enginePrograms.at(dj::EngineProgramID::prePBR);
		dj::ProgramPtr postprocessProgram = enginePrograms.at(dj::EngineProgramID::postProcessing);
		dj::ProgramPtr depthProgram = enginePrograms.at(dj::EngineProgramID::depth);
		dj::ProgramPtr depthCubeProgram = enginePrograms.at(dj::EngineProgramID::depthCube);
		dj::ProgramPtr skyboxProgram = enginePrograms.at(dj::EngineProgramID::skybox);
		dj::ProgramPtr debugMeshProgram = enginePrograms.at(dj::EngineProgramID::debugMesh);

		// These uniforms needs to be after program use
		prePBRProgram->use();
		constexpr unsigned int lightsCount = 4u;
		prePBRProgram->addUniformLimit(dj::Engine::UniformLimits::maxLightsCount, lightsCount);
		prePBRProgram->addUniformLimit(dj::Engine::UniformLimits::maxPointLightsCount, 2u);
		prePBRProgram->addUniformLimit(dj::Engine::UniformLimits::maxSpotLightsCount, 2u);
		check(prePBRProgram->registerUniform("u_model"));
		check(prePBRProgram->registerUniform("u_mvp"));
		check(prePBRProgram->registerUniform("u_normalMat"));
		check(prePBRProgram->registerUniform("u_wCamPos"));

		check(prePBRProgram->registerUniform("u_material.albedo"));
		check(prePBRProgram->registerUniform("u_material.roughness"));
		check(prePBRProgram->registerUniform("u_material.metallic"));
		check(prePBRProgram->registerUniform("u_material.normal"));
		check(prePBRProgram->registerUniform("u_skybox"));

		check(prePBRProgram->registerUniform("u_cubeShadow[0]"));
		check(prePBRProgram->registerUniform("u_cubeShadow[1]"));
		check(prePBRProgram->registerUniform("u_shadow[0]"));
		check(prePBRProgram->registerUniform("u_shadow[1]"));

		check(prePBRProgram->registerUniform("u_lightsCount"));
		check(prePBRProgram->registerUniform("u_lightNear"));
		check(prePBRProgram->registerUniform("u_lightFar"));
		check(prePBRProgram->registerUniform("u_lightVP[0]"));
		check(prePBRProgram->registerUniform("u_lightVP[1]"));

		registerLightsUniforms(prePBRProgram, lightsCount, "pos");
		registerLightsUniforms(prePBRProgram, lightsCount, "color");
		registerLightsUniforms(prePBRProgram, lightsCount, "range");
		registerLightsUniforms(prePBRProgram, lightsCount, "type");
		registerLightsUniforms(prePBRProgram, lightsCount, "dir");
		registerLightsUniforms(prePBRProgram, lightsCount, "spotExtCos");
		registerLightsUniforms(prePBRProgram, lightsCount, "spotIntCos");
		registerLightsUniforms(prePBRProgram, lightsCount, "shadowActive");

		postprocessProgram->use();
		check(postprocessProgram->registerUniform("frame"));
		postprocessProgram->assignTextureUnit("frame", GL_TEXTURE11);

		skyboxProgram->use();
		check(skyboxProgram->registerUniform("u_camRP"));
		check(skyboxProgram->registerUniform("u_skybox"));
		//skyboxProgram->assignTextureUnit("u_skybox", GL_TEXTURE10);

		debugMeshProgram->use();
		check(debugMeshProgram->registerUniform("u_model"));
		check(debugMeshProgram->registerUniform("u_view"));
		check(debugMeshProgram->registerUniform("u_normalMat"));
		check(debugMeshProgram->registerUniform("u_proj"));
		check(debugMeshProgram->registerUniform("u_axisLength"));
		check(debugMeshProgram->registerUniform("u_debugSelection"));

		depthProgram->use();
		check(depthProgram->registerUniform("u_camVP"));
		check(depthProgram->registerUniform("u_model"));
		check(depthProgram->registerUniform("u_near"));
		check(depthProgram->registerUniform("u_far"));

		depthCubeProgram->use();
		check(depthCubeProgram->registerUniform("u_model"));
		check(depthCubeProgram->registerUniform("u_cameraMatrices[0]"));
		check(depthCubeProgram->registerUniform("u_cameraMatrices[1]"));
		check(depthCubeProgram->registerUniform("u_cameraMatrices[2]"));
		check(depthCubeProgram->registerUniform("u_cameraMatrices[3]"));
		check(depthCubeProgram->registerUniform("u_cameraMatrices[4]"));
		check(depthCubeProgram->registerUniform("u_cameraMatrices[5]"));
		check(depthCubeProgram->registerUniform("u_far"));
		check(depthCubeProgram->registerUniform("u_lightPos"));

		if (uniformsCount != uniformsOkCount)
		{
			std::cerr << dj::Log::failPrefix() << "Could not locate in programs all required uniforms. Located " << uniformsOkCount << " out of " << uniformsCount << std::endl;
			return false;
		}
	}
	catch (const std::out_of_range& ex)
	{
		std::cerr << dj::Log::failPrefix() << __FUNCTION__ << "() Out of Range: " << ex.what() << std::endl;
		return false;
	}

	return true;
}

bool loadTextures(dj::TextureManager& texMgr, std::vector<dj::TextureHandle>& pbrTextures, std::vector<dj::TextureHandle>& skyboxTextures)
{
	static const unsigned int& texsCount = dj_basicEnviro::pbrMaterialsCount;
	static const auto& texPaths = dj_basicEnviro::pbrMaterialPaths;
	static const auto& texExts = dj_basicEnviro::pbrMaterialFileExtensions;

	static const unsigned int& skyboxesCount = dj_basicEnviro::skyboxMaterialsCount;
	static const auto& skyboxPaths = dj_basicEnviro::skyboxMaterialPaths;
	static const auto& skyboxExts = dj_basicEnviro::skyboxMaterialFileExtensions;

	for (unsigned int i = 0u; i < texsCount; ++i)
	{
		if (!loadTexturesPBR(texMgr, pbrTextures, texPaths[i].data(), texExts[i].data()))
		{
			return false;
		}
	}

	for (unsigned int i = 0u; i < skyboxesCount; ++i)
	{
		if (!loadTextureCube(texMgr, skyboxTextures, skyboxPaths[i].data(), skyboxExts[i].data()))
		{
			return false;
		}
	}

	return true;
}

bool loadTexturesPBR(dj::TextureManager& texMgr, std::vector<dj::TextureHandle>& pbrTextures, const char* path, const char* extension)
{
	static const std::string normalSuffix = "normal-ogl";
	static const std::array<std::string, 4> suffixes = {
		"albedo",
		"roughness",
		"metallic",
		normalSuffix,
	};
	static const dj::TextureSamplingDesc& sampling = dj::getTextureSamplingDescDefaultsFor2D();

	for (const auto suffix : suffixes)
	{
		const std::string fullPath = path + suffix + extension;
		bool allowSRGB = (suffix != normalSuffix);
		std::optional<dj::TextureHandle> handle = texMgr.create2DFromFile(sampling, fullPath.c_str(), true, true, allowSRGB);

		if (!handle)
		{
			std::cerr << dj::Log::failPrefix() << "Could not load texture: " << fullPath << std::endl;
			return false;
		}

		pbrTextures.push_back(*handle);
	}

	return true;
}

bool loadTextureCube(dj::TextureManager& texMgr, std::vector<dj::TextureHandle>& cubeTextures, const char* path, const char* extension)
{
	static const dj::TextureSamplingDesc& sampling = dj::getTextureSamplingDescDefaultsForCube();
	const std::array<dj::TextureManager::CubeSideMapping, 6u> suffixesMapping{ {
		{dj::TextureCubeSide::PositiveX, std::string("right") + std::string(extension)},
		{dj::TextureCubeSide::NegativeX, std::string("left") + std::string(extension)},
		{dj::TextureCubeSide::PositiveY, std::string("top") + std::string(extension)},
		{dj::TextureCubeSide::NegativeY, std::string("bottom") + std::string(extension)},
		{dj::TextureCubeSide::PositiveZ, std::string("front") + std::string(extension)},
		{dj::TextureCubeSide::NegativeZ, std::string("back") + std::string(extension)}
	} };

	std::optional<dj::TextureHandle> handle = texMgr.createCubeMapFromFile(sampling, path, suffixesMapping, false, false, true);
	if (!handle)
	{
		std::cerr << dj::Log::failPrefix() << "Could not load cubemap: " << path << std::endl;
		return false;
	}

	cubeTextures.push_back(*handle);

	return true;
}

void configureRasterization()
{
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

void loadObjects(dj::MeshData& meshData, std::vector<dj::ObjectPtr>& objects)
{
	auto loadObject = [&meshData, &objects](auto& vertices, auto& indices, const char* objName) {
		dj::Mesh mesh;
		mesh.addVertices(vertices, std::size(vertices));
		mesh.addIndices(indices, std::size(indices));
		mesh.computeBoundingBox();

		if (!mesh.computeTangents())
		{
			std::cerr << "Tangents computation failed\n";
		}

		dj::MeshAlignment meshIndices = meshData.addMesh(mesh);
		dj::ObjectPtr object(std::make_shared<dj::Object>(meshIndices, mesh.getBoundingBox(), objName));
		objects.push_back(object);
	};

	loadObject(dj::triangleVerts, dj::triangleIndices, "Triangle");
	loadObject(dj::triWallCubeVerts, dj::triWallCubeIndices, "TriWallCube");
	loadObject(dj::planeVerts, dj::planeIndices, "Plane");
	loadObject(dj::boxVerts, dj::boxIndices, "Cube");
}

void setDefaultMaterials(std::vector<dj::ObjectPtr>& objects, dj::MaterialPtr mat)
{
	for (const dj::ObjectPtr& obj : objects)
	{
		obj->setDefaultMaterial(mat);
	}
}

void setMaterials(std::vector<dj::ObjectInstancePtr>& objectInstances, const std::vector<dj::MaterialPtr>& materials)
{
	assert(objectInstances.size() >= 3u);
	assert(materials.size() >= 2u);
	objectInstances[2]->setMaterial(materials[1]);
}

void createObjectInstances(const std::vector<dj::ObjectPtr>& objects, std::vector<dj::ObjectInstancePtr>& objectInstances)
{
	assert(objects.size() >= 4);
	if (objects.size() >= 4)
	{
		dj::ObjectPtrConst triangleObj = objects[0];
		dj::ObjectPtrConst triWallCubeObj = objects[1];
		dj::ObjectPtrConst planeObj = objects[2];
		dj::ObjectPtrConst cubeObj = objects[3];

		objectInstances.push_back(std::make_shared<dj::ObjectInstance>(triangleObj));
		objectInstances.push_back(std::make_shared<dj::ObjectInstance>(triWallCubeObj));
		objectInstances.push_back(std::make_shared<dj::ObjectInstance>(planeObj));
		objectInstances.push_back(std::make_shared<dj::ObjectInstance>(cubeObj));
		objectInstances.push_back(std::make_shared<dj::ObjectInstance>(triangleObj));
		objectInstances.push_back(std::make_shared<dj::ObjectInstance>(cubeObj));
		objectInstances.push_back(std::make_shared<dj::ObjectInstance>(cubeObj));
	}
}

void loadLights(std::vector<dj::LightPtr>& lights)
{
	dj::LightPtr light1(std::make_shared<dj::Light>(dj::Light::Type::Point, true));
	dj::LightPtr light2(std::make_shared<dj::Light>(dj::Light::Type::Point, true));
	dj::LightPtr light3(std::make_shared<dj::Light>(dj::Light::Type::Spot, true));
	dj::LightPtr light4(std::make_shared<dj::Light>(dj::Light::Type::Spot, true));

	light1->set({ 0.8f, 0.8f, 1.0f }, 100.0f, 20.0f);	// 1.0f
	light1->setPosition(-2.0f, 5.0f, 0.0f);

	light2->set({ 1.0f, 0.8f, 0.8f }, 100.0f, 40.0f); // 3.0f
	light2->setPosition(6.0f, 3.0f, -1.0f);

	light3->set({ 1.0f, 0.3f, 0.3f }, 100.0f, 100.0f);	// 100.0f
	light3->setPosition(-1.0f, 2.0f, -2.0f);
	light3->setDirection(-40.0f, 0.0f);
	light3->setSpotAngles(60.0f, 55.0f);

	light4->set({ 0.3f, 0.3f, 1.0f }, 200.0f, 30.0f);	// 200.0f
	light4->setPosition(-1.0f, 1.0f, -1.0f);
	light4->setDirection(-40.0f, 180.0f);
	light4->setSpotAngles(60.0f, 1.0f);

	lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);
	lights.push_back(light4);
}

bool createShadows(dj::TextureManager& texMgr, 
					const std::vector<dj::LightPtr>& lights, 
					std::vector<dj::LightFramebufferBinding>& spotFBOs,
					std::vector<dj::LightFramebufferBinding>& pointFBOs,
					const dj::QualitySettings<4u>& quality)
{
	const unsigned int shadow2DRes = quality.getActive<dj::EngineQuality::Shadow2DResolution>();
	const unsigned int shadowCubeRes = quality.getActive<dj::EngineQuality::ShadowCubeResolution>();
	bool ok = true;

	for (dj::LightPtr light : lights)
	{
		if (light->getType() == dj::Light::Type::Spot)
		{
			std::cout << "Creating Framebuffer for 2D Shadow\n";

			dj::TextureDesc desc{};
			desc.glType = dj::TextureType::Texture2D;
			desc.resolution.width = shadow2DRes;
			desc.resolution.height = shadow2DRes;
			desc.format.inGPUColorFormat = dj::ColorFormatInDevice::Depth;
			desc.format.sourceColorFormat = dj::ColorFormatInSource::Depth;
			desc.format.sourceValueType = dj::PixelDataTypeInSource::Float;
			desc.sampling.minFilter = dj::TextureFilteringMin::Nearest;
			desc.sampling.magFilter = dj::TextureFilteringMag::Nearest;
			desc.sampling.wrapS = dj::TextureWrapping::ClampToBorder;
			desc.sampling.wrapT = dj::TextureWrapping::ClampToBorder;
			desc.mipmaps = false;

			std::optional<dj::TextureHandle> handle = texMgr.createEmptyTexture(desc);
			if (!handle)
			{
				std::cerr << dj::Log::failPrefix() << "Could not create texture for Spotlight\n";
				return false;
			}

			texMgr.setBorderColor(*handle, { 1.0f, 1.0f, 1.0f, 1.0f });

			dj::FramebufferPtr fbo = std::make_shared<dj::Framebuffer>();
			fbo->bind();
			fbo->assignTextureAttachment(texMgr, *handle, GL_DEPTH_ATTACHMENT);
			fbo->nullifyData();

			ok &= verifyFramebufferStatus(fbo->getFramebufferStatus());
			fbo->unbind();

			spotFBOs.push_back({light, fbo});
		}
		else if (light->getType() == dj::Light::Type::Point)
		{
			std::cout << "Creating Framebuffer for Cubemap Shadow\n";

			const static GLenum sides[6] = {
				GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			};

			dj::TextureDesc desc{};
			desc.glType = dj::TextureType::TextureCube;
			desc.resolution.width = shadowCubeRes;
			desc.resolution.height = shadowCubeRes;
			desc.format.inGPUColorFormat = dj::ColorFormatInDevice::Depth;
			desc.format.sourceColorFormat = dj::ColorFormatInSource::Depth;
			desc.format.sourceValueType = dj::PixelDataTypeInSource::Float;
			desc.sampling.minFilter = dj::TextureFilteringMin::Linear;
			desc.sampling.magFilter = dj::TextureFilteringMag::Linear;
			desc.sampling.wrapS = dj::TextureWrapping::ClampToEdge;
			desc.sampling.wrapT = dj::TextureWrapping::ClampToEdge;
			desc.sampling.wrapR = dj::TextureWrapping::ClampToEdge;
			desc.mipmaps = false;

			std::optional<dj::TextureHandle> handle = texMgr.createEmptyTexture(desc);
			if (!handle)
			{
				std::cerr << dj::Log::failPrefix() << "Could not create texture for Pointlight\n";
				return false;
			}

			// Create Framebuffer (Cube)
			dj::FramebufferPtr cubeDepthFBO = std::make_shared<dj::Framebuffer>();
			cubeDepthFBO->bind();
			cubeDepthFBO->assignTextureAttachment(texMgr, *handle, GL_DEPTH_ATTACHMENT);
			cubeDepthFBO->nullifyData();
			ok &= verifyFramebufferStatus(cubeDepthFBO->getFramebufferStatus());
			cubeDepthFBO->unbind();

			pointFBOs.push_back({light, cubeDepthFBO});
		}
	}

	return ok;
}

bool createMaterials(std::vector<dj::MaterialPtr>& materials,
	const std::map<dj::EngineProgramID, dj::ProgramPtr>& enginePrograms,
	const std::vector<dj::TextureHandle>& texturesPBR,
	const std::vector<dj::TextureHandle>& texturesCube)
{

	assert(texturesPBR.size() >= 8u && "Too few textures in texturesPBR");
	assert(texturesCube.size() >= 1u && "Too few textures in texturesCube");
	try
	{
		dj::ProgramPtr prePBRProgram = enginePrograms.at(dj::EngineProgramID::prePBR);
		dj::ProgramPtr postprocessProgram = enginePrograms.at(dj::EngineProgramID::postProcessing);
		dj::ProgramPtr depthProgram = enginePrograms.at(dj::EngineProgramID::depth);
		dj::ProgramPtr skyboxProgram = enginePrograms.at(dj::EngineProgramID::skybox);
		
		dj::MaterialPtr material = std::make_shared<dj::Material>();
		dj::MaterialPtr material2 = std::make_shared<dj::Material>();
		dj::MaterialPtr materialSkybox = std::make_shared<dj::Material>();
		dj::MaterialPtr materialDebugCube = std::make_shared<dj::Material>();
		materials.push_back(material);
		materials.push_back(material2);
		materials.push_back(materialSkybox);
		materials.push_back(materialDebugCube);

		material->setProgram(prePBRProgram);
		material2->setProgram(prePBRProgram);
		material->addTexture(texturesPBR.at(0), "u_material.albedo");
		material->addTexture(texturesPBR.at(1), "u_material.roughness");
		material->addTexture(texturesPBR.at(2), "u_material.metallic");
		material->addTexture(texturesPBR.at(3), "u_material.normal");
		material2->addTexture(texturesPBR.at(4), "u_material.albedo");
		material2->addTexture(texturesPBR.at(5), "u_material.roughness");
		material2->addTexture(texturesPBR.at(6), "u_material.metallic");
		material2->addTexture(texturesPBR.at(7), "u_material.normal");
		material->addTexture(texturesCube.at(0), "u_skybox");
		material2->addTexture(texturesCube.at(0), "u_skybox");
		
		materialSkybox->setProgram(skyboxProgram);
		materialSkybox->addTexture(texturesCube.at(0), "u_skybox");

		materialDebugCube->setProgram(skyboxProgram);
	}
	catch (const std::out_of_range& ex)
	{
		std::cerr << dj::Log::failPrefix() << __FUNCTION__ << "() Out of Range: " << ex.what() << std::endl;
		return false;
	}

	return true;
}

void maualObjectsPreTransformations(std::vector<dj::ObjectInstancePtr>& instances)
{
	assert(instances.size() >= 7);
	instances[0]->setTranslation(1.0f, 2.0f, -10.0f);
	instances[0]->setScale(10.0f, 5.0f, 1.0f);

	instances[1]->setTranslation(-0.5f, 0.5f, -4.0f);
	instances[1]->setScale(1.0f, 1.0f, 1.0f);

	instances[2]->setTranslation(0.0f, 0.0f, -5.0f);
	instances[2]->setScale(20.0f, 1.0f, 20.0f);

	instances[3]->setTranslation(3.0f, 1.0f, -8.0f);
	instances[3]->setScale(0.2f, 0.2f, 0.2f);

	instances[4]->setTranslation(5.0f, 2.0f, 5.0f);
	instances[4]->setScale(10.0f, 5.0f, 1.0f);
	glm::mat4 obj4Rot = glm::rotate(glm::mat4(1.0f), glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	instances[4]->setRotation(obj4Rot);

	instances[5]->setTranslation(-7.0f, -0.7f, -9.0f);
	glm::mat4 obj5Rot = glm::rotate(glm::mat4(1.0f), glm::radians(33.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	instances[5]->setRotation(obj5Rot);

	instances[6]->setTranslation(-3.0f, 3.0f, 10.0f);
	instances[6]->setScale(2.0f, 2.0f, 1.75f);
}

void manualObjectsTransformations(std::vector<dj::ObjectInstancePtr>& instances, const dj::TimeDrivenMovement &tdm)
{
	assert(instances.size() > 1);
	static dj::LinearUpdate obj1RotY(tdm, 0.0f, glm::radians(45.0f));

	instances[1]->setRotation(glm::rotate(glm::mat4(1.0f), obj1RotY.getVal(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void updateCamera(GLFWwindow* window, dj::Camera& camera, const dj::TimeDrivenMovement &tdm)
{
	const float diffS = 1000.0f / tdm.getFrameDiffMs().count();
	// Per second
	const float step = static_cast<float>(3.5f / diffS);
	const float stepSideDeg = static_cast<float>(80.0f / diffS);
	const float stepVDeg = static_cast<float>(60.0f / diffS);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.fly(glm::vec3(0.0f, 0.0f, -step));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.fly(glm::vec3(0.0f, 0.0f, step));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.fly(glm::vec3(step, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.fly(glm::vec3(-step, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.lookSide(-stepSideDeg);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.lookSide(stepSideDeg);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		camera.lookVerticaly(stepVDeg);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		camera.lookVerticaly(-stepVDeg);
	}

	camera.updateView();
}

void uniformLights(dj::ProgramPtr program, const std::vector<dj::LightPtr>& lights)
{
	for (unsigned int i = 0; i < lights.size(); ++i)
	{
		const std::string lightName = std::string("u_light[") + std::to_string(i) + std::string("].");
		std::string posName = lightName + std::string("pos");
		std::string colorName = lightName + std::string("color");
		std::string rangeName = lightName + std::string("range");
		std::string typeName = lightName + std::string("type");
		std::string dirName = lightName + std::string("dir");
		std::string spotExtCosName = lightName + std::string("spotExtCos");
		std::string spotIntCosName = lightName + std::string("spotIntCos");
		std::string shadowActiveName = lightName + std::string("shadowActive");

		glUniform3fv(program->getUniformLocation(posName.c_str()), 1, glm::value_ptr(lights[i]->getPosition()));
		glUniform3fv(program->getUniformLocation(colorName.c_str()), 1, glm::value_ptr(lights[i]->getColor() * lights[i]->getIntensity()));
		glUniform1f(program->getUniformLocation(rangeName.c_str()), lights[i]->getRange());
		glUniform1ui(program->getUniformLocation(typeName.c_str()), static_cast<GLuint>(lights[i]->getType()));
		glUniform1i(program->getUniformLocation(shadowActiveName.c_str()), (lights[i]->isShadowActive() ? 1 : 0));
		
		if (lights[i]->getType() != dj::Light::Type::Point)
		{
			glUniform3fv(program->getUniformLocation(dirName.c_str()), 1, glm::value_ptr(lights[i]->getDirectionVector()));

			if (lights[i]->getType() == dj::Light::Type::Spot)
			{
				glUniform1f(program->getUniformLocation(spotExtCosName.c_str()), glm::cos(glm::radians(lights[i]->getSpotlightAngle() / 2.0f)));
				glUniform1f(program->getUniformLocation(spotIntCosName.c_str()), glm::cos(glm::radians(lights[i]->getSpotlightInternalAngle() / 2.0f)));
			}
		}
	}

	glUniform1ui(program->getUniformLocation("u_lightsCount"), static_cast<GLuint>(lights.size()));
}

bool checkFramebufferStatus(GLenum framebufferFlag)
{
	GLenum status = glCheckFramebufferStatus(framebufferFlag);
	return verifyFramebufferStatus(status);
}

bool verifyFramebufferStatus(GLenum status)
{
	dj::Log log;
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::string errorMsg = std::string("Could not create Framebuffer. Error Code: ");
		std::string errorCode;
		switch (status)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: errorCode = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: errorCode = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: errorCode = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: errorCode = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
		case GL_FRAMEBUFFER_UNSUPPORTED: errorCode = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: errorCode = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: errorCode = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
		case GL_FRAMEBUFFER_UNDEFINED: errorCode = "GL_FRAMEBUFFER_UNDEFINED"; break;
		case GL_INVALID_ENUM: errorCode = "GL_INVALID_ENUM"; break;
		default: errorCode = std::to_string(status);
		}
		errorMsg += errorCode;
		log.ok = false;
		log.print(errorMsg.c_str());

		return false;
	}

	log.ok = true;
	log.print("Succesfully created Framebuffer");
	return true;
}

void reportFPSCallback(unsigned int framesCount, std::chrono::milliseconds period)
{
	float fps = static_cast<float>(framesCount) / (static_cast<float>(period.count()) / 1000.0f);
	std::cout << "FPS: " << static_cast<unsigned int>(fps) << std::endl;
}