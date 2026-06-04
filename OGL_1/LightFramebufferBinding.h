#pragma once
#include "Definitions.h"
#include "FramebufferManager.h"

namespace dj
{

/*!
*	\todo
*	- it is a helper class only (temporary)
*/
struct LightFramebufferBinding
{
	dj::LightPtr light;
	dj::FramebufferHandleSet fbo;
};

} // namespace dj