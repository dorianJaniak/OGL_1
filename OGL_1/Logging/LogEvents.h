#pragma once
#include "ILogger.h"
#include <string>

namespace dj
{

struct LogMsg : public virtual ILogger::ILog
{
	const char* msg;

	explicit LogMsg(LogLevel level, const std::string& source, const char* msg, unsigned int code = 0u) noexcept
		: ILogger::ILog(level, source, code)
		, msg(msg)
	{
	}

	virtual void print(std::ostream& str) const
	{
		if (msg)
		{
			str << msg;
		}
	}
};

struct LogString : public virtual ILogger::ILog
{
	std::string str;

	explicit LogString(LogLevel level, const std::string& source, const std::string& str, unsigned int code = 0u)
		: ILogger::ILog(level, source, code)
		, str(str)
	{
	}

	virtual void print(std::ostream& stream) const
	{
		stream << str;
	}
};


} // namespace dj