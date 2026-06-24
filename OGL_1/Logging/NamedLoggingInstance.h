#pragma once

#include "Utils/Name.h"
#include "Logging/LoggingInstance.h"

namespace dj
{

class NamedLoggingInstance : public Name, protected LoggingInstance
{
public:
	explicit NamedLoggingInstance(std::shared_ptr<ILogger> logger, const std::string& name = "") noexcept
		: Name(name)
		, LoggingInstance(*this, logger)
	{ }

	NamedLoggingInstance(const NamedLoggingInstance& nli) noexcept
		: Name(nli.getName())
		, LoggingInstance(*this, nli.logger)
	{ }

	NamedLoggingInstance(NamedLoggingInstance&& nli) noexcept
		: Name(nli.getName())
		, LoggingInstance(*this, nli.logger)
	{
		nli.setName("");
	}

	NamedLoggingInstance& operator=(const NamedLoggingInstance&) = delete;
	NamedLoggingInstance& operator=(NamedLoggingInstance&&) = delete;
};

} // namespace dj