#pragma once

#include "BaseSoundResourceCollection.h"

enum class ItemSoundId
{
	GrenadeBounce = 0,
	Count
};

class CItemSoundResources : public CBaseSoundResourceCollection<ItemSoundId, static_cast<size_t>(ItemSoundId::Count)>
{
public:
	virtual void Initialise() override;
};
