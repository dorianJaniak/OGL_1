#pragma once
#include "IRenderNode.h"
#include "Object.h"
#include "Camera.h"
#include <vector>

namespace dj
{
class TextureManager;
class FramebufferManager;
} // namespace dj

namespace dj {

/*! \class IRenderWorldNode
	\brief Core of the 3D Object Instances rendering.
		
	It is middle interface in Rendering Nodes hierarchy. 
	IRenderWorldNode Implements \ref draw function, which is a core rendering function of a Node.
	\ref draw is called by \ref IRednerNode after it prepares \ref Framebuffer for rendering
	(by clearing viewport, setting buffer flags, etc.).
	IRenderWorldNode keeps the list of Object Instances over which it iterates and renders them,
	respecting their Materials and Programs.
*/
class IRenderWorldNode : public IRenderNode {

	std::vector<dj::ObjectInstancePtr> objectInstances;
	GLuint ebo;
	MaterialPtr customMaterial;

public:
	IRenderWorldNode(
		const TextureManager& texMgr,
		const FramebufferManager& fboMgr,
		FramebufferHandle output,
		GLuint ebo,
		std::vector<dj::ObjectInstancePtr>& objectInstances,
		const std::string& name = "");

	/*! \brief Replace all Materials assigned to Object Instances. Forces Node to render all Objects with this Material.
		\param[in] material Replacement Material
	*/
	void setCustomMaterial(MaterialPtr material);

protected:
	/*! \brief Main rendering function
	* 
	*	Iterates over Object Instances, gets their Materials, binds and uniforms Textures,
	*	calls methods defined in children classes to "uniform" (send to the Program) all uniforms.
	*	When this function is called, following algorithm is executed:
	*	-# \ref uboPerFrame is called - children Node should handle Uniform Buffer Object there,
	*	-# binds EBO (Element Buffer Object)
	*	-# iterates over Object Instances
	*	-# when Material or Program between Objects has changed: 
	*		- calls \ref IRenderNode::beginBindingTextures - in order to reset Binded Textures counter
	*		- binds \ref Program
	*		- calls \ref uniformPerProgram - children Node should register uniforms that are common per Program,
	*		- calls \ref IRenderNode::bindAndUniformNodeTextures - binds and uniforms Node Input Textures,
	*		- calls \ref bindMaterialTextures - gets Textures assigned to active Material and binds and uniforms them,
	*		- calls \ref bindAndUniformTextures [DEPRECEATED]
	*		- calls \ref uniformPerMaterial - children Node should register uniforms that are related to specific Material,
	*	-# calls \ref uniformPerObject - children Node should register uniforms related to the Object Instance
	*	-# renders objects by calling OpenGL drawing function
	*/
	void draw() final;
	
	/*! \brief Virtual method for handling Uniform Buffer Object before the start of Frame Rendering 
	*/
	virtual void uboPerFrame() = 0;
	/*! \brief Virtual method for registering all uniform variables per Program Change.
		It is called by \ref draw method, anytime different Program is used between Object Instances.
		\param[in] program pointer to the active \ref Program 
	*/
	virtual void uniformPerProgram(ConstProgramPtr program) = 0;
	/*! \brief Virtual method for registering all uniform variables per Material Change.
		It is called by \ref draw method anytime different Material is used between Object Instances.
		\param[in] program pointer to the active \ref Program
		\param[in] material pointer to the active \ref Material
	*/
	virtual void uniformPerMaterial(ConstProgramPtr program, ConstMaterialPtr material) = 0;
	/*! \brief Virtual method for registering all uniform variables per Object Instance.
		It is called by \ref draw method per each Object Instance that is processed.
		\param[in] program pointer to the active \ref Program
		\param[in] obj pointer to the active \ref ObjectInstance
	*/
	virtual void uniformPerObject(ConstProgramPtr program, ObjectInstancePtr obj) = 0;

	/*! \brief Iterates over active Material Textures and binds and uniforms them.
		\param[in] program - pointer to the active \ref Program
		\param[in] material - pointer to the active \ref Material
	*/
	virtual void bindMaterialTextures(ConstProgramPtr program, ConstMaterialPtr material);
	/*! \brief DEPRECEATED - it does nothing
	*/
	virtual void bindAndUniformTextures();
};

} // namespace dj