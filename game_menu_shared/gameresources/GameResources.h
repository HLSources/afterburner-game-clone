#pragma once

#include "filesystemops/FileSystemOps.h"
#include "utlvector.h"
#include "utlstring.h"
#include "SoundResources.h"

class CGameResources
{
public:
	static CGameResources& StaticInstance();

	// Model name should be provided without the .mdl extension.
	static CUtlString MultiplayerModelFullPath(const CUtlString& modelName);

	void Initialise();

	const CUtlVector<CUtlString>& MultiplayerModelList() const;

	CSoundResources& SoundResources();
	const CSoundResources& SoundResources() const;

private:
	void InitMultiplayerModelList();

	CUtlVector<CUtlString> m_MultiplayerModelList;
	CSoundResources m_SoundResources;
};
