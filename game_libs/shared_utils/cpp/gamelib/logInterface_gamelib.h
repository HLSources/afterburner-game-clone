#pragma once

#include "projectInterface/ILogInterface.h"

#ifndef CLIENT_DLL

class LogInterface_Server : public ILogInterface
{
public:
	virtual ~LogInterface_Server() {}

	virtual void Debug(const CUtlString& message) override;
	virtual void Message(const CUtlString& message) override;
	virtual void Warning(const CUtlString& message) override;
	virtual void Error(const CUtlString& message) override;
};

#else

class LogInterface_Client : public ILogInterface
{
public:
	virtual ~LogInterface_Client() {}

	virtual void Debug(const CUtlString& message) override;
	virtual void Message(const CUtlString& message) override;
	virtual void Warning(const CUtlString& message) override;
	virtual void Error(const CUtlString& message) override;
};

#endif
