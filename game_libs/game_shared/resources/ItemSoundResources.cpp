#include "ItemSoundResources.h"

void CItemSoundResources::Initialise()
{
	InitialiseSound(ItemSoundId::AmmoPickup, "items/ammopickup1.wav");
	InitialiseSound(ItemSoundId::WeaponDrop, "items/weapondrop1.wav");
	InitialiseSound(ItemSoundId::WeaponPickup, "items/gunpickup1.wav");
}
