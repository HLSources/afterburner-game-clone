#include <stdlib.h>
#include <string.h>
#include "nightfire/nfanimationcollection.h"
#include "xash3d_types.h"

static inline qboolean CalcIndex(nfanimationcollection_t* collection, size_t blend, size_t bone, size_t component, size_t* outIndex)
{
	if ( !collection ||
		 !outIndex ||
		 blend >= collection->blendCount ||
		 bone >= collection->boneCount ||
		 component >= NF_ANIMATIONCOLLECTION_AXES )
	{
		return false;
	}

	// For each blend, there are boneCount bones.
	// For each bone, there are NF_ANIMATIONCOLLECTION_AXES axes.
	// This means that incrementing the bone index by 1 increments
	// the linear index by NF_ANIMATIONCOLLECTION_AXES, and
	// incrementing the blend by 1 increments the linear index
	// by (boneCount * NF_ANIMATIONCOLLECTION_AXES).
	// This evaluates to:

	*outIndex = (blend * collection->boneCount * NF_ANIMATIONCOLLECTION_AXES) +
				(bone * NF_ANIMATIONCOLLECTION_AXES) +
				component;

	// For sanity (should never fail):
	return (*outIndex) < collection->bucketCount;
}

void NF_AnimationCollection_Init(nfanimationcollection_t* collection, size_t blendCount, size_t boneCount)
{
	if ( !collection )
	{
		return;
	}

	memset(collection, 0, sizeof(*collection));

	collection->blendCount = blendCount;
	collection->boneCount = boneCount;
	collection->bucketCount = collection->blendCount * collection->boneCount * NF_ANIMATIONCOLLECTION_AXES;

	if ( collection->bucketCount > 0 )
	{
		collection->buckets = (nfanimationbucket_t*)malloc(collection->bucketCount * sizeof(*collection->buckets));

		for ( size_t index = 0; index < collection->bucketCount; ++index )
		{
			NF_AnimationBucket_Init(&collection->buckets[index]);
		}
	}
}

void NF_AnimationCollection_Deinit(nfanimationcollection_t* collection)
{
	if ( !collection )
	{
		return;
	}

	for ( size_t index = 0; index < collection->bucketCount; ++index )
	{
		NF_AnimationBucket_Deinit(&collection->buckets[index]);
	}

	if ( collection->buckets )
	{
		free(collection->buckets);
		collection->buckets = NULL;
	}

	memset(collection, 0, sizeof(*collection));
}

size_t NF_AnimationCollection_GetBlendCount(nfanimationcollection_t* collection)
{
	return collection ? collection->blendCount : 0;
}

size_t NF_AnimationCollection_GetBoneCount(nfanimationcollection_t* collection)
{
	return collection ? collection->boneCount : 0;
}

nfanimationbucket_t* NF_AnimationCollection_GetBucket(nfanimationcollection_t* collection, size_t blend, size_t bone, size_t component)
{
	size_t index = 0;

	return CalcIndex(collection, blend, bone, component, &index)
		? &collection->buckets[index]
		: NULL;
}
