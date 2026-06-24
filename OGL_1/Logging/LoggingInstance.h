#pragma once
#include "ILogger.h"
#include "Log.h"
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
	void log(LogLevel level, CodeType code, std::string_view fmt, Args... args)
	{
		if (logger)
		{
			logger->log(Log(level, code, instanceName.getName(), fmt, std::forward<Args>(args)...));
		}
	}

	template <typename CodeType>
	void log(LogLevel level, CodeType code, std::string_view description)
	{
		if (logger)
		{
			logger->log(Log(level, instanceName.getName(), description, code));
		}
	}

	template <typename CodeType>
	void logOnce(LogLevel level, CodeType code, std::string_view description)
	{
		static Log lastLog(level, instanceName.getName(), description, code);
		if (lastLog.level != level || lastLog.code != code || lastLog.description != description)
		{
			lastLog.level = level;
			lastLog.code = code;
			lastLog.description = description;

			if (logger)
			{
				logger->log(Log(level, instanceName.getName(), description, code));
			}
		}
	}
};

} // namespace dj