#include "GLWrapper.h"
#include "Enums/Converters.h"
#include <array>
using namespace dj;

namespace {
std::array<char, 2048u> logArray;
}

void glBindTexture(TextureType type, GLuint id)
{
	glBindTexture(toGLenum(type), id);
}

void glTexImage2D(GLenum type, GLint level, ColorFormatInDevice internalFormat, unsigned int width, unsigned int height, int border, ColorFormatInSource sourceFormat, PixelDataTypeInSource sourceDataType, const void* data)
{
	glTexImage2D(type, level, toGLenum(internalFormat), width, height, border, toGLenum(sourceFormat), toGLenum(sourceDataType), data);
}

void glTexImage2D(TextureCubeSide side, GLint level, ColorFormatInDevice internalFormat, unsigned int width, unsigned int height, int border, ColorFormatInSource sourceFormat, PixelDataTypeInSource sourceDataType, const void* data)
{
	glTexImage2D(toGLenum(side), level, toGLenum(internalFormat), width, height, border, toGLenum(sourceFormat), toGLenum(sourceDataType), data);
}

std::string glGetShaderInfoLogString(GLuint shaderID)
{
	glGetShaderInfoLog(shaderID, static_cast<GLsizei>(logArray.size()), nullptr, logArray.data());
	return logArray.data();
}

std::string glGetProgramInfoLogString(GLuint programID)
{
	glGetProgramInfoLog(programID, static_cast<GLsizei>(logArray.size()), nullptr, logArray.data());
	return logArray.data();
}
