#pragma once
#include "ILogger.h"
#include "LogEvents.h"
#include "Utils/Name.h"
#include <format>

namespace dj
{

class LoggingInstance
{
	const Name& instanceName;
	ILogger& logger;

public:
	explicit LoggingInstance(const Name& instanceName, ILogger& logger) noexcept
		: instanceName(instanceName)
		, logger(logger)
	{
	}

	template <typename... Args>
	void log(LogLevel level, unsigned int code, const std::string& fmt, Args... args)
	{
		std::string str = std::vformat(fmt, std::make_format_args(args...));
		logger.log(LogString(level, instanceName.getName(), str, code));
	}

	template <typename... Args>
	void log(LogLevel level, unsigned int code, const char* fmt, Args... args)
	{
		std::string str = std::vformat(fmt, std::make_format_args(args...));
		logger.log(LogString(level, instanceName.getName(), str, code));
	}

	void log(LogLevel level, unsigned int code, const char* msg)
	{
		logger.log(LogMsg(level, instanceName.getName(), msg, code));
	}

	void logOnce(LogLevel level, unsigned int code, const char* msg)
	{
		static LogMsg lastMsg(level, instanceName.getName(), msg, code);
		if (lastMsg.level != level || lastMsg.code != code || lastMsg.msg != msg)
		{
			lastMsg.level = level;
			lastMsg.code = code;
			lastMsg.msg = msg;

			logger.log(LogMsg(level, instanceName.getName(), msg, code));
		}
	}
};

} // namespace dj