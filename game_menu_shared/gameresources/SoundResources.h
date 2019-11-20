#pragma once

#include <cstddef>
#include "utlvector.h"
#include "utlstring.h"

class CSoundResources
{
public:
	enum SoundType
	{
		ST_GrenadeBounce = 0,

		ST__Count
	};

	void Initialise();

	size_t SoundCount(SoundType type) const;
	const char* SoundPath(SoundType type, uint32_t index) const;
	const char* GetRandomSoundPath(SoundType type) const;

private:
	template<size_t N>
	inline void InitialiseSounds(SoundType type, const char* const (&array)[N])
	{
		InitialiseSounds(type, array, N);
	}

	void InitialiseSounds(SoundType type, const char* const* paths, size_t pathsLength);

	CUtlVector<CUtlString> m_SoundPaths[ST__Count];
};
