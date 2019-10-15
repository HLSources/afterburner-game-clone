#include "logInterface_client.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"
#include "cl_dll.h"

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
