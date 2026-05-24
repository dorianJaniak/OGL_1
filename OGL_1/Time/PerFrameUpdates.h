#pragma once
#include "TimeDrivenMovement.h"
#include <chrono>
#include <glm.hpp>

namespace dj
{

template <class T>
class PerFrameUpdate
{
	const TimeDrivenMovement& tdm;
	T value;
	T stepMs;
public:
	PerFrameUpdate(const TimeDrivenMovement& tdm, T initVaue)
		: tdm(tdm)
		, value(initVaue)
	{
	}

	void resetValue(T startValue)
	{
		// Correction of frameStart
		value = T();
		value = startValue - getVal();
	}

	void setStepPerMs(T step)
	{
		this->stepMs = step;
	}

	void setStepPerS(T step)
	{
		this->stepMs = step / static_cast<T>(1000.0);
	}

	T getVal()
	{
		value += stepMs * tdm.getFrameDiffMs().count();
		return value;
	}
};

class LinearUpdate : public PerFrameUpdate<float>
{
public:
	LinearUpdate(const TimeDrivenMovement& tdm, float initValue, float stepPerS)
		: PerFrameUpdate(tdm, initValue)
	{
		setStepPerS(stepPerS);
	}
};

class WahwahSinUpdate : private PerFrameUpdate<float>
{
	float initValue;
	float range;
public:
	WahwahSinUpdate(const TimeDrivenMovement& tdm, float initValue, float range, float stepDegPerS)
		: PerFrameUpdate(tdm, 0.0f)
		, initValue(initValue)
		, range(range)
	{
		setStepPerS(glm::radians(stepDegPerS));
	}

	float getVal()
	{
		return initValue + range * glm::sin(PerFrameUpdate::getVal());
	}
};

} // namespace dj