#pragma once
#include "InputCallback.h"

namespace dj
{

class DebugTweaks : public InputCallback
{
	unsigned int activeCameraIndex;
	bool gammaCorrection;
	unsigned int debugVertices;

public:

	DebugTweaks();

	unsigned int getActiveCameraIndex() const;
	bool isGammaCorrected() const;
	unsigned int getDebugVertices() const;

	void changeCamera();
	void switchGammaCorrection();
	void switchDebugVertices();

	virtual void keyCallback(int key, int scancode, int action, int mods) final;
};

} // namespace dj