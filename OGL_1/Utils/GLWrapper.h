#pragma once
#include "Enums/GLTypes.h"
#include "Enums/TextureColorEnums.h"
#include "Enums/TextureParameterEnums.h"

namespace dj
{

/*!
* \todo Consider adding more functions, with awarness that GLenum target argument for glTexImage2D, glBindTexture and glGenerateMipmaps is different in each case
*/

void glBindTexture(TextureType type, GLuint id);
void glTexImage2D(GLenum type, GLint level, ColorFormatInDevice internalFormat, unsigned int width, unsigned int height, int border, ColorFormatInSource sourceFormat, PixelDataTypeInSource sourceDataType, const void* data);
void glTexImage2D(TextureCubeSide side, GLint level, ColorFormatInDevice internalFormat, unsigned int width, unsigned int height, int border, ColorFormatInSource sourceFormat, PixelDataTypeInSource sourceDataType, const void* data);

} // namespace dj