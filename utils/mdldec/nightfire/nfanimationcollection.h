#pragma once

#include "nightfire/nfanimationbucket.h"

#define NF_ANIMATIONCOLLECTION_AXES 6

typedef struct
{
	size_t blendCount;
	size_t boneCount;
	size_t bucketCount;
	nfanimationbucket_t* buckets;
} nfanimationcollection_t;

void NF_AnimationCollection_Init(nfanimationcollection_t* collection, size_t blendCount, size_t boneCount);
void NF_AnimationCollection_Deinit(nfanimationcollection_t* collection);

size_t NF_AnimationCollection_GetBlendCount(nfanimationcollection_t* collection);
size_t NF_AnimationCollection_GetBoneCount(nfanimationcollection_t* collection);
nfanimationbucket_t* NF_AnimationCollection_GetBucket(nfanimationcollection_t* collection, size_t blend, size_t bone, size_t component);
