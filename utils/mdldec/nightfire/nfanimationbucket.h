#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct
{
	size_t count;
	int16_t* values;
} nfanimationbucket_t;

void NF_AnimationBucket_Init(nfanimationbucket_t* bucket);
void NF_AnimationBucket_Deinit(nfanimationbucket_t* bucket);

static void NF_AnimationBucket_InitGeneric(void* bucket)
{
	NF_AnimationBucket_Init((nfanimationbucket_t*)bucket);
}

static void NF_AnimationBucket_DeinitGeneric(void* bucket)
{
	NF_AnimationBucket_Deinit((nfanimationbucket_t*)bucket);
}

size_t NF_AnimationBucket_GetValueCount(nfanimationbucket_t* bucket);
void NF_AnimationBucket_Resize(nfanimationbucket_t* bucket, size_t newCount);

static inline int16_t NF_AnimationBucket_GetValue(nfanimationbucket_t* bucket, size_t index)
{
	return (bucket && bucket->values && index < bucket->count)
		? bucket->values[index]
		: 0;
}

static inline void NF_AnimationBucket_SetValue(nfanimationbucket_t* bucket, size_t index, int16_t value)
{
	if ( bucket && bucket->values && index < bucket->count )
	{
		bucket->values[index] = value;
	}
}
