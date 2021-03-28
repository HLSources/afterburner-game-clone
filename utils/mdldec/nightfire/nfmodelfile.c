#include <string.h>
#include <stdlib.h>
#include "nightfire/nfmodelfile.h"
#include "nightfire/allocutils.h"

void NF_ModelFile_Init(nfmodelfile_t* modelFile, const uint8_t* data)
{
	if ( !modelFile )
	{
		return;
	}

	memset(modelFile, 0, sizeof(*modelFile));

	modelFile->data = data;

	NF_SkinCollection_Init(&modelFile->skinCollection, 0, 0);
}

void NF_ModelFile_Deinit(nfmodelfile_t* modelFile)
{
	if ( !modelFile )
	{
		return;
	}

	if ( modelFile->bones )
	{
		free(modelFile->bones);
	}

	if ( modelFile->boneControllers )
	{
		free(modelFile->boneControllers);
	}

	if ( modelFile->hitboxes )
	{
		free(modelFile->hitboxes);
	}

	NF_ModelFile_AllocateSequences(modelFile, 0);

	if ( modelFile->sequenceGroups )
	{
		free(modelFile->sequenceGroups);
	}

	if ( modelFile->textures )
	{
		free(modelFile->textures);
	}

	NF_SkinCollection_Deinit(&modelFile->skinCollection);

	if ( modelFile->bodyGroups )
	{
		free(modelFile->bodyGroups);
	}

	if ( modelFile->attachments )
	{
		free(modelFile->attachments);
	}

	if ( modelFile->soundGroups )
	{
		free(modelFile->soundGroups);
	}

	if ( modelFile->triangleMaps )
	{
		free(modelFile->triangleMaps);
	}

	if ( modelFile->vertices )
	{
		free(modelFile->vertices);
	}

	if ( modelFile->normals )
	{
		free(modelFile->normals);
	}

	if ( modelFile->texCoOrds )
	{
		free(modelFile->texCoOrds);
	}

	if ( modelFile->blendingScales )
	{
		free(modelFile->blendingScales);
	}

	if ( modelFile->blends )
	{
		free(modelFile->blends);
	}

	if ( modelFile->boneFixUps )
	{
		free(modelFile->boneFixUps);
	}

	NF_ModelFile_AllocateModels(modelFile, 0);

	memset(modelFile, 0, sizeof(*modelFile));
}

const uint8_t* NF_ModelFile_GetData(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->data : NULL;
}

const mdlv44_header_t* NF_ModelFile_GetHeader(nfmodelfile_t* modelFile)
{
	return (const mdlv44_header_t*)NF_ModelFile_GetData(modelFile);
}

void NF_ModelFile_AllocateBones(nfmodelfile_t* modelFile, size_t boneCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->bones,
								&modelFile->boneCount,
								boneCount,
								sizeof(*modelFile->bones),
								setToZero);
}

size_t NF_ModelFile_GetBoneCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->boneCount : 0;
}

mstudiobone_t* NF_ModelFile_GetBone(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->boneCount)
		? &modelFile->bones[index]
		: NULL;
}

void NF_ModelFile_AllocateBoneControllers(nfmodelfile_t* modelFile, size_t boneControllerCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->boneControllers,
								&modelFile->boneControllerCount,
								boneControllerCount,
								sizeof(*modelFile->boneControllers),
								setToZero);
}

size_t NF_ModelFile_GetBoneControllerCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->boneControllerCount : 0;
}

mstudiobonecontroller_t* NF_ModelFile_GetBoneController(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->boneControllerCount)
		? &modelFile->boneControllers[index]
		: NULL;
}

void NF_ModelFile_AllocateHitboxes(nfmodelfile_t* modelFile, size_t hitboxCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->hitboxes,
								&modelFile->hitboxCount,
								hitboxCount,
								sizeof(*modelFile->hitboxes),
								setToZero);
}

size_t NF_ModelFile_GetHitboxCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->hitboxCount : 0;
}

