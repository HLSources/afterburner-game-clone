#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct
{
	size_t skinFamilies;
	size_t skinReferences;
	size_t skinCount;
	int16_t* skins;
} nfskincollection_t;

void NF_SkinCollection_Init(nfskincollection_t* collection, size_t skinFamilies, size_t skinReferences);
void NF_SkinCollection_Deinit(nfskincollection_t* collection);

size_t NF_SkinCollection_GetFamilies(nfskincollection_t* collection);
size_t NF_SkinCollection_GetReferences(nfskincollection_t* collection);
int16_t NF_SkinCollection_GetSkin(nfskincollection_t* collection, size_t family, size_t reference);
void NF_SkinCollection_SetSkin(nfskincollection_t* collection, size_t family, size_t reference, int16_t value);
