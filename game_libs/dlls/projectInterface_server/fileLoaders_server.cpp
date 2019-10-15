#include "fileLoaders_server.h"
#include "standard_includes.h"
#include "utlstring.h"
#include "enginecallback.h"

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
