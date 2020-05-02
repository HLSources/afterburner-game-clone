#pragma once

#include "BaseSoundResourceCollection.h"

enum class ItemSoundId
{
	AmmoPickup = 0,
	WeaponPickup,
	WeaponDrop,
	Count
};

class CItemSoundResources : public CBaseSoundResourceCollection<ItemSoundId, static_cast<size_t>(ItemSoundId::Count)>
{
public:
	virtual void Initialise() override;
};
