#include "InputSwitchboard.h"
using namespace dj;

void InputSwitchboard::addCallback(std::shared_ptr<InputCallback> callback)
{
	callbacks.push_back(callback);
}

bool InputSwitchboard::removeCallback(std::shared_ptr<InputCallback> callback)
{
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
	{
		if (*it == callback)
		{
			callbacks.erase(it);
			return true;
		}
	}

	return false;
}

void InputSwitchboard::removeAllCallbacks()
{
	callbacks.clear();
}

void InputSwitchboard::propagateKeyCallback(int key, int scancode, int action, int mods)
{
	for (std::shared_ptr<InputCallback> callback : callbacks)
	{
		callback->keyCallback(key, scancode, action, mods);
	}
}

void InputSwitchboard::propagateCursorPosCallback(double xPix, double yPix, double xNorm, double yNorm)
{
	for (std::shared_ptr<InputCallback> callback : callbacks)
	{
		callback->cursorPosCallback(xPix, yPix, xNorm, yNorm);
	}
}

void InputSwitchboard::propagateResizeCallback(unsigned int width, unsigned int height)
{
	for (std::shared_ptr<InputCallback> callback : callbacks)
	{
		callback->resizeCallback(width, height);
	}
}