#include "Shader.h"
#include "Definitions.h"
#include "Logging/Log.h"
#include "Enums/LogCodes.h"
#include "Utils/GLWrapper.h"
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

bool Shader::compile(std::shared_ptr<ILogger> logger)
{
	GLint ok;
	glCompileShader(index);
	glGetShaderiv(index, GL_COMPILE_STATUS, &ok);

	if (!ok && logger)
	{
		logger->log(Log(LogLevel::Error, "Shader", glGetShaderInfoLogString(index), LogCode::Shader_Compilation_Fail));
	}

	return static_cast<bool>(ok);
}

void Shader::deleteShader()
{
	glDeleteShader(index);
}

ShaderID Shader::getIndex() const
{
	return index;
}