#include "ILogger.h"
#include <iostream>
using namespace dj;

ILog::ILog(LogLevel level, const std::string& source, unsigned int code) noexcept
	: level(level)
	, code(code)
	, source(source)
{
}

ILog::ILog(LogLevel level, std::string_view source, unsigned int code) noexcept
	: level(level)
	, code(code)
	, source(source)
{
}

std::ostream& dj::operator<<(std::ostream& str, const ILog& event)
{
	event.print(str);
	return str;
}