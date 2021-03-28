#include <stdlib.h>
#include <string.h>
#include "nightfire/nfmodel.h"

void NF_Model_Init(nfmodel_t* model)
{
	if ( !model )
	{
		return;
	}

	memset(model, 0, sizeof(*model));
}

void NF_Model_Deinit(nfmodel_t* model)
{
	if ( !model )
	{
		return;
	}

	NF_Model_AllocateModelInfos(model, 0);

	memset(model, 0, sizeof(*model));
}

void NF_Model_AllocateModelInfos(nfmodel_t* model, size_t modelInfoCount)
{
	if ( !model )
	{
		return;
	}

	NF_AllocUtils_AllocateComplexItems(&model->modelInfos,
									   &model->modelInfoCount,
									   modelInfoCount,
									   sizeof(*model->modelInfos),
									   &NF_ModelInfo_InitGeneric,
									   &NF_ModelInfo_DeinitGeneric);
}

size_t NF_Model_GetModelInfoCount(nfmodel_t* model)
{
	return model ? model->modelInfoCount : 0;
}

nfmodelinfo_t* NF_Model_GetModelInfo(nfmodel_t* model, size_t index)
{
	return (model && index < model->modelInfoCount)
		? &model->modelInfos[index]
		: NULL;
}
