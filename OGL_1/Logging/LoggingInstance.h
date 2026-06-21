#pragma once
#include "ILogger.h"
#include "LogEvents.h"
#include "Utils/Name.h"
#include <memory>
#include <format>

namespace dj
{

class LoggingInstance
{
	const Name& instanceName;

protected:
	std::shared_ptr<ILogger> logger;

public:
	explicit LoggingInstance(const Name& instanceName, std::shared_ptr<ILogger> logger) noexcept
		: instanceName(instanceName)
		, logger(logger)
	{
	}

	template <typename CodeType, typename... Args>
	void log(LogLevel level, CodeType code, const std::string& fmt, Args... args)
	{
		std::string str = std::vformat(fmt, std::make_format_args(args...));
		if (logger)
		{
			logger->log(LogString(level, instanceName.getName(), str, static_cast<unsigned int>(code)));
		}
	}

	template <typename CodeType, typename... Args>
	void log(LogLevel level, CodeType code, const char* fmt, Args... args)
	{
		std::string str = std::vformat(fmt, std::make_format_args(args...));
		if (logger)
		{
			logger->log(LogString(level, instanceName.getName(), str, static_cast<unsigned int>(code)));
		}
	}

	template <typename CodeType>
	void log(LogLevel level, CodeType code, const char* msg)
	{
		if (logger)
		{
			logger->log(LogMsg(level, instanceName.getName(), msg, static_cast<unsigned int>(code)));
		}
	}

	template <typename CodeType>
	void logOnce(LogLevel level, CodeType code, const char* msg)
	{
		logOnce<unsigned int>(level, static_cast<unsigned int>(code), msg);
	}

	template <>
	void logOnce<unsigned int>(LogLevel level, unsigned int code, const char* msg)
	{
		static LogMsg lastMsg(level, instanceName.getName(), msg, code);
		if (lastMsg.level != level || lastMsg.code != code || lastMsg.msg != msg)
		{
			lastMsg.level = level;
			lastMsg.code = code;
			lastMsg.msg = msg;

			if (logger)
			{
				logger->log(LogMsg(level, instanceName.getName(), msg, code));
			}
		}
	}
};

} // namespace dj