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
