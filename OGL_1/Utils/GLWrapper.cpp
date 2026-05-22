#include "GLWrapper.h"
#include "Enums/Converters.h"
using namespace dj;

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
