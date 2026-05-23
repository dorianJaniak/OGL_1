#pragma once

namespace dj
{

class InputCallback
{
public:
	virtual void keyCallback(int key, int scancode, int action, int mods);
	virtual void cursorPosCallback(double xPix, double yPix, double xNorm, double yNorm);
	virtual void resizeCallback(unsigned int width, unsigned int height);
};

} // namespace dj
