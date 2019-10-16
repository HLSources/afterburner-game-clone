#include <cstdarg>
#include <memory>
#include "logInterface_server.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"

// Ick. Engine functions don't take const char* pointers...

void LogInterface_Server::Debug(const CUtlString& message)
{
	ALERT(at_aiconsole, const_cast<char*>(message.String()));
}

void LogInterface_Server::Message(const CUtlString& message)
{
	ALERT(at_console, const_cast<char*>(message.String()));
}

void LogInterface_Server::Warning(const CUtlString& message)
{
	ALERT(at_warning, const_cast<char*>(message.String()));
}

void LogInterface_Server::Error(const CUtlString& message)
{
	ALERT(at_error, const_cast<char*>(message.String()));
}

void LogInterface_Server::DebugF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	ALERT(at_aiconsole, message.get());
}

void LogInterface_Server::MessageF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	ALERT(at_console, message.get());
}

void LogInterface_Server::WarningF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	ALERT(at_warning, message.get());
}

void LogInterface_Server::ErrorF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	ALERT(at_error, message.get());
}
