#include <stdlib.h>
#include <stdint.h>
#include "nightfire/allocutils.h"

void NF_AllocUtils_AllocateItems(void** outArray,
								 size_t* outCount,
								 size_t newCount,
								 size_t itemSize,
								 qboolean setToZero)
{
	if ( !outArray || !outCount )
	{
		return;
	}

	*outCount = newCount;

	if ( *outCount > 0 )
	{
		*outArray = realloc(*outArray, *outCount * itemSize);

		if ( setToZero )
		{
			memset(*outArray, 0, *outCount * itemSize);
		}
	}
	else if ( *outArray )
	{
		free(*outArray);
		*outArray = NULL;
	}
}

void NF_AllocUtils_AllocateComplexItems(void** outArray,
										size_t* outCount,
										size_t newCount,
										size_t itemSize,
										void (*initFunc)(void*),
										void (*deinitFunc)(void*))
{
	size_t oldCount = 0;

	if ( !outArray || !outCount )
	{
		return;
	}

	oldCount = *outCount;

	if ( deinitFunc )
	{
		// Run if new count is less than old count:
		for ( size_t index = newCount; index < oldCount; ++index )
		{
			deinitFunc(((uint8_t*)*outArray) + (index * itemSize));
		}
	}

	NF_AllocUtils_AllocateItems(outArray, outCount, newCount, itemSize, false);

	if ( initFunc )
	{
		// Run if new count is greater than old count:
		for ( size_t index = oldCount; index < *outCount; ++index )
		{
			initFunc(((uint8_t*)*outArray) + (index * itemSize));
		}
	}
}
