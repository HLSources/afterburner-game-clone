#include "logInterface_gamelib.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"

#ifdef CLIENT_DLL
#include "cl_dll.h"
#endif

#ifndef CLIENT_DLL

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

#else

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
	gEngfuncs.Con_Printf("^3Warning:^7 %s", message.String());
}

void LogInterface_Client::Error(const CUtlString& message)
{
	gEngfuncs.Con_Printf("^1Error:^7 %s", message.String());
}

#endif
