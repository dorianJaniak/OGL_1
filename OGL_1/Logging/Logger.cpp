#include "Logger.h"
#include <cassert>
#include <iostream>
using namespace dj;

Logger::Logger()
	: ILogger()
	, enabledLevels("11111")
{
}

void Logger::enableLogging(LogLevel level, bool active)
{
	assert(static_cast<std::size_t>(level) < enabledLevels.size() && "Logger could not handle Log Level - enabledLevels bitset too small");
	enabledLevels[static_cast<std::size_t>(level)] = active;
}

void Logger::log(const ILog& event)
{
	std::size_t logLevel = static_cast<std::size_t>(event.level);
	assert(logLevel < enabledLevels.size() && "Logger could not handle Log Level - enabledLevels bitset too small");

	if (!enabledLevels[logLevel])
	{
		return;
	}

	const char* prefix = getPrefix(event.level);
	std::ostream& stream = getStream(event.level);

	stream << prefix << ' ';
	if (event.code != 0u)
	{
		stream << event.code << ':' ;
	}
	if (!event.source.empty())
	{
		stream << event.source << ':';
	}

	stream << ' ' << event << std::endl;
}

const char* Logger::getPrefix(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Debug: return "D-";
	case LogLevel::Info: return "I-";
	case LogLevel::Warning: return "W-";
	case LogLevel::Error: return "ERR-";
	case LogLevel::Critical: return "FAIL-";
	}

	return "[-]";
}

std::ostream& Logger::getStream(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Debug:
	case LogLevel::Info:
		return std::cout;
	case LogLevel::Warning:
	case LogLevel::Error:
	case LogLevel::Critical:
		return std::cerr;
	}

	return std::cout;
}