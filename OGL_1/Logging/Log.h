#pragma once
#include "ILogger.h"
#include <string>
#include <format>
#include <string_view>

namespace dj
{

struct Log : public ILog
{
	std::string description;

	Log(LogLevel level, std::string_view source, std::string_view description) noexcept
		: ILog(level, source, 0u)
		, description(description)
	{
	}

	template <typename CodeType, typename... Args>
	Log(LogLevel level, CodeType code, std::string_view source, std::string_view fmt, Args... args) noexcept
		: ILog(level, source, static_cast<unsigned int>(code))
	{
		description = std::vformat(fmt, std::make_format_args(args...));
	}

	virtual void print(std::ostream& stream) const;
};

} // namespace dj