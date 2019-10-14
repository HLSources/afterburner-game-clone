#pragma once

#include "projectInterface/ILogInterface.h"

class LogInterface_MainUI : public ILogInterface
{
public:
	virtual ~LogInterface_MainUI() {}

	virtual void Debug(const CUtlString& message) override;
	virtual void Message(const CUtlString& message) override;
	virtual void Warning(const CUtlString& message) override;
	virtual void Error(const CUtlString& message) override;
};
