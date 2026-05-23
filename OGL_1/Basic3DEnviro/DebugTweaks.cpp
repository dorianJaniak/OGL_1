#include "DebugTweaks.h"
#include <GLFW/glfw3.h>
using namespace dj;

DebugTweaks::DebugTweaks()
	: activeCameraIndex(0)
	, gammaCorrection(false)
	, debugVertices(0u)
{
}

unsigned int DebugTweaks::getActiveCameraIndex() const
{
	return activeCameraIndex;
}

bool DebugTweaks::isGammaCorrected() const
{
	return gammaCorrection;
}

unsigned int DebugTweaks::getDebugVertices() const
{
	return debugVertices;
}

void DebugTweaks::changeCamera()
{
	activeCameraIndex = (activeCameraIndex + 1u) % 4u;
}

void DebugTweaks::switchGammaCorrection()
{
	gammaCorrection = !gammaCorrection;
}

void DebugTweaks::switchDebugVertices()
{
	debugVertices = (debugVertices + 1u) % 4u;
}

void DebugTweaks::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		changeCamera();
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		switchGammaCorrection();
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		switchDebugVertices();
	}
}