#include "ILogger.h"
using namespace dj;

ILogger::ILog::ILog(LogLevel level, const std::string& source, unsigned int code) noexcept
	: level(level)
	, code(code)
	, source(source)
{
}

std::ostream& dj::operator<<(std::ostream& str, const ILogger::ILog& event)
{
	event.print(str);
	return str;
}