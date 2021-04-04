#pragma once

#include <stddef.h>
#include "nightfire/enginecommonheaders.h"
#include "nightfire/mdlv14.h"
#include "nightfire/nfanimationcollection.h"

typedef struct
{
	mdlv14_sequence_t mdlSequence;

	size_t eventCount;
	mstudioevent_t* events;

	size_t pivotCount;
	mdlv14_pivot_t* pivots;

	nfanimationcollection_t animationCollection;
} nfsequence_t;

void NF_Sequence_Init(nfsequence_t* sequence);
void NF_Sequence_Deinit(nfsequence_t* sequence);

static void NF_Sequence_InitGeneric(void* sequence)
{
	NF_Sequence_Init((nfsequence_t*)sequence);
}

static void NF_Sequence_DeinitGeneric(void* sequence)
{
	NF_Sequence_Deinit((nfsequence_t*)sequence);
}

void NF_Sequence_AllocateEvents(nfsequence_t* sequence, size_t eventCount, qboolean setToZero);
size_t NF_Sequence_GetEventCount(nfsequence_t* sequence);
mstudioevent_t* NF_Sequence_GetEvent(nfsequence_t* sequence, size_t index);

void NF_Sequence_AllocatePivots(nfsequence_t* sequence, size_t pivotCount, qboolean setToZero);
size_t NF_Sequence_GetPivotCount(nfsequence_t* sequence);
mdlv14_pivot_t* NF_Sequence_GetPivot(nfsequence_t* sequence, size_t index);

mdlv14_sequence_t* NF_Sequence_GetMDLSequence(nfsequence_t* sequence);
nfanimationcollection_t* NF_Sequence_GetAnimationCollection(nfsequence_t* sequence);
