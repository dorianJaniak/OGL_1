#pragma once
#include <chrono>

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

} // namespace dj