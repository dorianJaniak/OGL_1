#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <iosfwd>

namespace dj
{

enum class LogLevel : uint8_t
{
	Debug,
	Info,
	Warning,
	Error,
	Critical,
};

struct ILog
{
	LogLevel level;
	unsigned int code;
	std::string source;

	ILog(LogLevel level, const std::string& source, unsigned int code = 0u) noexcept;
	ILog(LogLevel level, std::string_view source, unsigned int code = 0u) noexcept;

	virtual void print(std::ostream& str) const = 0;
};

class ILogger
{
public:
	virtual void log(const ILog& event) = 0;
};

std::ostream& operator<<(std::ostream& str, const ILog& event);

} // namespace dj