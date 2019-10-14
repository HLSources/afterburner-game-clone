#include "fileLoaders_gamelib.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"

#ifdef CLIENT_DLL
#include "cl_dll.h"
#endif

#ifndef CLIENT_DLL

uint8_t* FileLoader_Server::Load(const CUtlString& filePath, size_t& length)
{
	int localLength = 0;
	byte* fileData = LOAD_FILE_FOR_ME(filePath.String(), &localLength);
	length = localLength;
	return fileData;
}

void FileLoader_Server::Free(uint8_t* data)
{
	if ( data )
	{
		FREE_FILE(data);
	}
}

#else

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

#endif
