#include "Program.h"
#include "Shader.h"
#include "Definitions.h"
#include "Utils/GLWrapper.h"
#include "Enums/LogCodes.h"
using namespace dj;

Program::Program(std::shared_ptr<ILogger> logger) noexcept
	: NamedLoggingInstance(logger, "")
	, indexOk(false)
	, index(0u)
{
}

Program::Program(Program&& prog) noexcept
	: NamedLoggingInstance(std::move(prog))
	, indexOk(prog.indexOk)
	, index(prog.index)
{
	prog.indexOk = false;
	prog.index = 0u;
}

bool Program::prepare(const char* const vSource, const char* const fSource, const std::string &progName, const char* const gSource)
{
	if (indexOk)
	{
		log(LogLevel::Error, LogCode::Program_Initialized_Fail, "Tried to reinitialize program");
		return false;
	}

	setName(progName);

	// Create and compile shaders separately
	dj::Shader vShader(GL_VERTEX_SHADER);
	dj::Shader fShader(GL_FRAGMENT_SHADER);
	dj::Shader gShader(GL_GEOMETRY_SHADER);

	// Source Shaders
	vShader.source(vSource);
	fShader.source(fSource);
	if (gSource)
	{
		gShader.source(gSource);
	}

	// Compile Vertex Shader
	log(LogLevel::Debug, 0u, "Vertex Shader compilation");
	if (!vShader.compile(logger))
	{
		return false;
	}

	// Compile Fragment Shader
	log(LogLevel::Debug, 0u, "Fragment Shader compilation");
	if (!fShader.compile(logger))
	{
		return false;
	}

	if (gSource)
	{
		log(LogLevel::Debug, 0u, "Geometry Shader compilation");
		if (!gShader.compile(logger))
		{
			return false;
		}
	}

	// Create Program
	index = glCreateProgram();

	// Attach shaders
	glAttachShader(index, vShader.getIndex());
	glAttachShader(index, fShader.getIndex());
	if (gSource)
	{
		glAttachShader(index, gShader.getIndex());
	}

	// Link program
	log(LogLevel::Debug, 0u, "Program linking");
	if (!link())
	{
		// Delete Program because unsuccessful
		glDeleteProgram(index);
		return false;
	}

	// Delete shaders
	vShader.deleteShader();
	fShader.deleteShader();
	gShader.deleteShader();

	indexOk = true;

	return true;
}

bool Program::link()
{
	GLint ok;
	glLinkProgram(index);
	glGetProgramiv(index, GL_LINK_STATUS, &ok);
	if (!ok)
	{
		log(LogLevel::Error, LogCode::Program_Link_Fail, glGetProgramInfoLogString(index));
	}

	return static_cast<bool>(ok);
}

void Program::use() const
{
	glUseProgram(index);
}

bool Program::registerUniform(const char* uniformName)
{
	GLint loc = glGetUniformLocation(index, uniformName);

	if (loc == -1)
	{
		return false;
	}

	uniformLocations.insert({ std::string(uniformName), loc });
	return true;
}

bool Program::registerUniform(const std::string& uniformName)
{
	return registerUniform(uniformName.c_str());
}

void Program::addUniformLimit(const char* const keyword, unsigned int limit)
{
	uniformLimits.insert({ keyword, limit });
}

GLint Program::getUniformLocation(const std::string& uniformName) const
{
	if (auto elem = uniformLocations.find(uniformName); elem != uniformLocations.end())
	{
		return elem->second;
	}

	return -1;
}

unsigned int Program::getUniformLimit(const char* const keyword) const
{
	if (auto elem = uniformLimits.find(keyword); elem != uniformLimits.end())
	{
		return elem->second;
	}

	return 0u;
}

ProgramID Program::getIndex() const
{
	return index;
}

// temp
void Program::assignTextureUnit(const char* uniformName, GLuint textureUnit)
{
	unsigned int textureUnitNorm = toProgramTU(textureUnit);
	if (uniformName && textureUnitNorm < c_textureUnitsMax)
	{
		textureUniforms[textureUnitNorm] = { uniformName };
		glUniform1i(glGetUniformLocation(index, uniformName), textureUnitNorm);
	}
}

// temp
unsigned int Program::getTextureUnit(const std::string& uniformName) const
{
	for (unsigned int i = 0; i < c_textureUnitsMax; ++i)
	{
		if (textureUniforms[i] == uniformName)
		{
			return i;
		}
	}

	return c_textureUnitsMax;
}