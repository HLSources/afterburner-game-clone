#include "SoundResources.h"
#include "projectInterface/IProjectInterface.h"

static constexpr const char* const GRENADE_BOUNCE_PATHS[] =
{
	"weapons/grenade_hit1.wav"
};

void CSoundResources::Initialise()
{
	InitialiseSounds(ST_GrenadeBounce, GRENADE_BOUNCE_PATHS);
}

void CSoundResources::InitialiseSounds(SoundType type, const char* const* paths, size_t pathsLength)
{
	CUtlVector<CUtlString>& list = m_SoundPaths[type];
	list.Purge();

	for ( uint32_t index = 0; index < pathsLength; ++index )
	{
		list.AddToTail(CUtlString(paths[index]));
	}
}

size_t CSoundResources::SoundCount(SoundType type) const
{
	return type < ST__Count ? m_SoundPaths[type].Count() : 0;
}

const char* CSoundResources::SoundPath(SoundType type, uint32_t index) const
{
	if ( type >= ST__Count )
	{
		return nullptr;
	}

	const CUtlVector<CUtlString>& list = m_SoundPaths[type];

	if ( index >= list.Count() )
	{
		return nullptr;
	}

	return list[index].String();
}

const char* CSoundResources::GetRandomSoundPath(SoundType type) const
{
	if ( type >= ST__Count )
	{
		return nullptr;
	}

	const CUtlVector<CUtlString>& list = m_SoundPaths[type];

	if ( list.Count() < 1 )
	{
		return nullptr;
	}

	IProjectInterface* ifc = IProjectInterface::ProjectInterfaceImpl();
	int32_t index = ifc->RNG().GetRandomInt(0, list.Count() - 1);
	return list[index].String();
}
