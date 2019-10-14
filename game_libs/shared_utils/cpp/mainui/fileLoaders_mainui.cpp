#include "fileLoaders_mainui.h"
#include "enginecallback_menu.h"
#include "utlstring.h"

uint8_t* FileLoader_MainUI::Load(const CUtlString& filePath, size_t& length)
{
	int localLength = 0;
	byte* fileData = EngFuncs::COM_LoadFile(filePath.String(), &localLength);
	length = localLength;
	return fileData;
}

void FileLoader_MainUI::Free(uint8_t* data)
{
	if ( data )
	{
		EngFuncs::COM_FreeFile(data);
	}
}
