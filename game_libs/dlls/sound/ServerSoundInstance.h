#pragma once

#include "sound/Soundinstance.h"

typedef struct edict_s edict_t;

namespace ServerSoundInstance
{
	void PlayAmbient(const CSoundInstance& instance, edict_t* entity = nullptr);
	void PlayDynamic(const CSoundInstance& instance, edict_t* entity = nullptr);
	void StopDynamic(const CSoundInstance& instance, edict_t* entity = nullptr);
};
