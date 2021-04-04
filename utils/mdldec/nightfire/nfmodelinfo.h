#pragma once

#include <stddef.h>
#include "nightfire/enginecommonheaders.h"
#include "nightfire/mdlv14.h"

typedef struct
{
	mdlv14_modelinfo_t mdlElement;

	size_t meshCount;
	mdlv14_mesh_t* meshes;
} nfmodelinfo_t;

void NF_ModelInfo_Init(nfmodelinfo_t* modelInfo);
void NF_ModelInfo_Deinit(nfmodelinfo_t* modelInfo);

static void NF_ModelInfo_InitGeneric(void* modelInfo)
{
	NF_ModelInfo_Init((nfmodelinfo_t*)modelInfo);
}

static void NF_ModelInfo_DeinitGeneric(void* modelInfo)
{
	NF_ModelInfo_Deinit((nfmodelinfo_t*)modelInfo);
}


void NF_ModelInfo_AllocateMeshes(nfmodelinfo_t* modelInfo, size_t meshCount, qboolean setToZero);
size_t NF_ModelInfo_GetMeshCount(nfmodelinfo_t* modelInfo);
mdlv14_mesh_t* NF_ModelInfo_GetMesh(nfmodelinfo_t* modelInfo, size_t index);
