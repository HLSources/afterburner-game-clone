#pragma once

#include <stddef.h>
#include "nightfire/mdlv44.h"
#include "nightfire/nfmodelinfo.h"

typedef struct
{
	mdlv44_model_t mdlElement;

	size_t modelInfoCount;
	nfmodelinfo_t* modelInfos;
} nfmodel_t;

void NF_Model_Init(nfmodel_t* model);
void NF_Model_Deinit(nfmodel_t* model);

static void NF_Model_InitGeneric(void* model)
{
	NF_Model_Init((nfmodel_t*)model);
}

static void NF_Model_DeinitGeneric(void* model)
{
	NF_Model_Deinit((nfmodel_t*)model);
}

void NF_Model_AllocateModelInfos(nfmodel_t* model, size_t modelInfoCount);
size_t NF_Model_GetModelInfoCount(nfmodel_t* model);
nfmodelinfo_t* NF_Model_GetModelInfo(nfmodel_t* model, size_t index);
