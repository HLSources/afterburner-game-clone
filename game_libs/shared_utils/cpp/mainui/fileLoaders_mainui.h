#pragma once

#include "projectInterface/IFileLoader.h"

class FileLoader_MainUI: public IFileLoader
{
public:
	virtual ~FileLoader_MainUI() {}

	virtual uint8_t* Load(const CUtlString& filePath, size_t& length) override;
	virtual void Free(uint8_t* data) override;
};
