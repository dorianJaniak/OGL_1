#include "Shader.h"
#include "Definitions.h"
using namespace dj;

Shader::Shader(GLenum shaderType)
	: index(0u)
{
	index = glCreateShader(shaderType);
}

Shader::~Shader()
{
	deleteShader();
}

void Shader::source(const char* source)
{
	glShaderSource(index, 1, &source, nullptr);
}

bool Shader::compile(Log& log)
{
	glCompileShader(index);
	glGetShaderiv(index, GL_COMPILE_STATUS, &log.ok);

	if (!log.ok)
	{
		glGetShaderInfoLog(index, 1024, nullptr, log.log);
	}

	return log.ok;
}

void Shader::deleteShader()
{
	glDeleteShader(index);
}

ShaderID Shader::getIndex() const
{
	return index;
}