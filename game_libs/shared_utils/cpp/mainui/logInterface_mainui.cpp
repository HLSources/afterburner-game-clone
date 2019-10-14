#include "logInterface_mainui.h"
#include "enginecallback_menu.h"
#include "utlstring.h"

void LogInterface_MainUI::Debug(const CUtlString& message)
{
	Con_DPrintf(message.String());
}

void LogInterface_MainUI::Message(const CUtlString& message)
{
	Con_Printf(message.String());
}

void LogInterface_MainUI::Warning(const CUtlString& message)
{
	Con_Printf("^3Warning:^7 %s", message.String());
}

void LogInterface_MainUI::Error(const CUtlString& message)
{
	Con_Printf("^1Error:^7 %s", message.String());
}
