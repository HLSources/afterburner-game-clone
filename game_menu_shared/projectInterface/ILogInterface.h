#pragma once

class CUtlString;

class ILogInterface
{
public:
	static constexpr size_t MAX_MESSAGE_LENGTH = 8192;

	virtual ~ILogInterface() {}

	virtual void Debug(const CUtlString& message) = 0;
	virtual void Message(const CUtlString& message) = 0;
	virtual void Warning(const CUtlString& message) = 0;
	virtual void Error(const CUtlString& message) = 0;

	virtual void DebugF(const char* format, ...) = 0;
	virtual void MessageF(const char* format, ...) = 0;
	virtual void WarningF(const char* format, ...) = 0;
	virtual void ErrorF(const char* format, ...) = 0;
};
