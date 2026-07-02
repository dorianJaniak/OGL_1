#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "MainWindow.h"
#include "Logging/Logger.h"
#include "Logging/Log.h"
#include "QualitySettings.h"

#include "RenderNodes/RenderShadedWorldNode.h"
#include "RenderNodes/RenderDepthWorldNode.h"
#include "RenderNodes/RenderSkyboxWorldNode.h"
#include "RenderNodes/RenderParticlesNode.h"
#include "TextureManager.h"
#include "FramebufferManager.h"
#include "LightFramebufferBinding.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Object.h"
#include "Material.h"
#include "Shader.h"
#include "Program.h"
#include "Camera.h"
#include "CameraCube.h"
#include "Light.h"
#include "Framebuffer.h"
#include "ParticleSystem.h"
#include "TGLTFLoader.h"
#include "DefinitionsGL.h"
#include "EngineKeywords.h"
#include "Predefinitions/PredefinedShaders.h"
#include "Predefinitions/PredefinedMeshes.h"
#include "Basic3DEnviro/Basic3DEnviro.h"
#include "Basic3DEnviro/DebugTweaks.h"
#include "Time/TimeDrivenMovement.h"
#include "Time/PerFrameUpdates.h"
#include "Time/OccurrenceFrequency.h"
#include "Enums/LogCodes.h"
#include "Enums/Converters.h"

#include <chrono>
#include <vector>
#include <map>
#include <memory>
#include <array>

using namespace dj;

// Helpers - Loaders
bool loadEnginePrograms(std::map<EngineProgramID, ProgramPtr>& enginePrograms, std::shared_ptr<ILogger> logger);
bool setupEnginePrograms(std::map<EngineProgramID, ProgramPtr>& enginePrograms, std::shared_ptr<ILogger> logger);

bool loadTextures(TextureManager& texMgr, std::vector<TextureHandle>& pbrTextures, std::vector<TextureHandle>& skyboxTextures, std::shared_ptr<ILogger> logger);
std::optional<TextureHandle> loadTexture(TextureManager& texMgr, const char* path, std::shared_ptr<ILogger> logger);
bool loadTexturesPBR(TextureManager& texMgr, std::vector<TextureHandle>& pbrTextures, const char* path, const char* extension, std::shared_ptr<ILogger> logger);
bool loadTextureCube(TextureManager& texMgr, std::vector<TextureHandle>& cubeTextures, const char* path, const char* extension, std::shared_ptr<ILogger> logger);

void configureRasterization();
void loadObjects(MeshData& meshData, std::vector<ObjectPtr>& objects, std::shared_ptr<ILogger> logger);
void setDefaultMaterials(std::vector<ObjectPtr>& objects, MaterialPtr mat);
void setMaterials(std::vector<ObjectInstancePtr>& objectInstances, const std::vector<MaterialPtr>& mat);
void createObjectInstances(const std::vector<ObjectPtr>& objects, std::vector<ObjectInstancePtr>& objectInstances);
void loadLights(std::vector<LightPtr>& lights);

// Helpers - Relations
bool createShadows(
	TextureManager& texMgr,
	FramebufferManager& fboMgr,
	const std::vector<LightPtr>& lights,
	std::vector<LightFramebufferBinding>& spotFBOs,
	std::vector<LightFramebufferBinding>& pointFBOs,
	const QualitySettings<4u>& quality,
	std::shared_ptr<ILogger> logger);
bool createMaterials(
	std::vector<MaterialPtr>& materials,
	const std::map<EngineProgramID, ProgramPtr>& enginePrograms,
	const std::vector<TextureHandle>& texturesPBR,
	const std::vector<TextureHandle>& texturesCube,
	const std::vector<TextureHandle>& texturesAlpha,
	std::shared_ptr<ILogger> logger);
std::optional<FramebufferHandleSet> createMainFramebuffer(
	FramebufferManager& fboMgr,
	TextureManager& texMgr,
	const MainWindow& mw,
	QualitySettings<4u>& quality);

// Helpers - Transformations
void maualObjectsPreTransformations(std::vector<ObjectInstancePtr>& instances);
void manualObjectsTransformations(std::vector<ObjectInstancePtr>& instances, const TimeDrivenMovement& tdm);
void updateCamera(GLFWwindow* window, Camera& camera, const TimeDrivenMovement& tdm);
glm::mat3 calcNormalMatrixToViewSpace(const glm::mat4& view, const glm::mat4& model);

// Helpers - Render
void renderWorldForDepthTest(std::vector<ObjectInstancePtr>& instances, ProgramPtr program);
void renderWorldSingleProgram(std::vector<ObjectInstancePtr>& instances, ProgramPtr program, Camera& camera);

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
*/

/*! \page renderingLoopOrder Rendering Loop Stages
*   STAGE 1 :::: Window and Context Init \n
*   STAGE 2 :::: Shader Programs Creation \n
*   STAGE 3 :::: Objects Loading \n
*   STAGE 4 :::: FBO Creation \n
*   STAGE 5 :::: FBO for shadow maps \n
*   STAGE 6 :::: Vertex related Buffers Creation \n
*   STAGE 6.1 :::: VAO, VBOs for particles (cpu approach) \n
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
*   STAGE 9.3.3.2 :::: Render Particles Node \n
*   STAGE 9.3.3.3 :::: Render Skybox Node \n
*   STAGE 9.3.3.4 :::: Render Debug Edges \n
*   STAGE 9.3.4 :::: Draw Main FBO \n
*/

class FPSReporter
{
	std::shared_ptr<ILogger> logger;

public:
	FPSReporter(std::shared_ptr<ILogger> logger)
		: logger(logger)
	{
	}

	void reportFPSCallback(unsigned int framesCount, std::chrono::milliseconds period)
	{
		if (logger)
		{
			float fps = static_cast<float>(framesCount) / (static_cast<float>(period.count()) / 1000.0f);
			logger->log(Log(LogLevel::Info, 0u, "", "FPS: {}", static_cast<unsigned int>(fps)));
		}
	}
};

