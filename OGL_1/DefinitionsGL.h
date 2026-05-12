#pragma once
#include <GL/glew.h>
#include <memory>

/*
* This Header is created only to clarify some things in OpenGL
*/

namespace dj {

constexpr unsigned int c_textureUnitsMax = GL_TEXTURE31 - GL_TEXTURE0 + 1u;

using TextureID = GLuint;
using TextureUnitID = GLuint;
using ProgramID = GLuint;
using ShaderID = GLuint;
using FramebufferID = GLuint;
using TextureGLType = GLenum;

//using TexturePtr = std::shared_ptr<TextureID>;
using TextureUnitIDPtr = std::shared_ptr<TextureUnitID>;
using ProgramIDPtr = std::shared_ptr<ProgramID>;
using ShaderIDPtr = std::shared_ptr<ShaderID>;

// Converts GL_TEXTUREx value to value used by Shader Program
// Explanation:
// Identifiers for Texture Units in OpenGL are defined by definitions
// GL_TEXTURE0, GL_TEXTURE1, etc. Their value does not start from 0, but
// glActiveTexture(...) operates on this range. Meanwhile it is required to
// assign Texture Units to uniforms in Shader Program, which does not operate
// on the same range. It operates on range starting from 0.
// Thus, this function converts GL_TEXTUREx value to the one compatible with
// Shader Program uniforms numbering (passed with glUniform1i)
inline TextureUnitID toProgramTU(GLuint textureUnit)
{
	return (textureUnit - GL_TEXTURE0);
}

inline GLint getUniformLocation(ProgramID prog, const char* uniformName)
{
	return glGetUniformLocation(prog, uniformName);
}

inline const char* toCString(GLenum gle)
{
	switch (gle)
	{
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_INVALID_INDEX: return "GL_INVALID_INDEX";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_NO_ERROR: return "GL_NO_ERROR";
	}

	return "Undefined";
}

} // namespace dj