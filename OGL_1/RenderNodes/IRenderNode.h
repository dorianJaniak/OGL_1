#pragma once
#include "../Framebuffer.h"
#include "../DefinitionsGL.h"
#include <string>
#include <unordered_map>

namespace dj {
class TextureManager;
} // namespace dj

namespace dj {

/*! \class IRenderNode
	\brief Abstract base class for Rendering Nodes implementation.

	IRenderNode is limited to most basic functionality related to FBO and Textures handling.
	It implements only first stage of Render Node's processing, which is cleaning FBO related
	flags. Rest of the implementation needs to be done in its children classes.
	By calling \ref run method, IRenderNode starts rendering:
	-# calls \ref preFrame method - binds FBO, sets Viewport, sets FBO flags,
	-# calls virtual \ref draw method from children classes.
	
	First part of the interface is related to FBO. Each IRenderNode requires \ref Framebuffer assigned,
	to which it will rasterize its output. It also requires setting flags in order to handle and
	reset properly Framebuffer before rasterization begins.
	In order to configure it properly following things needs to be specified:
	- \ref FramebufferPtr - output FBO,
	- \ref setClearFlags, \ref enableDepthTest, \ref enableDepthUpdate, \ref setFaceCulling functions needs to be called OR
	- \ref setConfiguration to set all FBO related flags in a single shot.

	Second part of the interface is related to input Textures. By calling \ref addTexture method
	Textures can be added. These Textures will be "global" from perspective of the Node, which means they will
	be accessible from its children and should be used as a Textures not related to Object Materials.
	In order to bind these Textures, \ref bindAndUniformNodeTextures method needs to be called 
	in derived class. For binding Textures that are not part of the Node itself (like Material Textures)
	IRenderNode provides following methods: \ref bindTexture and \ref bindAndUniformTexture.

	Due to OpenGL limits (max 32 Texture Units), IRenderNode counts binded Textures.
	This class provides methods that allows tracking this number.
	Derived class should use:
	- \ref beginBindingTextures to reset binded Textures count - it should be performed at the very
	beginning of Render Node,
	- \ref getTextureUnitNo to get binded Textures count,
	- \ref getFreeTextureUnitsCount to get number of free Texture Units (available slots for Textures),
	
	Some implementations of Render Nodes may require to split Textures into sections. 
	For example: 
	3 textures are global for Render Node and are passed to it by \ref addTexture method.
	Aditional Textures are Material dependent, thus they will change per each Material change.
	In order to not bind the 3 Textures that are "global" on each Program change, it is sufficient
	to remember index that separates these "global" Textures and Material related Textures.
	Use these methods:
	- \ref saveTextureUnitNo to save starting index (offset) for specific Textures section,
	- \ref restoreTextureUnitNo to reset binded Textures count to starting index of specific Textures section.
*/
class IRenderNode {

	constexpr static unsigned int c_maxTextureUnitsCount = GL_TEXTURE31 - GL_TEXTURE0;
	constexpr static unsigned int c_maxTextureUnitOffsets = 16u;

	const TextureManager& texMgr;
	std::string name;
	FramebufferPtr output;
	GLuint clearFlags;
	bool depthTestEnabled;
	bool depthUpdateEnabled;
	GLuint culling;
	unsigned int textureUnitNo;
	unsigned int textureUnitNoOffsets[c_maxTextureUnitOffsets];

	std::unordered_map<std::string, Handle> nodeInputTextures;

protected:
	/*! \brief Core pure virtual Node's method. Needs to be implemented in derived class.
	 
	*/
	virtual void draw() = 0;

	/*! \brief Resets Binded Textures counter
	*/
	void beginBindingTextures();
	/*! \brief Creates textures section (i.e. remembers value of binded textures counter) 
			under the specified index
		\param[in] partIndex number of section under which offset needs to be remembered
	*/
	void saveTextureUnitNo(unsigned int partIndex);
	/*! \brief Sets Binded Textures counter to offset specified by textures section index
		\param[in] partIndex number of section under which offset is remembered
	*/
	void restoreTextureUnitNo(unsigned int partIndex);

