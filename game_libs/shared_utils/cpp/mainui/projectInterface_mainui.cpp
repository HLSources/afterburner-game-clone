#include "projectInterface_mainui.h"

IProjectInterface&  IProjectInterface::ProjectInterfaceImpl()
{
	return ProjectInterface_MainUI::StaticInstance();
}

IProjectInterface& ProjectInterface_MainUI::StaticInstance()
{
	static ProjectInterface_MainUI instance;
	return instance;
}

IFileLoader& ProjectInterface_MainUI::FileLoader()
{
	return m_FileLoader;
}

ILogInterface& ProjectInterface_MainUI::LogInterface()
{
	return m_LogInterface;
}
