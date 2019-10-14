#pragma once

#include "IFileLoader.h"
#include "ILogInterface.h"

class IProjectInterface
{
public:
	virtual ~IProjectInterface() {}

	// Provided depending on what project we're building:
	static IProjectInterface& ProjectInterfaceImpl();

	virtual IFileLoader& FileLoader() = 0;
	virtual ILogInterface& LogInterface() = 0;
};
