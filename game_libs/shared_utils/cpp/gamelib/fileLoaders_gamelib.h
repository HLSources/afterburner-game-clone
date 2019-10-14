#pragma once

#include "projectInterface/IFileLoader.h"

#ifndef CLIENT_DLL

class FileLoader_Server: public IFileLoader
{
public:
	virtual ~FileLoader_Server() {}

	virtual uint8_t* Load(const CUtlString& filePath, size_t& length) override;
	virtual void Free(uint8_t* data) override;
};

#else

class FileLoader_Client : public IFileLoader
{
public:
	virtual ~FileLoader_Client() {}

	virtual uint8_t* Load(const CUtlString& filePath, size_t& length) override;
	virtual void Free(uint8_t* data) override;
};

#endif
