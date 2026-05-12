#pragma once
#include <GL/glew.h>

namespace dj
{

struct TextureSamplingDesc
{
	GLenum wrapS;
	GLenum wrapT;
	GLenum wrapR;
	GLenum minFilter;
	GLenum magFilter;

	void setWrapping(GLenum s, GLenum t, GLenum r = GL_REPEAT)
	{
		wrapS = s;
		wrapT = t;
		wrapR = r;
	}

	void setFiltering(GLenum min, GLenum mag)
	{
		minFilter = min;
		magFilter = mag;
	}
};

} // namespace dj