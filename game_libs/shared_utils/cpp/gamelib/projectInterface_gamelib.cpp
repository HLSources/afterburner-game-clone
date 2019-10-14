#include "projectInterface_gamelib.h"

IProjectInterface&  IProjectInterface::ProjectInterfaceImpl()
{
#ifdef CLIENT_DLL
	return ProjectInterface_Client::StaticInstance();
#else
	return ProjectInterface_Server::StaticInstance();
#endif
}

#ifndef CLIENT_DLL

IProjectInterface& ProjectInterface_Server::StaticInstance()
{
	static ProjectInterface_Server instance;
	return instance;
}

IFileLoader& ProjectInterface_Server::FileLoader()
{
	return m_FileLoader;
}

ILogInterface& ProjectInterface_Server::LogInterface()
{
	return m_LogInterface;
}

#else

IProjectInterface& ProjectInterface_Client::StaticInstance()
{
	static ProjectInterface_Client instance;
	return instance;
}

IFileLoader& ProjectInterface_Client::FileLoader()
{
	return m_FileLoader;
}

ILogInterface& ProjectInterface_Client::LogInterface()
{
	return m_LogInterface;
}

#endif
