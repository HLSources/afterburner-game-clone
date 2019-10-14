#pragma once

#include "projectInterface/IProjectInterface.h"
#include "logInterface_gamelib.h"
#include "fileLoaders_gamelib.h"

class ProjectInterface_MainUI : public IProjectInterface
{
public:
	virtual ~ProjectInterface_MainUI() {}

	static IProjectInterface& StaticInstance();

	virtual IFileLoader& FileLoader() override;
	virtual ILogInterface& LogInterface() override;

private:
	FileLoader_Server m_FileLoader;
	LogInterface_Server m_LogInterface;
};
