#pragma once

#include <cstdint>
#include <cstddef>
#include "utlvector.h"
#include "utlstring.h"
#include "projectInterface/IProjectInterface.h"

template<typename ENUM, size_t N>
class CBaseSoundResourceCollection
{
public:
	using ResourceId = ENUM;
	static constexpr size_t NUM_ENTRIES = N;

	virtual void Initialise() = 0;

	inline size_t SoundCount(ResourceId id) const
	{
		const uint32_t intId = static_cast<uint32_t>(id);
		return intId < NUM_ENTRIES ? m_SoundPaths[intId].Count() : 0;
	}

	const char* SoundPath(ResourceId id, uint32_t index = 0) const
	{
		const uint32_t intId = static_cast<uint32_t>(id);

		if ( intId >= NUM_ENTRIES )
		{
			return nullptr;
		}

		const CUtlVector<CUtlString>& list = m_SoundPaths[intId];

		if ( index >= list.Count() )
		{
			return nullptr;
		}

		return list[index].String();
	}

	const char* GetRandomSoundPath(ResourceId id) const
	{
		const uint32_t intId = static_cast<uint32_t>(id);

		if ( intId >= NUM_ENTRIES )
		{
			return nullptr;
		}

		const CUtlVector<CUtlString>& list = m_SoundPaths[intId];

		if ( list.Count() < 1 )
		{
			return nullptr;
		}

		IProjectInterface* ifc = IProjectInterface::ProjectInterfaceImpl();
		int32_t index = ifc->RNG().GetRandomInt(0, list.Count() - 1);
		return list[index].String();
	}

protected:
	template<size_t LEN>
	inline void InitialiseSounds(ResourceId id, const char* const (&array)[LEN])
	{
		InitialiseSounds(id, array, LEN);
	}

	inline void InitialiseSound(ResourceId id, const char* const path)
	{
		InitialiseSounds(id, &path, 1);
	}

	void InitialiseSounds(ResourceId id, const char* const* paths, size_t pathsLength)
	{
		const uint32_t intId = static_cast<uint32_t>(id);

		CUtlVector<CUtlString>& list = m_SoundPaths[intId];
		list.Purge();

		for ( uint32_t index = 0; index < pathsLength; ++index )
		{
			list.AddToTail(CUtlString(paths[index]));
		}
	}

private:
	CUtlVector<CUtlString> m_SoundPaths[NUM_ENTRIES];
};
