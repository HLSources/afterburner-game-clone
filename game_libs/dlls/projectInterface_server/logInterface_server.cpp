#include <cstdarg>
#include <memory>
#include "logInterface_server.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"

void LogInterface_Server::Log(ILogInterface::Level level, const CUtlString& message)
{
	switch ( level )
	{
		case ILogInterface::Level::Debug:
		{
			ALERT(at_aiconsole, const_cast<char*>(message.String()));
			break;
		}

		case ILogInterface::Level::Warning:
		{
			ALERT(at_warning, const_cast<char*>(message.String()));
			break;
		}

		case ILogInterface::Level::Error:
		{
			ALERT(at_error, const_cast<char*>(message.String()));
			break;
		}

		default:
		{
			ALERT(at_console, const_cast<char*>(message.String()));
			break;
		}
	}
}

void LogInterface_Server::LogF(ILogInterface::Level level, const char* format, ...)
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
			ALERT(at_aiconsole, message.get());
			break;
		}

		case ILogInterface::Level::Warning:
		{
			ALERT(at_warning, message.get());
			break;
		}

		case ILogInterface::Level::Error:
		{
			ALERT(at_error, message.get());
			break;
		}

		default:
		{
			ALERT(at_console, message.get());
			break;
		}
	}
}
