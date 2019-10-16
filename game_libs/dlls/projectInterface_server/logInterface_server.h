#pragma once

#include "projectInterface/ILogInterface.h"

class LogInterface_Server : public ILogInterface
{
public:
	virtual ~LogInterface_Server() {}

	virtual void Log(ILogInterface::Level level, const CUtlString& message) override;
	virtual void LogF(ILogInterface::Level level, const char* format, ...) override;
};
