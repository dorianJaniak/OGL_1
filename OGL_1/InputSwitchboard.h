#pragma once
#include "InputCallback.h"
#include <vector>
#include <functional>
#include <memory>

namespace dj
{

class InputSwitchboard
{
	std::vector<std::shared_ptr<InputCallback>> callbacks;

public:
	InputSwitchboard() = default;
	
	void addCallback(std::shared_ptr<InputCallback> callback);
	bool removeCallback(std::shared_ptr<InputCallback> callback);
	void removeAllCallbacks();

protected:
	void propagateKeyCallback(int key, int scancode, int action, int mods);
	void propagateCursorPosCallback(double xPix, double yPix, double xNorm, double yNorm);
	void propagateResizeCallback(unsigned int width, unsigned int height);
};

} // namespace dj