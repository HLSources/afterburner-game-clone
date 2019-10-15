#include "fileLoaders_client.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"
#include "cl_dll.h"

uint8_t* FileLoader_Client::Load(const CUtlString& filePath, size_t& length)
{
	int localLength = 0;
	byte* fileData = gEngfuncs.COM_LoadFile(filePath.String(), 5, &localLength);
	length = localLength;
	return fileData;
}

void FileLoader_Client::Free(uint8_t* data)
{
	if ( data )
	{
		gEngfuncs.COM_FreeFile(data);
	}
}
