#pragma once

#include "sound/Soundinstance.h"

namespace ClientSoundInstance
{
	void PlayAmbient(const CSoundInstance& instance);
	void PlayPositional(const CSoundInstance& instance);
	void PlayEventSound(const CSoundInstance& instance, int entIndex = 0);
}
