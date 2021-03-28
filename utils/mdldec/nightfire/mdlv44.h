#pragma once

#include <stdint.h>

#define MDLV44_VERSION 44
static const char MDLV44_IDENT[4] = { 'M', 'D', 'L', 'Z' };

#pragma pack(push, 1)

typedef struct
{
	int32_t count;
	int32_t offset;
} mdlv44_count_offset_t;

typedef struct
{
	char identifier[4];
	int32_t version;

	char name[64];
	int32_t length;

	float eyePosition[3];

	float bboxMin[3];
	float bboxMax[3];

	float cboxMin[3];
	float cboxMax[3];

	uint32_t flags;

	mdlv44_count_offset_t bones;			// mstudiobone_t
	mdlv44_count_offset_t boneControllers;	// mstudiobonecontroller_t
	mdlv44_count_offset_t hitboxes;			// mstudiobbox_t
	mdlv44_count_offset_t sequences;		// mdlv44_sequence_t
	mdlv44_count_offset_t sequenceGroups;	// mstudioseqgroup_t
	mdlv44_count_offset_t textures;			// mdlv44_texture_t
	int32_t textureInfoOffset;
	int32_t skinReferences;
	int32_t skinFamilies;
	int32_t skinsOffset;					// int16_t
	mdlv44_count_offset_t bodyGroups;		// mstudiobodyparts_t
	mdlv44_count_offset_t attachments;		// mstudioattachment_t
	mdlv44_count_offset_t soundGroups;		// mdlv44_soundgroup_t
	int32_t soundsOffset;					// mdlv44_sound_t
	int32_t transitionsCount;
	int32_t transitionFlagsOffset;
	int32_t transitionsOffset;
	uint32_t levelOfDetailFlags;
	int32_t modelCount;
	int32_t vertexCount;
	int32_t triangleCount;
	int32_t triangleMapOffset;				// mdlv44_trianglemap_t
	int32_t verticesOffset;					// mdlv44_pos_scale_t
	int32_t normalsOffset;					// mdlv44_pos_scale_t
	int32_t textureCoOrdsOffset;			// mdlv44_texcoord_t
	int32_t unknown;
	int32_t blendingScalesOffset;			// mdlv44_blendingscale_t
	int32_t blendingOffset;					// mdlv44_blending_t
	int32_t boneFixupOffset;				// mdlv44_bonefixup_t
	int32_t modelsOffset[48];				// mdlv44_model_t
} mdlv44_header_t;

typedef struct
{
	char name[32];
	int32_t fps;
	uint32_t flags;
	int32_t activity;
	int32_t activityWeight;
	mdlv44_count_offset_t events;	// mstudioevent_t
	int32_t frameCount;
	mdlv44_count_offset_t pivots;	// mdlv44_pivot_t
	int32_t motionType;
	int32_t motionBone;
	float linearMovement[3];
	int32_t automovePositionOffset;
	int32_t automoveAngleOffset;
	float bboxMin[3];
	float bboxMax[3];
	int32_t blendCount;
	int32_t animationOffset;
	int32_t blendType[2];
	int32_t blendStart[2];
	int32_t blendEnd[2];
	int32_t blendParent;
	int32_t sequenceGroupOffset;
	int32_t nodeEntry;
	int32_t nodeExit;
	uint32_t nodeFlags;
	int32_t unknown[4];
} mdlv44_sequence_t;

typedef struct
{
	char materialName[64];
	char textureName[64];
	int32_t referenceCount;
	int32_t unknown;
} mdlv44_texture_t;

typedef struct
{
	float position[3];
	int32_t start;
	int32_t end;
} mdlv44_pivot_t;

typedef struct
{
	int32_t levels;
	int32_t distance[4];
} mdlv44_lod_t;

typedef struct
{
	char name[32];
	int32_t offset;
} mdlv44_soundgroup_t;

typedef struct
{
	int32_t count;
	char names[10][64];
} mdlv44_sound_t;

typedef struct
{
	uint16_t vertexIndex;
} mdlv44_trianglemap_t;

typedef struct
{
	float position[3];
	float scale;
} mdlv44_pos_scale_t;

typedef struct
{
	float u;
	float v;
} mdlv44_texcoord_t;

typedef struct
{
	float value[4];
} mdlv44_blendingscale_t;

typedef struct
{
	int8_t value[4];
} mdlv44_blending_t;

typedef struct
{
	float xScale;
	float xSkewY;
	float xSkewZ;
	float xPosition;

	float ySkewX;
	float yScale;
	float ySkewZ;
	float yPosition;

	float zSkewX;
	float zSkewY;
	float zScale;
	float zPosition;
} mdlv44_bonefixup_t;

typedef struct
{
	char name[32];
	int32_t index;
	int32_t modelInfoOffset[24];	// mdlv44_modelinfo_t
} mdlv44_model_t;

typedef struct
{
	int32_t skinReference;
	mdlv44_count_offset_t meshes;	// mdlv44_mesh_t
} mdlv44_modelinfo_t;

typedef struct
{
	int8_t boneMappings[24];
	uint16_t trianglesIndex;
	uint16_t trianglesCount;
	uint16_t vertexIndexFrom;
	uint16_t vertexIndexTo;
} mdlv44_mesh_t;

#pragma pack(pop)
