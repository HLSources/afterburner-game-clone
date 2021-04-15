#include <string.h>
#include <stdlib.h>
#include "nightfire/nfsequence.h"
#include "nightfire/allocutils.h"

void NF_Sequence_Init(nfsequence_t* sequence)
{
	if ( !sequence )
	{
		return;
	}

	memset(sequence, 0, sizeof(*sequence));

	NF_AnimationCollection_Init(&sequence->animationCollection, 0, 0);
}

void NF_Sequence_Deinit(nfsequence_t* sequence)
{
	if ( !sequence )
	{
		return;
	}

	NF_AnimationCollection_Deinit(&sequence->animationCollection);

	memset(sequence, 0, sizeof(*sequence));
}

void NF_Sequence_AllocateEvents(nfsequence_t* sequence, size_t eventCount, qboolean setToZero)
{
	if ( !sequence )
	{
		return;
	}

	NF_AllocUtils_AllocateItems((void**)&sequence->events,
								&sequence->eventCount,
								eventCount,
								sizeof(*sequence->events),
								setToZero);
}

size_t NF_Sequence_GetEventCount(nfsequence_t* sequence)
{
	return sequence ? sequence->eventCount : 0;
}

mstudioevent_t* NF_Sequence_GetEvent(nfsequence_t* sequence, size_t index)
{
	return (sequence && index < sequence->eventCount)
		? &sequence->events[index]
		: NULL;
}

void NF_Sequence_AllocatePivots(nfsequence_t* sequence, size_t pivotCount, qboolean setToZero)
{
	if ( !sequence )
	{
		return;
	}

	NF_AllocUtils_AllocateItems((void**)&sequence->pivots,
								&sequence->pivotCount,
								pivotCount,
								sizeof(*sequence->pivots),
								setToZero);
}

size_t NF_Sequence_GetPivotCount(nfsequence_t* sequence)
{
	return sequence ? sequence->pivotCount : 0;
}

mdlv14_pivot_t* NF_Sequence_GetPivot(nfsequence_t* sequence, size_t index)
{
	return (sequence && index < sequence->pivotCount)
		? &sequence->pivots[index]
		: NULL;
}

mdlv14_sequence_t* NF_Sequence_GetMDLSequence(nfsequence_t* sequence)
{
	return sequence ? &sequence->mdlSequence : NULL;
}

nfanimationcollection_t* NF_Sequence_GetAnimationCollection(nfsequence_t* sequence)
{
	return sequence ? &sequence->animationCollection : NULL;
}
