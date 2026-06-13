#pragma once
#include "ILogger.h"
#include <bitset>

namespace dj
{

class Logger : public ILogger
{
	std::bitset<5u> enabledLevels;

public:
	Logger();

	void enableLogging(LogLevel level, bool active);
	virtual void log(const ILogger::ILog& event);

private:
	static const char* getPrefix(LogLevel level);
	static std::ostream& getStream(LogLevel level);
};

} // namespace dj