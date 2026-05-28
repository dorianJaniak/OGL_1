#pragma once

#include "DefinitionsGL.h"
#include <string>
#include <unordered_map>

namespace dj
{
	class Shader;
	struct Log;
}

namespace dj
{

/*! \class Program
	\brief OpenGL Shader Program implementation.

	By instancing this class new OpenGL Program ID is created. This class delivers 
	interface for handling OpenGL Shader Programs. \n
	To create the Program \ref prepare method needs to be called with at least vector and fragment Shader source.
	It also supports geometry Shaders. \n
	\ref Program holds map which translates Uniform Names (names used in Shader source) to
	Uniform Locations (IDs of Uniform variables obtained from GPU during Uniform registration). 
	Call \ref registerUniform methods in order to register new Uniform Variable. Later it is sufficient
	to call \ref getUniformLocation in order to obtain Uniform ID. \n
	\ref Program delivers also \ref addUniformLimit method which allows to remember any
	uniform related limits. For example it can store maximum number of point lights supported
	by this Program. In order to get such number call \ref getUniformLimit. 

	\todo
	- move somewhere temporary functions - \ref assignTextureUnit and \ref getTextureUnit
*/
class Program {
	bool indexOk;
	ProgramID index;

	std::unordered_map<std::string, GLint> uniformLocations;
	std::unordered_map<std::string_view, unsigned int> uniformLimits;
	std::string name;
	
	// temp
	std::string textureUniforms[c_textureUnitsMax];

public:
	Program() noexcept;
	Program(const Program& prog) = delete;
	Program(Program&& prog) noexcept;

	/*! \brief Prepares, compiles Shaders and links the Program
		\param[in] vSource pointer to the raw source code of vertex Shader
		\param[in] fSource pointer to the raw source code of fragment Shader
		\param[out] log reference to Log, which will be used for logging errors (printing out in the console)
		\param[in] programName name of the Program which will be used for logging errors
		\param[in] gSource *[optional]* pointer to the raw source code of geometry Shader
		\return true if successfully created Shaders and linked them into single Program, otherwise false
	*/
	bool prepare(const char* const vSource, const char* const fSource, Log &log, const std::string &programName, const char *const gSource = nullptr);
	void use() const;

	bool registerUniform(const char* uniformName);
	bool registerUniform(const std::string& uniformName);
	void addUniformLimit(const char* const keyword, unsigned int limit);
	
	GLint getUniformLocation(const std::string& uniformName) const;
	unsigned int getUniformLimit(const char* const keyword) const;

	// temp
	void assignTextureUnit(const char* uniformName, GLuint textureUnit);
	unsigned int getTextureUnit(const std::string& uniformName) const;
	// temp

	ProgramID getIndex() const;
	const std::string& getName() const;
private:
	bool link(Log& log);
};

}