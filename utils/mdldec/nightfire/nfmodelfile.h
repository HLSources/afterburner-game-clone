#pragma once

#include <stddef.h>
#include <stdint.h>
#include "nightfire/enginecommonheaders.h"
#include "nightfire/mdlv44.h"
#include "nightfire/nfsequence.h"
#include "nightfire/nfskincollection.h"
#include "nightfire/nfsoundgroup.h"
#include "nightfire/allocutils.h"
#include "nightfire/nfmodel.h"

typedef struct
{
	// Not owned by this object:
	const uint8_t* data;

	// Owned by this object:
	size_t boneCount;
	mstudiobone_t* bones;

	size_t boneControllerCount;
	mstudiobonecontroller_t* boneControllers;

	size_t hitboxCount;
	mstudiobbox_t* hitboxes;

	size_t sequenceCount;
	nfsequence_t* sequences;

	size_t sequenceGroupCount;
	mstudioseqgroup_t* sequenceGroups;

	size_t textureCount;
	mdlv44_texture_t* textures;

	nfskincollection_t skinCollection;

	size_t bodyGroupCount;
	mstudiobodyparts_t* bodyGroups;

	size_t attachmentCount;
	mstudioattachment_t* attachments;

	size_t soundGroupCount;
	nfsoundgroup_t* soundGroups;

	size_t triangleMapCount;
	mdlv44_trianglemap_t* triangleMaps;

	size_t vertexCount;
	mdlv44_pos_scale_t* vertices;

	size_t normalCount;
	mdlv44_pos_scale_t* normals;

	size_t texCoOrdCount;
	mdlv44_texcoord_t* texCoOrds;

	size_t blendingScaleCount;
	mdlv44_blendingscale_t* blendingScales;

	size_t blendCount;
	mdlv44_blending_t* blends;

	size_t boneFixUpCount;
	mdlv44_bonefixup_t* boneFixUps;

	size_t modelCount;
	nfmodel_t* models;
} nfmodelfile_t;

void NF_ModelFile_Init(nfmodelfile_t* modelFile, const uint8_t* data);
void NF_ModelFile_Deinit(nfmodelfile_t* modelFile);

const uint8_t* NF_ModelFile_GetData(nfmodelfile_t* modelFile);
const mdlv44_header_t* NF_ModelFile_GetHeader(nfmodelfile_t* modelFile);

void NF_ModelFile_AllocateBones(nfmodelfile_t* modelFile, size_t boneCount, qboolean setToZero);
size_t NF_ModelFile_GetBoneCount(nfmodelfile_t* modelFile);
mstudiobone_t* NF_ModelFile_GetBone(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateBoneControllers(nfmodelfile_t* modelFile, size_t boneControllerCount, qboolean setToZero);
size_t NF_ModelFile_GetBoneControllerCount(nfmodelfile_t* modelFile);
mstudiobonecontroller_t* NF_ModelFile_GetBoneController(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateHitboxes(nfmodelfile_t* modelFile, size_t hitboxCount, qboolean setToZero);
size_t NF_ModelFile_GetHitboxCount(nfmodelfile_t* modelFile);
mstudiobbox_t* NF_ModelFile_GetHitbox(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateSequences(nfmodelfile_t* modelFile, size_t sequenceCount);
size_t NF_ModelFile_GetSequenceCount(nfmodelfile_t* modelFile);
nfsequence_t* NF_ModelFile_GetSequence(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateSequenceGroups(nfmodelfile_t* modelFile, size_t sequenceGroupCount, qboolean setToZero);
size_t NF_ModelFile_GetSequenceGroupCount(nfmodelfile_t* modelFile);
mstudioseqgroup_t* NF_ModelFile_GetSequenceGroup(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateTextures(nfmodelfile_t* modelFile, size_t textureCount, qboolean setToZero);
size_t NF_ModelFile_GetTextureCount(nfmodelfile_t* modelFile);
mdlv44_texture_t* NF_ModelFile_GetTexture(nfmodelfile_t* modelFile, size_t index);

nfskincollection_t* NF_ModelFile_GetSkinCollection(nfmodelfile_t* modelFile);

void NF_ModelFile_AllocateBodyGroups(nfmodelfile_t* modelFile, size_t bodyGroupCount, qboolean setToZero);
size_t NF_ModelFile_GetBodyGroupCount(nfmodelfile_t* modelFile);
mstudiobodyparts_t* NF_ModelFile_GetBodyGroup(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateAttachments(nfmodelfile_t* modelFile, size_t attachmentCount, qboolean setToZero);
size_t NF_ModelFile_GetAttachmentCount(nfmodelfile_t* modelFile);
mstudioattachment_t* NF_ModelFile_GetAttachment(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateSoundGroups(nfmodelfile_t* modelFile, size_t soundGroupCount, qboolean setToZero);
size_t NF_ModelFile_GetSoundGroupCount(nfmodelfile_t* modelFile);
nfsoundgroup_t* NF_ModelFile_GetSoundGroup(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateTriangleMaps(nfmodelfile_t* modelFile, size_t triangleMapCount, qboolean setToZero);
size_t NF_ModelFile_GetTriangleMapCount(nfmodelfile_t* modelFile);
mdlv44_trianglemap_t* NF_ModelFile_GetTriangleMap(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateVertices(nfmodelfile_t* modelFile, size_t vertexCount, qboolean setToZero);
size_t NF_ModelFile_GetVertexCount(nfmodelfile_t* modelFile);
mdlv44_pos_scale_t* NF_ModelFile_GetVertex(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateNormals(nfmodelfile_t* modelFile, size_t normalCount, qboolean setToZero);
size_t NF_ModelFile_GetNormalCount(nfmodelfile_t* modelFile);
mdlv44_pos_scale_t* NF_ModelFile_GetNormal(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateTexCoOrds(nfmodelfile_t* modelFile, size_t texCoOrdCount, qboolean setToZero);
size_t NF_ModelFile_GetTexCoOrdCount(nfmodelfile_t* modelFile);
mdlv44_texcoord_t* NF_ModelFile_GetTexCoOrd(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateBlendingScales(nfmodelfile_t* modelFile, size_t blendingScaleCount, qboolean setToZero);
size_t NF_ModelFile_GetBlendingScaleCount(nfmodelfile_t* modelFile);
mdlv44_blendingscale_t* NF_ModelFile_GetBlendingScale(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateBlends(nfmodelfile_t* modelFile, size_t blendCount, qboolean setToZero);
size_t NF_ModelFile_GetBlendCount(nfmodelfile_t* modelFile);
mdlv44_blending_t* NF_ModelFile_GetBlend(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateBoneFixUps(nfmodelfile_t* modelFile, size_t boneFixUpCount, qboolean setToZero);
size_t NF_ModelFile_GetBoneFixUpCount(nfmodelfile_t* modelFile);
mdlv44_bonefixup_t* NF_ModelFile_GetBoneFixUp(nfmodelfile_t* modelFile, size_t index);

void NF_ModelFile_AllocateModels(nfmodelfile_t* modelFile, size_t modelCount);
size_t NF_ModelFile_GetModelCount(nfmodelfile_t* modelFile);
nfmodel_t* NF_ModelFile_GetModel(nfmodelfile_t* modelFile, size_t index);
