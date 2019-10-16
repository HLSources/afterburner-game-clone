#include <cstdarg>
#include <memory>
#include "logInterface_client.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"
#include "cl_dll.h"

#define PREFIX_WARNING "^3Warning:^7 "
#define PREFIX_ERROR "^1Error:^7 "

void LogInterface_Client::Log(ILogInterface::Level level, const CUtlString& message)
{
	switch ( level )
	{
		case ILogInterface::Level::Debug:
		{
			gEngfuncs.Con_DPrintf(message.String());
			break;
		}

		case ILogInterface::Level::Warning:
		{
			gEngfuncs.Con_Printf(PREFIX_WARNING "%s", message.String());
			break;
		}

		case ILogInterface::Level::Error:
		{
			gEngfuncs.Con_Printf(PREFIX_ERROR "%s", message.String());
			break;
		}

		default:
		{
			gEngfuncs.Con_Printf(message.String());
			break;
		}
	}
}

void LogInterface_Client::LogF(ILogInterface::Level level, const char* format, ...)
{
	std::unique_ptr<char> message(new char[MAX_MESSAGE_LENGTH]);
	va_list args;

	va_start(args, format);
	V_vsnprintf(message.get(), MAX_MESSAGE_LENGTH, format, args);
	va_end(args);

	switch ( level )
	{
		case ILogInterface::Level::Debug:
		{
			gEngfuncs.Con_DPrintf(message.get());
			break;
		}

		case ILogInterface::Level::Warning:
		{
			gEngfuncs.Con_Printf(PREFIX_WARNING "%s", message.get());
			break;
		}

		case ILogInterface::Level::Error:
		{
			gEngfuncs.Con_Printf(PREFIX_ERROR "%s", message.get());
			break;
		}

		default:
		{
			gEngfuncs.Con_Printf(message.get());
			break;
		}
	}
}
