#include <string.h>
#include <stdlib.h>
#include "nightfire/nfanimationbucket.h"

void NF_AnimationBucket_Init(nfanimationbucket_t* bucket)
{
	if ( !bucket )
	{
		return;
	}

	memset(bucket, 0, sizeof(*bucket));
}

void NF_AnimationBucket_Deinit(nfanimationbucket_t* bucket)
{
	if ( !bucket )
	{
		return;
	}

	if ( bucket->values )
	{
		free(bucket->values);
		bucket->values = NULL;
	}

	memset(bucket, 0, sizeof(*bucket));
}

size_t NF_AnimationBucket_GetValueCount(nfanimationbucket_t* bucket)
{
	return bucket ? bucket->count : 0;
}

void NF_AnimationBucket_Resize(nfanimationbucket_t* bucket, size_t newCount)
{
	if ( !bucket || bucket->count == newCount )
	{
		return;
	}

	bucket->count = newCount;

	if ( bucket->count > 0 )
	{
		bucket->values = (int16_t*)realloc(bucket->values, bucket->count * sizeof(*bucket->values));
	}
	else if ( bucket->values )
	{
		free(bucket->values);
		bucket->values = NULL;
	}
}
