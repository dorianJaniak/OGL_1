#include "TimeDrivenMovement.h"
using namespace dj;

const std::chrono::steady_clock::time_point TimeDrivenMovement::appStart = std::chrono::steady_clock::now();

TimeDrivenMovement::TimeDrivenMovement()
	: startTime(appStart)
	, endTime(appStart)
{
}

void TimeDrivenMovement::nextFrame()
{
	startTime = endTime;
	endTime = std::chrono::steady_clock::now();
}

std::chrono::milliseconds TimeDrivenMovement::getFrameDiffMs() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
}

std::chrono::milliseconds TimeDrivenMovement::getAppDiffMs() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - appStart);
}

std::chrono::seconds TimeDrivenMovement::getAppTimeDiffS() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - appStart);
}