#include "Program.h"
#include "Shader.h"
#include "Definitions.h"
using namespace dj;

Program::Program() noexcept
	: indexOk(false)
	, index(0u)
{
}

Program::Program(Program&& prog) noexcept
	: indexOk(prog.indexOk)
	, index(prog.index)
{
	prog.indexOk = false;
	prog.index = 0u;
}

bool Program::prepare(const char* const vSource, const char* const fSource, Log &log, const std::string &progName, const char* const gSource)
{
	if (indexOk)
	{
		log.ok = false;
		log.print("Executed prepare on already prepared program");
		return false;
	}

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
	vShader.compile(log);
	std::string msg = std::string("Vertex Shader Compilation (") + progName + std::string(")");
	log.print(msg.c_str());
	if (!log.ok)
	{
		return false;
	}

	// Compile Fragment Shader
	fShader.compile(log);
	msg = std::string("Fragment Shader Compilation (") + progName + std::string(")");
	log.print(msg.c_str());
	if (!log.ok)
	{
		return false;
	}

	if (gSource)
	{
		gShader.compile(log);
		msg = std::string("Geometry Shader Compilation (") + progName + std::string(")");
		log.print(msg.c_str());
		if (!log.ok)
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
	link(log);
	msg = std::string("Program Linking (") + progName + std::string(")");
	log.print(msg.c_str());
	if (!log.ok)
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

bool Program::link(Log& log)
{
	glLinkProgram(index);
	glGetProgramiv(index, GL_LINK_STATUS, &log.ok);
	if (!log.ok)
	{
		glGetProgramInfoLog(index, 1024, nullptr, log.log);
	}

	return log.ok;
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