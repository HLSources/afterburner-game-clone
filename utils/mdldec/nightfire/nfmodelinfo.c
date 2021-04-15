#include <stdlib.h>
#include <string.h>
#include "nightfire/nfmodelinfo.h"
#include "nightfire/allocutils.h"

void NF_ModelInfo_Init(nfmodelinfo_t* modelInfo)
{
	if ( !modelInfo )
	{
		return;
	}

	memset(modelInfo, 0, sizeof(*modelInfo));
}

void NF_ModelInfo_Deinit(nfmodelinfo_t* modelInfo)
{
	if ( !modelInfo )
	{
		return;
	}

	if ( modelInfo->meshes )
	{
		free(modelInfo->meshes);
	}

	memset(modelInfo, 0, sizeof(*modelInfo));
}

void NF_ModelInfo_AllocateMeshes(nfmodelinfo_t* modelInfo, size_t meshCount, qboolean setToZero)
{
	if ( !modelInfo )
	{
		return;
	}

	NF_AllocUtils_AllocateItems((void**)&modelInfo->meshes,
								&modelInfo->meshCount,
								meshCount,
								sizeof(*modelInfo->meshes),
								setToZero);
}

size_t NF_ModelInfo_GetMeshCount(nfmodelinfo_t* modelInfo)
{
	return modelInfo ? modelInfo->meshCount : 0;
}

mdlv14_mesh_t* NF_ModelInfo_GetMesh(nfmodelinfo_t* modelInfo, size_t index)
{
	return (modelInfo && index < modelInfo->meshCount)
		? &modelInfo->meshes[index]
		: NULL;
}