	/*! \brief Binds and Uniforms Texture
	*	
	*	It is general method for binding and "uniforming" Texture. It should be used
	*	for any type of the texture. It should be used for both Node Textures and Material Textures. 
	*	Depending on use case it might be useful to
	*	call \ref restoreTextureUnitNo before calling it. 
	*	It binds Texture to free Texture Unit, "uniforms" them (i.e. sends id to the Program),
	*	and increases Binded Textures counter.
	*	\param[in] texInfo Texture informations
	*	\param[in] uniformLocation uniform location for specific \ref Program
	*	\return true if successful, false if there was no free Texture Units
	*/
	bool bindAndUniformTexture(const Handle& texHandle, GLint uniformLocation);
	/*! \brief Binds Texture [DEPRECEATED]
	*/
	bool bindTexture(const Handle& texHandle);
	/*! \brief Binds and Uniforms Textures added to this Node
	*	
	*	It should be called from children node. It binds all Textures added
	*   to this Node to free Texture Units. It automatically "uniforms" 
	*	(sends Textures IDs to the Program) them.
	*	It might be useful to call \ref restoreTextureUnitNo before calling this method. 
	*	\param[in] program Program for which Textures needs to be "uniformed"
	*	\return true if successfull, false if reached limit of free Texture Units or 
	*			if it could not find uniform location in the Program
	*/
	bool bindAndUniformNodeTextures(ConstProgramPtr program);

	/*! \brief Returns Binded Textures count (occupied Texture Units count)
		\return count of Binded Textures
	*/
	unsigned int getTextureUnitNo() const;
	/*! \brief Returns free Texture Units count (how many Textures can be bound yet)
		\return number of free Texture Units 
	*/
	unsigned int getFreeTextureUnitsCount() const;


public:
	IRenderNode(const TextureManager& texMgr, FramebufferPtr output, const std::string& name = "");
	/*! \brief Core Node's method - starts Rendering
		- Calls \ref preFrame method
		- Calls pure virtual \ref draw method 
	*/
	void run();

	/*! \brief Sets FBO output clearing flags
		\param[in] flags OpenGL flags for glClear (for example: GL_COLOR_BUFFER_BIT)
	*/
	void setClearFlags(GLuint flags);
	/*! \brief Sets a flag that control whether Depth Test is enabled during Rendering.
		\param[in] enable Depth Test is active when true (glEnable(GL_DEPTH_TEST)), otherwise disabled (glDisable(GL_DEPTH_TEST))
	*/
	void enableDepthTest(bool enable);
	/*! \brief Sets a flag that controls whether Depth Test effects are stored in Depth Buffer
		\param[in] enable Stores Depth Test results in Depth Buffer if true (glDepthMask(GL_TRUE)), otherwise it does not (glDepthMask(GL_FALSE))
	*/
	void enableDepthUpdate(bool enable);
	/*! \brief Sets Face Culling
		\param[in] culling GL_FRONT, GL_BACK or GL_FRONT_AND_BACK to enable face culling, otherwise disabled
	*/
	void setFaceCulling(GLuint culling);
	void setConfiguration(GLuint clearFlags, bool depthTestEnabled, bool depthUpdateEnabled, GLuint faceCulling);
	
	/*! \brief Adds Texture input into a Node
		\param[in] samplerName texture sampler name in Shader
		\param[in] typeInfo texture info
	*/
	void addTexture(const std::string& samplerName, const Handle& handle);

	/*! \brief Sets all flags related to FBO
		- binds FBO,
		- set Viewport (based on FBO's Texture size)
		- clears buffers (specified in \ref setClearFlags)
		- enables / disables Depth Buffer Update (specified in \ref enableDepthTest)
		- sets Face Culling (specified in \ref setFaceCulling)
		- enables / disables Depth Test (specified in \ref enableDepthTest)
	*/
	void preFrame();
private:
};

} // namespace dj