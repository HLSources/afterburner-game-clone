#include <string.h>
#include <stdlib.h>
#include "nightfire/enginecommonheaders.h"
#include "nightfire/nfskincollection.h"

static inline qboolean CalcIndex(nfskincollection_t* collection, size_t family, size_t reference, size_t* outIndex)
{
	if ( !collection ||
		 !outIndex ||
		 family >= collection->skinFamilies ||
		 reference >= collection->skinReferences )
	{
		return false;
	}

	*outIndex = (family * collection->skinReferences) + reference;

	// For sanity (should never fail):
	return (*outIndex) < collection->skinCount;
}

void NF_SkinCollection_Init(nfskincollection_t* collection, size_t skinFamilies, size_t skinReferences)
{
	if ( !collection )
	{
		return;
	}

	memset(collection, 0, sizeof(*collection));

	collection->skinFamilies = skinFamilies;
	collection->skinReferences = skinReferences;
	collection->skinCount = collection->skinFamilies * collection->skinReferences;

	if ( collection->skinCount > 0 )
	{
		collection->skins = (int16_t*)malloc(collection->skinCount * sizeof(*collection->skins));
		memset(collection->skins, 0, collection->skinCount * sizeof(*collection->skins));
	}
}

void NF_SkinCollection_Deinit(nfskincollection_t* collection)
{
	if ( !collection )
	{
		return;
	}

	if ( collection->skins )
	{
		free(collection->skins);
	}

	memset(collection, 0, sizeof(*collection));
}

size_t NF_SkinCollection_GetFamilies(nfskincollection_t* collection)
{
	return collection ? collection->skinFamilies : 0;
}

size_t NF_SkinCollection_GetReferences(nfskincollection_t* collection)
{
	return collection ? collection->skinReferences : 0;
}

int16_t NF_SkinCollection_GetSkin(nfskincollection_t* collection, size_t family, size_t reference)
{
	size_t index = 0;

	return CalcIndex(collection, family, reference, &index)
		? collection->skins[index]
		: 0;
}

void NF_SkinCollection_SetSkin(nfskincollection_t* collection, size_t family, size_t reference, int16_t value)
{
	size_t index = 0;

	if ( CalcIndex(collection, family, reference, &index) )
	{
		collection->skins[index] = value;
	}
}
