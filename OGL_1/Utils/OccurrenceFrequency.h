#pragma once
#include "TimeDrivenMovement.h"
#include <functional>
#include <type_traits>

namespace dj
{

template <class PeriodType, class CounterType = unsigned int>
class OccurrenceFrequency
{
	static_assert(std::is_unsigned_v<CounterType>);

	using ReportCallback = std::function<void(CounterType, PeriodType)>;

	CounterType counter;
	TimeFlow<PeriodType> timer;
	ReportCallback reportCallback;
public:
	OccurrenceFrequency(PeriodType period)
		: counter{}
		, timer(period)
		, reportCallback{}
	{
	}

	void start()
	{
		timer.start();
	}

	void tick()
	{
		if (reportCallback && timer.isExpired())
		{
			PeriodType timePassed = timer.timePassed();
			timer.startFromLastPeriod();
			reportCallback(counter, timePassed);
			resetCounter();
		}
	}

	void increment()
	{
		++counter;
		tick();
	}

	void resetCounter()
	{
		counter = CounterType{};
	}

	CounterType getCount() const
	{
		return counter;
	}

	void setReportCallback(ReportCallback callback)
	{
		reportCallback = callback;
	}
};

} // namespace dj