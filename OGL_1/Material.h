#pragma once

#include "Definitions.h"
#include "DefinitionsGL.h"
#include "Texture.h"
#include <GL/glew.h>
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace dj {
	class Program;
	class Material;
} // namespace dj

namespace dj {

// In future: add Material Instance
/*! \class Material
	\brief Class that represents Material (or Material Instance to be more specific). 
	
	Material / Material Instance holds pointer to the \ref Program. It needs the pointer
	in order to get uniform location for Textures. 
	By calling \ref addTexture specific Textures can be assigned to the Material.

	\todo
	- Removing Textures
	- Reassigning Textures (?)
*/
class Material
{

private:
	ProgramWeakPtr program;
	// Index of this vector is the Textue Unit number
	// id of the texture is the id of Texture Data (+Configuration) stored in GPU
	// uniformName is NOT NEEDED
	//std::vector<TextureID> textures; // Vector index is the Texture Unit number; Id is the number of Texture Data in GPU; 

	/*! \brief Texture Bindings. \n
		ID of each Texture is the ID of Texture Data (+ Configuration) stored in GPU (specific Texture handler). \n
		Uniform Name is needed since it connects sampler with Texture ID.
	*/
	std::vector<TextureBinding> textures;

public:
	Material();

	/*! \brief Replaces active Program pointer without checks.
		\param[in] program pointer to the new active \ref Program
	*/
	void setProgram(ProgramWeakPtr program);
	/*! \brief Adds / Assigns Texture to the Material
	*	\param[in] texture specific Texture handler with type info to be added
	*	\param[in] samplerName name of the sampler in the \ref Program
	*	\return true if successfull, false if Program does not exist or if could not locate Uniform ID
	*/
	bool addTexture(const TextureTypeInfo& texture, const std::string& samplerName);
	
	unsigned int getTexturesCount() const;
	const std::vector<TextureBinding>& getTextures() const;

	ProgramWeakPtr getProgram() const;

private:
	unsigned int getIndex(const TextureTypeInfo& texInfo) const;
};

} // namespace dj