mstudiobbox_t* NF_ModelFile_GetHitbox(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->hitboxCount)
		? &modelFile->hitboxes[index]
		: NULL;
}

void NF_ModelFile_AllocateSequences(nfmodelfile_t* modelFile, size_t sequenceCount)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateComplexItems(&modelFile->sequences,
									   &modelFile->sequenceCount,
									   sequenceCount,
									   sizeof(*modelFile->sequences),
									   &NF_Sequence_InitGeneric,
									   &NF_Sequence_DeinitGeneric);
}

size_t NF_ModelFile_GetSequenceCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->sequenceCount : 0;
}

nfsequence_t* NF_ModelFile_GetSequence(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->sequenceCount)
		? &modelFile->sequences[index]
		: NULL;
}

void NF_ModelFile_AllocateSequenceGroups(nfmodelfile_t* modelFile, size_t sequenceGroupCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->sequenceGroups,
								&modelFile->sequenceGroupCount,
								sequenceGroupCount,
								sizeof(*modelFile->sequenceGroups),
								setToZero);
}

size_t NF_ModelFile_GetSequenceGroupCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->sequenceGroupCount : 0;
}

mstudioseqgroup_t* NF_ModelFile_GetSequenceGroup(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->sequenceGroupCount)
		? &modelFile->sequenceGroups[index]
		: NULL;
}

void NF_ModelFile_AllocateTextures(nfmodelfile_t* modelFile, size_t textureCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->textures,
								&modelFile->textureCount,
								textureCount,
								sizeof(*modelFile->textures),
								setToZero);
}

size_t NF_ModelFile_GetTextureCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->textureCount : 0;
}

mdlv44_texture_t* NF_ModelFile_GetTexture(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->textureCount)
		? &modelFile->textures[index]
		: NULL;
}

nfskincollection_t* NF_ModelFile_GetSkinCollection(nfmodelfile_t* modelFile)
{
	return modelFile ? &modelFile->skinCollection : NULL;
}

void NF_ModelFile_AllocateBodyGroups(nfmodelfile_t* modelFile, size_t bodyGroupCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->bodyGroups,
								&modelFile->bodyGroupCount,
								bodyGroupCount,
								sizeof(*modelFile->bodyGroups),
								setToZero);
}

size_t NF_ModelFile_GetBodyGroupCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->bodyGroupCount : 0;
}

mstudiobodyparts_t* NF_ModelFile_GetBodyGroup(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->bodyGroupCount)
		? &modelFile->bodyGroups[index]
		: NULL;
}

void NF_ModelFile_AllocateAttachments(nfmodelfile_t* modelFile, size_t attachmentCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->attachments,
								&modelFile->attachmentCount,
								attachmentCount,
								sizeof(*modelFile->attachments),
								setToZero);
}

size_t NF_ModelFile_GetAttachmentCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->attachmentCount : 0;
}

mstudioattachment_t* NF_ModelFile_GetAttachment(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->attachmentCount)
		? &modelFile->attachments[index]
		: NULL;
}

void NF_ModelFile_AllocateSoundGroups(nfmodelfile_t* modelFile, size_t soundGroupCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->soundGroups,
								&modelFile->soundGroupCount,
								soundGroupCount,
								sizeof(*modelFile->soundGroups),
								setToZero);
}

size_t NF_ModelFile_GetSoundGroupCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->soundGroupCount : 0;
}

nfsoundgroup_t* NF_ModelFile_GetSoundGroup(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->soundGroupCount)
		? &modelFile->soundGroups[index]
		: NULL;
}

void NF_ModelFile_AllocateTriangleMaps(nfmodelfile_t* modelFile, size_t triangleMapCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->triangleMaps,
								&modelFile->triangleMapCount,
								triangleMapCount,
								sizeof(*modelFile->triangleMaps),
								setToZero);
}

size_t NF_ModelFile_GetTriangleMapCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->triangleMapCount : 0;
}

