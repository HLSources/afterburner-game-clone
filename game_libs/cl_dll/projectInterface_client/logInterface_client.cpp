#include <cstdarg>
#include <memory>
#include "logInterface_client.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"
#include "cl_dll.h"

#define PREFIX_WARNING "^3Warning:^7 "
#define PREFIX_ERROR "^1Error:^7 "

void LogInterface_Client::Debug(const CUtlString& message)
{
	gEngfuncs.Con_DPrintf(message.String());
}

void LogInterface_Client::Message(const CUtlString& message)
{
	gEngfuncs.Con_Printf(message.String());
}

void LogInterface_Client::Warning(const CUtlString& message)
{
	gEngfuncs.Con_Printf(PREFIX_WARNING "%s", message.String());
}

void LogInterface_Client::Error(const CUtlString& message)
{
	gEngfuncs.Con_Printf(PREFIX_ERROR "%s", message.String());
}

void LogInterface_Client::DebugF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	gEngfuncs.Con_DPrintf(message.get());
}

void LogInterface_Client::MessageF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	gEngfuncs.Con_Printf(message.get());
}

void LogInterface_Client::WarningF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	gEngfuncs.Con_Printf(PREFIX_WARNING "%s", message.get());
}

void LogInterface_Client::ErrorF(const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	gEngfuncs.Con_Printf(PREFIX_ERROR "%s", message.get());
}
