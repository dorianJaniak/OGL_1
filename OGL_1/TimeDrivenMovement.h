#pragma once
#include <chrono>
#include <glm.hpp>

namespace dj
{

class TimeDrivenMovement 
{
	static const std::chrono::steady_clock::time_point appStart;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point endTime;
public:
	TimeDrivenMovement();

	void nextFrame();

	std::chrono::milliseconds getFrameDiffMs() const;
	std::chrono::milliseconds getAppDiffMs() const;
	std::chrono::seconds getAppTimeDiffS() const;
};

template <class T>
class PerFrameUpdate
{
	const TimeDrivenMovement& tdm;
	T value;
	T stepMs;
public:
	PerFrameUpdate(const TimeDrivenMovement &tdm, T initVaue)
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

template <class T>
class TimeFlow
{
	std::chrono::steady_clock::time_point fromTime;
	T period;
	bool expired;

public:
	TimeFlow(T period)
		: fromTime(std::chrono::steady_clock::now())
		, period(period)
		, expired(false)
	{
	}

	void start()
	{
		expired = false;
		fromTime = std::chrono::steady_clock::now();
	}

	void startFromLastPeriod()
	{
		fromTime = fromTime + expirationCount() * period;
		expired = false;
	}

	bool isExpired()
	{
		if (!expired)
		{
			expirationCount();
		}

		return expired;
	}

	T timePassed() const
	{
		const auto now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<T>(now - fromTime);
	}

	unsigned int expirationCount()
	{
		unsigned int expirationCount = static_cast<unsigned int>(timePassed() / period);
		expired = (expirationCount > 0u);
		return expirationCount;
	}
};

} // namespace dj