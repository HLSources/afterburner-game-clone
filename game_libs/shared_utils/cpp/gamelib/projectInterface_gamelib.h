#pragma once

#include "projectInterface/IProjectInterface.h"
#include "logInterface_gamelib.h"
#include "fileLoaders_gamelib.h"

#ifndef CLIENT_DLL

class ProjectInterface_Server : public IProjectInterface
{
public:
	virtual ~ProjectInterface_Server() {}

	static IProjectInterface& StaticInstance();

	virtual IFileLoader& FileLoader() override;
	virtual ILogInterface& LogInterface() override;

private:
	FileLoader_Server m_FileLoader;
	LogInterface_Server m_LogInterface;
};

#else

class ProjectInterface_Client : public IProjectInterface
{
public:
	virtual ~ProjectInterface_Client() {}

	static IProjectInterface& StaticInstance();

	virtual IFileLoader& FileLoader() override;
	virtual ILogInterface& LogInterface() override;

private:
	FileLoader_Client m_FileLoader;
	LogInterface_Client m_LogInterface;
};

#endif
