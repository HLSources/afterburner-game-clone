#pragma once

#include "projectInterface/ILogInterface.h"

class LogInterface_Server : public ILogInterface
{
public:
	virtual ~LogInterface_Server() {}

	virtual void Debug(const CUtlString& message) override;
	virtual void Message(const CUtlString& message) override;
	virtual void Warning(const CUtlString& message) override;
	virtual void Error(const CUtlString& message) override;

	virtual void DebugF(const char* format, ...) override;
	virtual void MessageF(const char* format, ...) override;
	virtual void WarningF(const char* format, ...) override;
	virtual void ErrorF(const char* format, ...) override;
};
