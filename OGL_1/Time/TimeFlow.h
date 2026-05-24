#pragma once
#include <chrono>

namespace dj
{

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