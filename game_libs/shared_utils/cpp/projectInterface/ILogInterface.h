#pragma once

class CUtlString;

class ILogInterface
{
public:
	virtual ~ILogInterface() {}

	virtual void Debug(const CUtlString& message) = 0;
	virtual void Message(const CUtlString& message) = 0;
	virtual void Warning(const CUtlString& message) = 0;
	virtual void Error(const CUtlString& message) = 0;
};
