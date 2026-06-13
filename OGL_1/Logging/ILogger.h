#pragma once
#include <cstdint>
#include <iostream>
#include <string>

namespace dj
{

enum class LogLevel : uint8_t
{
	Debug,
	Info,
	Warning,
	Error,
	Critical
};

class ILogger
{
public:
	struct ILog
	{
		LogLevel level;
		unsigned int code;
		std::string source;

		explicit ILog(LogLevel level, const std::string& source, unsigned int code = 0u) noexcept;
		virtual void print(std::ostream& str) const = 0;
	};

	virtual void log(const ILog& event) = 0;
};

std::ostream& operator<<(std::ostream& str, const ILogger::ILog& event);

} // namespace dj