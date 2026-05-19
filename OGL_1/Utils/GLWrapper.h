#pragma once
#include "Enums/GLTypes.h"
#include "Enums/TextureColorEnums.h"
#include "Enums/TextureParameterEnums.h"

/*!
* \todo Consider adding more functions, with awarness that GLenum target argument for glTexImage2D, glBindTexture and glGenerateMipmaps is different in each case
*/

void glBindTexture(dj::TextureType type, GLuint id);
void glTexImage2D(GLenum type, GLint level, dj::ColorFormatInDevice internalFormat, unsigned int width, unsigned int height, int border, dj::ColorFormatInSource sourceFormat, dj::PixelDataTypeInSource sourceDataType, const void* data);
void glTexImage2D(dj::TextureCubeSide side, GLint level, dj::ColorFormatInDevice internalFormat, unsigned int width, unsigned int height, int border, dj::ColorFormatInSource sourceFormat, dj::PixelDataTypeInSource sourceDataType, const void* data);
