#pragma once

#include "xash3d_types.h"

#define NF_ARRAYSIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define NF_MEMBER_ARRAYSIZE(type, member) NF_ARRAYSIZE(((type*)0)->member)

void NF_AllocUtils_AllocateItems(void** outArray,
								 size_t* outCount,
								 size_t newCount,
								 size_t itemSize,
								 qboolean setToZero);

void NF_AllocUtils_AllocateComplexItems(void** outArray,
										size_t* outCount,
										size_t newCount,
										size_t itemSize,
										void (*initFunc)(void*),
										void (*deinitFunc)(void*));
