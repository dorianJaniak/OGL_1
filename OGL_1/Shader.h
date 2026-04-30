#pragma once
#include "DefinitionsGL.h"

namespace dj 
{
	struct Log;
}

namespace dj
{

class Shader {
	//const char* source;
	ShaderID index;
public:
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader(GLenum shaderType);
	~Shader();

	void source(const char* source);
	bool compile(Log& log);
	void deleteShader();

	ShaderID getIndex() const;
};

} // namespace dj