void basicSetupQualitySettings(QualitySettings<4u>& quality)
{
	quality.set<EngineQuality::Shadow2DResolution>(std::array{ 2048u, 1024u, 512u, 256u });
	quality.set<EngineQuality::ShadowCubeResolution>(std::array{ 1024u, 1024u, 512u, 256u });
	quality.set<EngineQuality::MainFBHighPrecision>(std::array{ true, true, true, false });
	quality.setActiveLevel(1u);
}

int main()
{
	std::shared_ptr<Logger> logger = std::make_shared<Logger>();

	QualitySettings<4u> quality;
	basicSetupQualitySettings(quality);

	std::shared_ptr<DebugTweaks> dt = std::make_shared<DebugTweaks>();
	TimeDrivenMovement tdm;

	TextureManager texMgr(logger);
	FramebufferManager fboMgr(logger);

	std::vector<ObjectPtr> objects;
	std::vector<ObjectInstancePtr> objectInstances;
	std::vector<MaterialPtr> materials;
	std::map<EngineProgramID, ProgramPtr> enginePrograms;
	std::vector<LightPtr> lights;
	std::vector<LightFramebufferBinding> spotFBOs;
	std::vector<LightFramebufferBinding> pointFBOs;

	std::optional<TextureHandle> cameraIDs[3];		// Temporary - active cameras
	CameraPtr camera = std::make_shared<Camera>();

	//TGLTFLoader tgltfLoader;
	//return tgltfLoader.load("res/gltfs/twoCubes_altC_rot_texture_customProp/untitled.gltf");

	// STAGE 1 :::: Window and Context Init
	MainWindow mw(logger, 1200, 800, (1200.0f / 800.0f));
	if (!mw.initGLFW(3, 3, "Prosty silnik"))
	{
		return -1;
	}
	mw.addCallback(dt);

	// STAGE 2 :::: Shader Programs Creation
	// Loads, compiles and links shaders and generates Program ID
	if (!loadEnginePrograms(enginePrograms, logger))
	{
		mw.terminate();
		return -1;
	}

	// Setups Texture Units
	if (!setupEnginePrograms(enginePrograms, logger))
	{
		mw.terminate();
		return -1;
	}

	// STAGE 3 :::: Objects Loading
	// Load objects and init their transformations
	MeshData meshData;
	loadObjects(meshData, objects, logger);
	createObjectInstances(objects, objectInstances);
	loadLights(lights);

	// STAGE 4 :::: Cameras Configuration
	camera->setPerspective(30.0f, mw.getAspectRatio(), 0.1f, 300.0f);

	// STAGE 5 :::: FBO Creation
	std::optional<FramebufferHandleSet> mainFBOHandles = createMainFramebuffer(fboMgr, texMgr, mw, quality);
	if (!mainFBOHandles)
	{
		if (logger)
		{
			logger->log(Log(LogLevel::Critical, "", "Could not create main FBO and its Textures"));
		}
		mw.terminate();
		return -1;
	}

	GLuint svbo, svao;
	glGenBuffers(1, &svbo);
	glBindBuffer(GL_ARRAY_BUFFER, svbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreenVerts), fullscreenVerts, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &svao);
	glBindVertexArray(svao);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);				// position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));	// UV
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// STAGE 6 :::: FBO for shadow maps
	if (!createShadows(texMgr, fboMgr, lights, spotFBOs, pointFBOs, quality, logger))
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, MeshData::vertexComponentsCount * sizeof(float), (void*)NULL);					// position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, MeshData::vertexComponentsCount * sizeof(float), (void*)(3 * sizeof(float)));	// normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, MeshData::vertexComponentsCount * sizeof(float), (void*)(6 * sizeof(float)));	// tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, MeshData::vertexComponentsCount * sizeof(float), (void*)(9 * sizeof(float)));	// bitangent
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, MeshData::vertexComponentsCount * sizeof(float), (void*)(12 * sizeof(float)));	// UV
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.getIndicesDataSize(), &meshData.getAllIndices()[0], GL_STATIC_DRAW);

	// STAGE 6.1 :::: VAO, VBOs for particles (cpu approach)
	// This part is only for testing - to check how instancing works
	std::shared_ptr<ParticleSystem> particles = std::make_shared<ParticleSystem>(tdm, 2000);
	if (!particles->initBuffers())
	{
		logger->log(Log(LogLevel::Error, 0u, "", "Could not initialize particle system with {} elements", particles->getCount()));
		mw.terminate();
		return -1;
	}
	particles->setBounds(20.0f, 10.0f, 20.0f);
	particles->setPosition({ 0.0f, 5.0f, 0.0f });
	particles->setVelocity(glm::vec3(0.0f, -8.0f, 0.0f));
	particles->setParticleScale(glm::vec2(0.05f, 0.1f));
	particles->setOpacity(0.3f);
	particles->randomizeParticles();

	// STAGE 7 :::: Materials Creation
	std::vector<TextureHandle> pbrTextures;
	std::vector<TextureHandle> skyboxTextures;
	std::vector<TextureHandle> alphaTextures;

	if (!loadTextures(texMgr, pbrTextures, skyboxTextures, logger))
	{
		mw.terminate();
		return -1;
	}

	{
		std::optional<TextureHandle> raindropTex = loadTexture(texMgr, "res/textures/alpha/raindrop.png", logger);
		if (!raindropTex)
		{
			mw.terminate();
			return -1;
		}
		alphaTextures.push_back(*raindropTex);
	}

	// Materials configuration
	if (!createMaterials(materials, enginePrograms, pbrTextures, skyboxTextures, alphaTextures, logger))
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
		//cameraIDs[i + 1u] = spotFBOs[i].fbo->getTextureAttachment(GL_DEPTH_ATTACHMENT);
		cameraIDs[i + 1u] = spotFBOs[i].fbo.texHandles[0u];
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
	TimeFlow<std::chrono::seconds> tfLightIntensity(std::chrono::seconds(15));
	WahwahSinUpdate light0Move(tdm, lights[0]->getPosition().x, 10.0f, 30.0f);
	LinearUpdate light0Range(tdm, 1.0f, 3.0f);
	WahwahSinUpdate light2RotX(tdm, -40.0f, 30.0f, 90.0f);
	LinearUpdate light3RotY(tdm, 180.0f, 120.0f);

	// STAGE XX :::: Render Nodes creation
	fboMgr.printFramebuffers();
	std::vector<ObjectInstancePtr> skyboxObjectInstances;
	{
		ObjectInstancePtr skyboxCubeObjInst = std::make_shared<ObjectInstance>(objects.at(3u), true);
		skyboxCubeObjInst->setMaterial(materials.at(2u));
		skyboxObjectInstances.push_back(skyboxCubeObjInst);
	}

	// VARIANT 2
	std::shared_ptr<RenderShadedWorldNode> worldNode;
	std::shared_ptr<RenderSkyboxWorldNode> skyboxNode;
	std::shared_ptr<RenderSkyboxWorldNode> cubeMapDebugNode;
	std::shared_ptr<RenderParticlesNode> particlesNode;

	std::vector<ObjectInstancePtr> cubeMapDebugObjectInstances;
	{
		ObjectInstancePtr cubeObjInst = std::make_shared<ObjectInstance>(objects.at(3u), true);
		cubeObjInst->setMaterial(materials.at(3u));
		cubeMapDebugObjectInstances.push_back(cubeObjInst);
	}

	FPSReporter fpsReporter(logger);
	OccurrenceFrequency fps(std::chrono::milliseconds(5000u));
	//fps.setReportCallback(fpsReporter.reportFPSCallback);
	fps.setReportCallback(std::bind(&FPSReporter::reportFPSCallback, &fpsReporter, std::placeholders::_1, std::placeholders::_2));
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
			if (logger)
			{
				logger->log(Log(LogLevel::Debug, 0u, "", "Light intensity: {}", lights[0]->getIntensity()));
			}
		}

		lights[0]->setPosition(light0Move.getVal(), lights[0]->getPosition().y, lights[0]->getPosition().z);
		lights[0]->setRange(light0Range.getVal());
		
		lights[2]->setDirection(light2RotX.getVal(), -60.0f);
		lights[3]->setDirection(-10.0f, light3RotY.getVal());

		updateCamera(mw.getWindow(), *camera, tdm);
		manualObjectsTransformations(objectInstances, tdm);
		if (particles)
		{
			particles->updatePositions();
		}

		// Stage 9.3 :::: Rendering
		glBindVertexArray(vao);

		// Stage 9.3.1 :::: Render Shadow Maps (Depth Tests)
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		enginePrograms[EngineProgramID::depth]->use();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// Stage 9.3.1.1 :::: Render Spot Shadow Maps (Depth Tests)
		for (auto& shadow : spotFBOs)
		{
			// Pointer to active program
			ProgramPtr activeProgram = enginePrograms.at(EngineProgramID::depth);

			// Copy lights data to camera
			// The same as ShadowFramebuffer::configureCamera
			Camera cam;
			cam.setPosition(shadow.light->getPosition());
			cam.setRotation(glm::vec3(shadow.light->tmpXPitch, shadow.light->tmpYYaw, 0.0f));
			cam.setPerspective(shadow.light->getSpotlightAngle() + 30.0f, 1.0f, shadow.light->getShadowNearPlane(), shadow.light->getShadowFarPlane());

			// Bind Framebuffer and Setup Rendering
			fboMgr.check(shadow.fbo.handle, [](const Framebuffer& f) {
				f.bind();
				glViewport(0, 0, f.getWidth(), f.getHeight());
				return true;
			});

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
		enginePrograms.at(EngineProgramID::depthCube)->use();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		for (auto& shadow : pointFBOs)
		{
			// Pointer to active program
			ProgramPtr activeProgram = enginePrograms.at(EngineProgramID::depthCube);

			// Copy lights data to camera
			CameraCube cameraCube;
			cameraCube.setPlanes(1.0f, 25.0f);
			cameraCube.setPosition(shadow.light->getPosition());

			glUniform1f(activeProgram->getUniformLocation("u_far"), cameraCube.getFar());
			glUniform3fv(activeProgram->getUniformLocation("u_lightPos"), 1, glm::value_ptr(cameraCube.getPosition()));

			for (unsigned int i = 0u; i < 6u; ++i)
			{
				std::string uniformName = std::string("u_cameraMatrices[") + std::to_string(i) + std::string("]");
				glUniformMatrix4fv(activeProgram->getUniformLocation(uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(cameraCube.getVPMatrix(i)));
			}

			// Bind Framebuffer and Setup Rendering
			fboMgr.check(shadow.fbo.handle, [](const Framebuffer& f) {
				f.bind();
				glViewport(0, 0, f.getWidth(), f.getHeight());
				return true;
			});
			glClear(GL_DEPTH_BUFFER_BIT);
			
			// Render vertices for Shadows (duplicated)
			renderWorldForDepthTest(objectInstances, activeProgram);
		}

		// Stage 9.3.2 :::: Render Debug Cubemap
		if (dt->getActiveCameraIndex() == 3u)
		{
			assert(pointFBOs.size() >= 2u && "Expected 2 Point lights in pointFBOs vector");
			TextureHandle shTex = pointFBOs.at(1).fbo.texHandles[0u];
			if (texMgr.exists(shTex))
			{
				cubeMapDebugNode->addTexture("u_skybox", shTex);
			}
			cubeMapDebugNode->run();

			glDepthMask(GL_TRUE);
			glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		// Stage 9.3.3 :::: Render Scene
		else
		{
			static unsigned int lastWidth = 0u;
			static unsigned int lastHeight = 0u;

			if (lastWidth != mw.getWidth() || lastHeight != mw.getHeight())
			{
				if (logger)
				{
					logger->log(Log(LogLevel::Info, 0u, "", "Resizing main framebuffer from: {}x{} to {}x{}", lastWidth, lastHeight, mw.getWidth(), mw.getHeight()));
				}

				lastWidth = mw.getWidth();
				lastHeight = mw.getHeight();

				// Replace main framebuffer handles (creates new framebuffer, texture and renderbuffer attachments)
				mainFBOHandles = createMainFramebuffer(fboMgr, texMgr, mw, quality);

				worldNode = std::make_shared<RenderShadedWorldNode>(texMgr, fboMgr, mainFBOHandles->handle, camera, ebo, objectInstances, spotFBOs, pointFBOs, lights, std::string("Main Color - World"));
				worldNode->setConfiguration(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, true, true, GL_NONE);
				skyboxNode = std::make_shared<RenderSkyboxWorldNode>(texMgr, fboMgr, mainFBOHandles->handle, camera, ebo, skyboxObjectInstances, "Main Color - Skybox");
				skyboxNode->setConfiguration(GL_NONE, true, false, GL_NONE);

				assert(materials.size() >= 5 && "There is no material for particles");
				particlesNode = std::make_shared<RenderParticlesNode>(texMgr, fboMgr, mainFBOHandles->handle, camera, particles, materials[4], "Particles Node");
				particlesNode->setConfiguration(GL_NONE, true, false, GL_NONE);

				cubeMapDebugNode = std::make_shared<RenderSkyboxWorldNode>(texMgr, fboMgr, mainFBOHandles->handle, camera, ebo, cubeMapDebugObjectInstances, "Debug CubeMap");
				cubeMapDebugNode->setConfiguration(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, false, false, GL_FRONT);

				// Needs to be before texMgr.forceDeleteUnused(), because this is last reference to fbo
				assert(mainFBOHandles->texHandles.size() > 0 && "Main FBO creation failed - it should have at least 1 texture attached");
				cameraIDs[0] = mainFBOHandles->texHandles[0];

				logger->log(Log(LogLevel::Debug, 0u, "", "Deleted {} unused framebuffers. Left: {}", fboMgr.forceDeleteUnused(), fboMgr.getCount()));
				logger->log(Log(LogLevel::Debug, 0u, "", "Deleted {} unused textures. Left: {}", texMgr.forceDeleteUnused(), texMgr.getCount()));

				particles->randomizeParticles();
			}

			// Stage 9.3.3.1 :::: Render World Node
			std::shared_ptr<IRenderNode> worldNodeG = worldNode;
			worldNodeG->run();

			// Stage 9.3.3.3 :::: Render Skybox Node
			std::shared_ptr<IRenderNode> skyboxNodeG = skyboxNode;
			glDepthFunc(GL_LEQUAL);
			skyboxNodeG->run();

			// Stage 9.3.3.2 :::: Render Particles Node
			if (particlesNode && particles)
			{
				particles->bind();
				particles->updateTransformations();
				particlesNode->run();
			}

			// Stage 9.3.3.4 :::: Render Debug Edges
			glBindVertexArray(vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glDepthFunc(GL_LESS);
			if (dt->getDebugVertices())
			{
				assert(enginePrograms.size() >= 5);
				ProgramPtr programDebugMesh = enginePrograms.at(EngineProgramID::debugMesh);
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
		enginePrograms[EngineProgramID::postProcessing]->use();
		glBindVertexArray(svbo);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE11);

		if (cameraIDs[dt->getActiveCameraIndex()])
		{
			texMgr.bind(*cameraIDs[dt->getActiveCameraIndex()]);
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

void renderWorldForDepthTest(std::vector<ObjectInstancePtr>& instances, ProgramPtr activeProgram)
{
	for (ObjectInstancePtr& obj : instances)
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

void renderWorldSingleProgram(std::vector<ObjectInstancePtr> &instances, ProgramPtr program, Camera &camera)
{
	glUniformMatrix4fv(program->getUniformLocation("u_proj"), 1, GL_FALSE, glm::value_ptr(camera.getPerspectiveMatrix()));
	glUniformMatrix4fv(program->getUniformLocation("u_view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	
	for (ObjectInstancePtr& obj : instances)
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

bool loadEnginePrograms(std::map<EngineProgramID, ProgramPtr>& programs, std::shared_ptr<ILogger> logger)
{
	assert(logger && "Logger is nullptr");

	ProgramPtr program = std::make_shared<Program>(logger);
	ProgramPtr programFBO = std::make_shared<Program>(logger);
	ProgramPtr programDepthTest = std::make_shared<Program>(logger);
	ProgramPtr programSkybox = std::make_shared<Program>(logger);
	ProgramPtr programDebugMesh = std::make_shared<Program>(logger);
	ProgramPtr programDepthTestCube = std::make_shared<Program>(logger);
	ProgramPtr particles = std::make_shared<Program>(logger);

	if (!program->prepare(prePBR_vsSource, pbr_fsSource, std::string("PBR Shader")))
	{
		return false;
	}

	if (!programFBO->prepare(fullscreenFBO_vsSource, fullscreenFBO_fsSource, std::string("FBO Shader")))
	{
		return false;
	}

	if (!programDepthTest->prepare(depthTestMat_vsSource, depthTestMat_fsSource, std::string("Depth Test Shader")))
	{
		return false;
	}

	if (!programSkybox->prepare(skybox_vsSource, skybox_fsSource, std::string("Skybox Shader")))
	{
		return false;
	}

	if (!programDebugMesh->prepare(debugMesh_vsSource, debugMesh_fsSource, std::string("Debug Mesh Shader"), debugMesh_gsSource))
	{
		return false;
	}

	if (!programDepthTestCube->prepare(depthTestCube_vsSource, depthTestCube_fsSource, std::string("Depth Test Cube"), depthTestCube_gsSource))
	{
		return false;
	}

	if (!particles->prepare(particleSystem_vsSource, particleSystem_fsSource, std::string("Particles")))
	{
		return false;
	}

	programs.insert({ EngineProgramID::prePBR, program });
	programs.insert({ EngineProgramID::postProcessing, programFBO });
	programs.insert({ EngineProgramID::depth, programDepthTest });
	programs.insert({ EngineProgramID::skybox, programSkybox });
	programs.insert({ EngineProgramID::debugMesh, programDebugMesh });
	programs.insert({ EngineProgramID::depthCube, programDepthTestCube });
	programs.insert({ EngineProgramID::particles, particles });
	return true;
}

bool setupEnginePrograms(std::map<EngineProgramID, ProgramPtr>& enginePrograms, std::shared_ptr<ILogger> logger)
{
	unsigned int uniformsCount = 0u;
	unsigned int uniformsOkCount = 0u;

	auto registerAndCheck = [&uniformsCount, &uniformsOkCount, &logger](ProgramPtr& program, const std::string& uniformName)
		{
			++uniformsCount;
			if (program->registerUniform(uniformName))
			{
				++uniformsOkCount;
			}
			else
			{
				if (logger)
				{
					logger->log(Log(LogLevel::Error, 0u, __FUNCTION__,
						"Could not locate uniform: {} in program: {}", uniformName, program->getName()));
				}
			}
		};

	auto registerLightsUniforms = [&registerAndCheck](ProgramPtr& program, const unsigned int lightsCount, const char* postfix)
		{
			for (unsigned int i = 0; i < lightsCount; ++i)
			{
				std::string name = "u_light[" + std::to_string(i) + "]." + postfix;
				registerAndCheck(program, name);
			}
		};

	try
	{
		ProgramPtr prePBRProgram = enginePrograms.at(EngineProgramID::prePBR);
		ProgramPtr postprocessProgram = enginePrograms.at(EngineProgramID::postProcessing);
		ProgramPtr depthProgram = enginePrograms.at(EngineProgramID::depth);
		ProgramPtr depthCubeProgram = enginePrograms.at(EngineProgramID::depthCube);
		ProgramPtr skyboxProgram = enginePrograms.at(EngineProgramID::skybox);
		ProgramPtr debugMeshProgram = enginePrograms.at(EngineProgramID::debugMesh);
		ProgramPtr particlesProgram = enginePrograms.at(EngineProgramID::particles);

		// These uniforms needs to be after program use
		prePBRProgram->use();
		constexpr unsigned int lightsCount = 4u;
		prePBRProgram->addUniformLimit(Engine::UniformLimits::maxLightsCount, lightsCount);
		prePBRProgram->addUniformLimit(Engine::UniformLimits::maxPointLightsCount, 2u);
		prePBRProgram->addUniformLimit(Engine::UniformLimits::maxSpotLightsCount, 2u);
		registerAndCheck(prePBRProgram, "u_model");
		registerAndCheck(prePBRProgram, "u_mvp");
		registerAndCheck(prePBRProgram, "u_normalMat");
		registerAndCheck(prePBRProgram, "u_wCamPos");

		registerAndCheck(prePBRProgram, "u_material.albedo");
		registerAndCheck(prePBRProgram, "u_material.roughness");
		registerAndCheck(prePBRProgram, "u_material.metallic");
		registerAndCheck(prePBRProgram, "u_material.normal");
		registerAndCheck(prePBRProgram, "u_skybox");

		registerAndCheck(prePBRProgram, "u_cubeShadow[0]");
		registerAndCheck(prePBRProgram, "u_cubeShadow[1]");
		registerAndCheck(prePBRProgram, "u_shadow[0]");
		registerAndCheck(prePBRProgram, "u_shadow[1]");

		registerAndCheck(prePBRProgram, "u_lightsCount");
		registerAndCheck(prePBRProgram, "u_lightNear");
		registerAndCheck(prePBRProgram, "u_lightFar");
		registerAndCheck(prePBRProgram, "u_lightVP[0]");
		registerAndCheck(prePBRProgram, "u_lightVP[1]");

		registerLightsUniforms(prePBRProgram, lightsCount, "pos");
		registerLightsUniforms(prePBRProgram, lightsCount, "color");
		//registerLightsUniforms(prePBRProgram, lightsCount, "range");
		registerLightsUniforms(prePBRProgram, lightsCount, "type");
		registerLightsUniforms(prePBRProgram, lightsCount, "dir");
		registerLightsUniforms(prePBRProgram, lightsCount, "spotExtCos");
		registerLightsUniforms(prePBRProgram, lightsCount, "spotIntCos");
		registerLightsUniforms(prePBRProgram, lightsCount, "shadowActive");

		postprocessProgram->use();
		registerAndCheck(postprocessProgram, "frame");
		postprocessProgram->assignTextureUnit("frame", GL_TEXTURE11);

		skyboxProgram->use();
		registerAndCheck(skyboxProgram, "u_camRP");
		registerAndCheck(skyboxProgram, "u_skybox");
		//skyboxProgram->assignTextureUnit("u_skybox", GL_TEXTURE10;

		debugMeshProgram->use();
		registerAndCheck(debugMeshProgram, "u_model");
		registerAndCheck(debugMeshProgram, "u_view");
		registerAndCheck(debugMeshProgram, "u_normalMat");
		registerAndCheck(debugMeshProgram, "u_proj");
		registerAndCheck(debugMeshProgram, "u_axisLength");
		registerAndCheck(debugMeshProgram, "u_debugSelection");

		depthProgram->use();
		registerAndCheck(depthProgram, "u_camVP");
		registerAndCheck(depthProgram, "u_model");
		registerAndCheck(depthProgram, "u_near");
		registerAndCheck(depthProgram, "u_far");

		depthCubeProgram->use();
		registerAndCheck(depthCubeProgram, "u_model");
		registerAndCheck(depthCubeProgram, "u_cameraMatrices[0]");
		registerAndCheck(depthCubeProgram, "u_cameraMatrices[1]");
		registerAndCheck(depthCubeProgram, "u_cameraMatrices[2]");
		registerAndCheck(depthCubeProgram, "u_cameraMatrices[3]");
		registerAndCheck(depthCubeProgram, "u_cameraMatrices[4]");
		registerAndCheck(depthCubeProgram, "u_cameraMatrices[5]");
		registerAndCheck(depthCubeProgram, "u_far");
		registerAndCheck(depthCubeProgram, "u_lightPos");

		particlesProgram->use();
		registerAndCheck(particlesProgram, "u_scale");
		registerAndCheck(particlesProgram, "u_camVP");
		registerAndCheck(particlesProgram, "u_texture");
		registerAndCheck(particlesProgram, "u_opacity");

		if (uniformsCount != uniformsOkCount)
		{
			if (logger)
			{
				logger->log(Log(LogLevel::Warning, 0u, __FUNCTION__, "Could not locate all required uniforms in programs. Located {} out of {}", uniformsOkCount, uniformsCount));
			}
			return true;
		}
	}
	catch (const std::out_of_range& ex)
	{
		if (logger)
		{
			logger->log(Log(LogLevel::Critical, 0u, __FUNCTION__, "Out of range: {}", ex.what()));
		}

		return false;
	}

	return true;
}

bool loadTextures(TextureManager& texMgr, std::vector<TextureHandle>& pbrTextures, std::vector<TextureHandle>& skyboxTextures, std::shared_ptr<ILogger> logger)
{
	static const unsigned int& texsCount = dj_basicEnviro::pbrMaterialsCount;
	static const auto& texPaths = dj_basicEnviro::pbrMaterialPaths;
	static const auto& texExts = dj_basicEnviro::pbrMaterialFileExtensions;

	static const unsigned int& skyboxesCount = dj_basicEnviro::skyboxMaterialsCount;
	static const auto& skyboxPaths = dj_basicEnviro::skyboxMaterialPaths;
	static const auto& skyboxExts = dj_basicEnviro::skyboxMaterialFileExtensions;

	for (unsigned int i = 0u; i < texsCount; ++i)
	{
		if (!loadTexturesPBR(texMgr, pbrTextures, texPaths[i].data(), texExts[i].data(), logger))
		{
			return false;
		}
	}

	for (unsigned int i = 0u; i < skyboxesCount; ++i)
	{
		if (!loadTextureCube(texMgr, skyboxTextures, skyboxPaths[i].data(), skyboxExts[i].data(), logger))
		{
			return false;
		}
	}

	return true;
}

std::optional<TextureHandle> loadTexture(TextureManager& texMgr, const char* path, std::shared_ptr<ILogger> logger)
{
	static const TextureSamplingDesc& sampling = getTextureSamplingDescDefaultsFor2D();
	std::optional<TextureHandle> handle = texMgr.create2DFromFile(sampling, path, true, true, false);

	if (!handle)
	{
		logger->log(Log(LogLevel::Error, 0u, "", "Could not load texture: {}", path));
		return std::nullopt;
	}

	return handle;
}

bool loadTexturesPBR(TextureManager& texMgr, std::vector<TextureHandle>& pbrTextures, const char* path, const char* extension, std::shared_ptr<ILogger> logger)
{
	static const std::string normalSuffix = "normal-ogl";
	static const std::array<std::string, 4> suffixes = {
		"albedo",
		"roughness",
		"metallic",
		normalSuffix,
	};
	static const TextureSamplingDesc& sampling = getTextureSamplingDescDefaultsFor2D();

	for (const auto suffix : suffixes)
	{
		const std::string fullPath = path + suffix + extension;
		bool allowSRGB = (suffix != normalSuffix);
		std::optional<TextureHandle> handle = texMgr.create2DFromFile(sampling, fullPath.c_str(), true, true, allowSRGB);

		if (!handle)
		{
			logger->log(Log(LogLevel::Error, 0u, "", "Could not load texture: {}", fullPath));
			return false;
		}

		pbrTextures.push_back(*handle);
	}

	return true;
}

bool loadTextureCube(TextureManager& texMgr, std::vector<TextureHandle>& cubeTextures, const char* path, const char* extension, std::shared_ptr<ILogger> logger)
{
	static const TextureSamplingDesc& sampling = getTextureSamplingDescDefaultsForCube();
	const std::array<TextureManager::CubeSideMapping, 6u> suffixesMapping{ {
		{TextureCubeSide::PositiveX, std::string("right") + std::string(extension)},
		{TextureCubeSide::NegativeX, std::string("left") + std::string(extension)},
		{TextureCubeSide::PositiveY, std::string("top") + std::string(extension)},
		{TextureCubeSide::NegativeY, std::string("bottom") + std::string(extension)},
		{TextureCubeSide::PositiveZ, std::string("front") + std::string(extension)},
		{TextureCubeSide::NegativeZ, std::string("back") + std::string(extension)}
	} };

	std::optional<TextureHandle> handle = texMgr.createCubeMapFromFile(sampling, path, suffixesMapping, false, false, true);
	if (!handle)
	{
		if (logger)
		{
			logger->log(Log(LogLevel::Error, 0u, __FUNCTION__, "Could not load cubemap: {}", path));
		}
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

void loadObjects(MeshData& meshData, std::vector<ObjectPtr>& objects, std::shared_ptr<ILogger> logger)
{
	auto loadObject = [&meshData, &objects, &logger](auto& vertices, auto& indices, const char* objName) {
		Mesh mesh;
		mesh.addVertices(vertices, std::size(vertices));
		mesh.addIndices(indices, std::size(indices));
		mesh.computeBoundingBox();

		if (!mesh.computeTangents(logger) && logger)
		{
			logger->log(Log(LogLevel::Error, LogCode::Object_TangentComputation_Fail, "", "Tangent computation faile"));
		}

		MeshAlignment meshIndices = meshData.addMesh(mesh);
		ObjectPtr object(std::make_shared<Object>(meshIndices, mesh.getBoundingBox(), objName));
		objects.push_back(object);
	};

	loadObject(triangleVerts, triangleIndices, "Triangle");
	loadObject(triWallCubeVerts, triWallCubeIndices, "TriWallCube");
	loadObject(planeVerts, planeIndices, "Plane");
	loadObject(boxVerts, boxIndices, "Cube");
}

void setDefaultMaterials(std::vector<ObjectPtr>& objects, MaterialPtr mat)
{
	for (const ObjectPtr& obj : objects)
	{
		obj->setDefaultMaterial(mat);
	}
}

void setMaterials(std::vector<ObjectInstancePtr>& objectInstances, const std::vector<MaterialPtr>& materials)
{
	assert(objectInstances.size() >= 3u);
	assert(materials.size() >= 2u);
	objectInstances[2]->setMaterial(materials[1]);
}

void createObjectInstances(const std::vector<ObjectPtr>& objects, std::vector<ObjectInstancePtr>& objectInstances)
{
	assert(objects.size() >= 4);
	if (objects.size() >= 4)
	{
		ObjectPtrConst triangleObj = objects[0];
		ObjectPtrConst triWallCubeObj = objects[1];
		ObjectPtrConst planeObj = objects[2];
		ObjectPtrConst cubeObj = objects[3];

		objectInstances.push_back(std::make_shared<ObjectInstance>(triangleObj));
		objectInstances.push_back(std::make_shared<ObjectInstance>(triWallCubeObj));
		objectInstances.push_back(std::make_shared<ObjectInstance>(planeObj));
		objectInstances.push_back(std::make_shared<ObjectInstance>(cubeObj));
		objectInstances.push_back(std::make_shared<ObjectInstance>(triangleObj));
		objectInstances.push_back(std::make_shared<ObjectInstance>(cubeObj));
		objectInstances.push_back(std::make_shared<ObjectInstance>(cubeObj));
	}
}

void loadLights(std::vector<LightPtr>& lights)
{
	LightPtr light1(std::make_shared<Light>(Light::Type::Point, true));
	LightPtr light2(std::make_shared<Light>(Light::Type::Point, true));
	LightPtr light3(std::make_shared<Light>(Light::Type::Spot, true));
	LightPtr light4(std::make_shared<Light>(Light::Type::Spot, true));

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

bool createShadows(TextureManager& texMgr,
					FramebufferManager& fboMgr,
					const std::vector<LightPtr>& lights, 
					std::vector<LightFramebufferBinding>& spotFBOs,
					std::vector<LightFramebufferBinding>& pointFBOs,
					const QualitySettings<4u>& quality,
					std::shared_ptr<ILogger> logger)
{
	const unsigned int shadow2DRes = quality.getActive<EngineQuality::Shadow2DResolution>();
	const unsigned int shadowCubeRes = quality.getActive<EngineQuality::ShadowCubeResolution>();
	bool ok = true;

	for (LightPtr light : lights)
	{
		if (light->getType() == Light::Type::Spot)
		{
			if (logger)
			{
				logger->log(Log(LogLevel::Info, "", "Creating Framebuffer for 2D Shadow"));
			}

			TextureAttachmentDesc texAttachDesc{};
			texAttachDesc.glType = TextureType::Texture2D;
			texAttachDesc.format.inGPUColorFormat = ColorFormatInDevice::Depth;
			texAttachDesc.format.sourceColorFormat = ColorFormatInSource::Depth;
			texAttachDesc.format.sourceValueType = PixelDataTypeInSource::Float;
			texAttachDesc.sampling.minFilter = TextureFilteringMin::Nearest;
			texAttachDesc.sampling.magFilter = TextureFilteringMag::Nearest;
			texAttachDesc.sampling.wrapS = TextureWrapping::ClampToBorder;
			texAttachDesc.sampling.wrapT = TextureWrapping::ClampToBorder;

			FramebufferAttachmentDesc fboAttachDesc{};
			fboAttachDesc.attachment = FramebufferAttachment::Depth;
			fboAttachDesc.type = FramebufferAttachmentType::Texture;
			fboAttachDesc.desc = texAttachDesc;

			FramebufferDesc desc{};
			desc.resolution.width = shadow2DRes;
			desc.resolution.height = shadow2DRes;
			desc.attachments[toUnsigned(FramebufferAttachment::Depth)] = fboAttachDesc;

			std::optional<FramebufferHandleSet> handleSet = fboMgr.createFramebufferAndTextures(texMgr, desc);
			if (!handleSet || handleSet->texHandles.size() == 0u)
			{
				if (logger)
				{
					logger->log(Log(LogLevel::Error, __FUNCTION__, "Could not create texture for Spotlight"));
				}
				return false;
			}

			texMgr.setBorderColor(handleSet->texHandles[0], {1.0f, 1.0f, 1.0f, 1.0f});

			spotFBOs.push_back({light, *handleSet});
		}
		else if (light->getType() == Light::Type::Point)
		{
			if (logger)
			{
				logger->log(Log(LogLevel::Info, "", "Creating Framebuffer for Cubemap Shadow"));
			}

			TextureAttachmentDesc texAttachDesc{};
			texAttachDesc.glType = TextureType::TextureCube;
			texAttachDesc.format.inGPUColorFormat = ColorFormatInDevice::Depth;
			texAttachDesc.format.sourceColorFormat = ColorFormatInSource::Depth;
			texAttachDesc.format.sourceValueType = PixelDataTypeInSource::Float;
			texAttachDesc.sampling.minFilter = TextureFilteringMin::Linear;
			texAttachDesc.sampling.magFilter = TextureFilteringMag::Linear;
			texAttachDesc.sampling.wrapS = TextureWrapping::ClampToEdge;
			texAttachDesc.sampling.wrapT = TextureWrapping::ClampToEdge;
			texAttachDesc.sampling.wrapR = TextureWrapping::ClampToEdge;

			FramebufferAttachmentDesc fboAttachDesc{};
			fboAttachDesc.attachment = FramebufferAttachment::Depth;
			fboAttachDesc.type = FramebufferAttachmentType::Texture;
			fboAttachDesc.desc = texAttachDesc;

			FramebufferDesc desc{};
			desc.resolution.width = shadowCubeRes;
			desc.resolution.height = shadowCubeRes;
			desc.attachments[toUnsigned(FramebufferAttachment::Depth)] = fboAttachDesc;

			std::optional<FramebufferHandleSet> handleSet = fboMgr.createFramebufferAndTextures(texMgr, desc);
			if (!handleSet || handleSet->texHandles.size() == 0u)
			{
				if (logger)
				{
					logger->log(Log(LogLevel::Error, __FUNCTION__, "Could not create texture for Pointlight"));
				}
				return false;
			}

			pointFBOs.push_back({ light, *handleSet });
		}
	}

	return ok;
}

bool createMaterials(std::vector<MaterialPtr>& materials,
	const std::map<EngineProgramID, ProgramPtr>& enginePrograms,
	const std::vector<TextureHandle>& texturesPBR,
	const std::vector<TextureHandle>& texturesCube,
	const std::vector<TextureHandle>& texturesAlpha,
	std::shared_ptr<ILogger> logger)
{

	assert(texturesPBR.size() >= 8u && "Too few textures in texturesPBR");
	assert(texturesCube.size() >= 1u && "Too few textures in texturesCube");
	try
	{
		ProgramPtr prePBRProgram = enginePrograms.at(EngineProgramID::prePBR);
		ProgramPtr postprocessProgram = enginePrograms.at(EngineProgramID::postProcessing);
		ProgramPtr depthProgram = enginePrograms.at(EngineProgramID::depth);
		ProgramPtr skyboxProgram = enginePrograms.at(EngineProgramID::skybox);
		ProgramPtr particlesProgram = enginePrograms.at(EngineProgramID::particles);

		MaterialPtr material = std::make_shared<Material>();
		MaterialPtr material2 = std::make_shared<Material>();
		MaterialPtr materialSkybox = std::make_shared<Material>();
		MaterialPtr materialDebugCube = std::make_shared<Material>();
		MaterialPtr materialParticles = std::make_shared<Material>();
		materials.push_back(material);
		materials.push_back(material2);
		materials.push_back(materialSkybox);
		materials.push_back(materialDebugCube);
		materials.push_back(materialParticles);

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

		materialParticles->setProgram(particlesProgram);
		materialParticles->addTexture(texturesAlpha.at(0), "u_texture");
	}
	catch (const std::out_of_range& ex)
	{
		if (logger)
		{
			logger->log(Log(LogLevel::Critical, 0u, __FUNCTION__, "Out of Range: {}", ex.what()));
		}
		return false;
	}

	return true;
}

std::optional<FramebufferHandleSet> createMainFramebuffer(
	FramebufferManager& fboMgr,
	TextureManager& texMgr,
	const MainWindow& mw,
	QualitySettings<4u>& quality)
{
	TextureAttachmentDesc texAttDesc{};
	texAttDesc.glType = TextureType::Texture2D;
	texAttDesc.sampling.minFilter = TextureFilteringMin::Nearest;
	texAttDesc.sampling.magFilter = TextureFilteringMag::Nearest;
	texAttDesc.sampling.wrapS = TextureWrapping::ClampToEdge;
	texAttDesc.sampling.wrapT = TextureWrapping::ClampToEdge;
	texAttDesc.sampling.wrapR = TextureWrapping::Repeat;
	texAttDesc.format.inGPUColorFormat = (quality.getActive<EngineQuality::MainFBHighPrecision>() ?
		ColorFormatInDevice::RGB16F : ColorFormatInDevice::RGB);
	texAttDesc.format.sourceColorFormat = ColorFormatInSource::RGB;
	texAttDesc.format.sourceValueType = PixelDataTypeInSource::UnsignedByte;

	RenderBufferAttachmentDesc rboAttDesc{};
	rboAttDesc.internalFormat = ColorFormatInDevice::Depth24_Stencil8;

	FramebufferAttachmentDesc fboTexAttDesc{};
	fboTexAttDesc.attachment = FramebufferAttachment::Color0;
	fboTexAttDesc.type = FramebufferAttachmentType::Texture;
	fboTexAttDesc.desc = texAttDesc;

	FramebufferAttachmentDesc fboRboAttDesc{};
	fboRboAttDesc.attachment = FramebufferAttachment::DepthStencil;
	fboRboAttDesc.type = FramebufferAttachmentType::RenderBuffer;
	fboRboAttDesc.desc = rboAttDesc;

	FramebufferDesc desc{};
	desc.resolution.width = mw.getWidth();
	desc.resolution.height = mw.getHeight();
	desc.attachments[toUnsigned(FramebufferAttachment::Color0)] = fboTexAttDesc;
	desc.attachments[toUnsigned(FramebufferAttachment::DepthStencil)] = fboRboAttDesc;;

	std::optional<FramebufferHandleSet> handleSet = fboMgr.createFramebufferAndTextures(texMgr, desc);

	return handleSet;
}

void maualObjectsPreTransformations(std::vector<ObjectInstancePtr>& instances)
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

void manualObjectsTransformations(std::vector<ObjectInstancePtr>& instances, const TimeDrivenMovement &tdm)
{
	assert(instances.size() > 1);
	static LinearUpdate obj1RotY(tdm, 0.0f, glm::radians(45.0f));

	instances[1]->setRotation(glm::rotate(glm::mat4(1.0f), obj1RotY.getVal(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void updateCamera(GLFWwindow* window, Camera& camera, const TimeDrivenMovement &tdm)
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