mdlv44_trianglemap_t* NF_ModelFile_GetTriangleMap(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->triangleMapCount)
		? &modelFile->triangleMaps[index]
		: NULL;
}

void NF_ModelFile_AllocateVertices(nfmodelfile_t* modelFile, size_t vertexCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->vertices,
								&modelFile->vertexCount,
								vertexCount,
								sizeof(*modelFile->vertices),
								setToZero);
}

size_t NF_ModelFile_GetVertexCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->vertexCount : 0;
}

mdlv44_pos_scale_t* NF_ModelFile_GetVertex(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->vertexCount)
		? &modelFile->vertices[index]
		: NULL;
}

void NF_ModelFile_AllocateNormals(nfmodelfile_t* modelFile, size_t normalCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->normals,
								&modelFile->normalCount,
								normalCount,
								sizeof(*modelFile->normals),
								setToZero);
}

size_t NF_ModelFile_GetNormalCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->normalCount : 0;
}

mdlv44_pos_scale_t* NF_ModelFile_GetNormal(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->normalCount)
		? &modelFile->normals[index]
		: NULL;
}

void NF_ModelFile_AllocateTexCoOrds(nfmodelfile_t* modelFile, size_t texCoOrdCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->texCoOrds,
								&modelFile->texCoOrdCount,
								texCoOrdCount,
								sizeof(*modelFile->texCoOrds),
								setToZero);
}

size_t NF_ModelFile_GetTexCoOrdCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->texCoOrdCount : 0;
}

mdlv44_texcoord_t* NF_ModelFile_GetTexCoOrd(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->texCoOrdCount)
		? &modelFile->texCoOrds[index]
		: NULL;
}

void NF_ModelFile_AllocateBlendingScales(nfmodelfile_t* modelFile, size_t blendingScaleCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->blendingScales,
								&modelFile->blendingScaleCount,
								blendingScaleCount,
								sizeof(*modelFile->blendingScales),
								setToZero);
}

size_t NF_ModelFile_GetBlendingScaleCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->blendingScaleCount : 0;
}

mdlv44_blendingscale_t* NF_ModelFile_GetBlendingScale(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->blendingScaleCount)
		? &modelFile->blendingScales[index]
		: NULL;
}

void NF_ModelFile_AllocateBlends(nfmodelfile_t* modelFile, size_t blendCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->blends,
								&modelFile->blendCount,
								blendCount,
								sizeof(*modelFile->blends),
								setToZero);
}

size_t NF_ModelFile_GetBlendCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->blendCount : 0;
}

mdlv44_blending_t* NF_ModelFile_GetBlend(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->blendCount)
		? &modelFile->blends[index]
		: NULL;
}

void NF_ModelFile_AllocateBoneFixUps(nfmodelfile_t* modelFile, size_t boneFixUpCount, qboolean setToZero)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateItems(&modelFile->boneFixUps,
								&modelFile->boneFixUpCount,
								boneFixUpCount,
								sizeof(*modelFile->boneFixUps),
								setToZero);
}

size_t NF_ModelFile_GetBoneFixUpCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->boneFixUpCount : 0;
}

void NF_ModelFile_AllocateModels(nfmodelfile_t* modelFile, size_t modelCount)
{
	if ( !modelFile )
	{
		return;
	}

	NF_AllocUtils_AllocateComplexItems(&modelFile->models,
									   &modelFile->modelCount,
									   modelCount,
									   sizeof(*modelFile->models),
									   &NF_Model_InitGeneric,
									   &NF_Model_DeinitGeneric);
}

size_t NF_ModelFile_GetModelCount(nfmodelfile_t* modelFile)
{
	return modelFile ? modelFile->modelCount : 0;
}

nfmodel_t* NF_ModelFile_GetModel(nfmodelfile_t* modelFile, size_t index)
{
	return (modelFile && index < modelFile->modelCount)
		? &modelFile->models[index]
		: NULL;
}
