#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nightfire/enginecommonheaders.h"
#include "studio.h"
#include "crtlib.h"
#include "mdldec.h"
#include "version.h"
#include "qc.h"
#include "nightfire/mdlv14.h"

static void WriteBodyGroupInfo(FILE* fp)
{
	mstudiobodyparts_t* bodypart = NULL;
	mstudiomodel_t* model = NULL;
	char modelname[64];

	fputs("\n// reference mesh(es)\n", fp);

	for ( int bodyGroupIndex = 0; bodyGroupIndex < nightfireHeader->bodyGroups.count; ++bodyGroupIndex )
	{
		bodypart = (mstudiobodyparts_t*) ((byte*)nightfireHeader + nightfireHeader->bodyGroups.offset) + bodyGroupIndex;

		if ( bodypart->nummodels == 1 )
		{
			model = (mstudiomodel_t*)((byte*)nightfireHeader + bodypart->modelindex);

			COM_FileBase(model->name, modelname);

			fprintf(fp, "$body \"%s\" \"%s\"\n\n", bodypart->name, model->name);
			continue;
		}

		fprintf(fp, "$bodygroup \"%s\"\n", bodypart->name);

		fputs("{\n", fp);

		for ( int modelIndex = 0; modelIndex < bodypart->nummodels; ++modelIndex )
		{
			model = (mstudiomodel_t*)((byte*)nightfireHeader + bodypart->modelindex) + modelIndex;

			if( Q_strcmp(model->name, "blank") == 0 )
			{
				fputs("blank\n", fp);
				continue;
			}

			COM_FileBase(model->name, modelname);

			fprintf(fp, "studio \"%s\"\n", modelname);
		}

		fputs("}\n\n" , fp);
	}
}

static void WriteTextureRenderMode(FILE* fp)
{
	mstudiobonecontroller_t* bonecontroller = NULL;
	mstudiobone_t* bone = NULL;
	char motion_types[64];

	if( nightfireHeader->boneControllers.count < 1 )
	{
		return;
	}

	fprintf(fp, "\n// %i bone controller(s)\n", nightfireHeader->boneControllers.count);

	for ( int boneControllerIndex = 0; boneControllerIndex < nightfireHeader->boneControllers.count; ++boneControllerIndex )
	{
		bonecontroller = (mstudiobonecontroller_t*)((byte*)nightfireHeader + nightfireHeader->boneControllers.offset ) + boneControllerIndex;
		bone = (mstudiobone_t*)((byte*)nightfireHeader + nightfireHeader->bones.offset) + bonecontroller->bone;

		GetMotionTypeString(bonecontroller->type & ~STUDIO_RLOOP, motion_types, false);

		fprintf(fp, "$controller %i \"%s\" %s %f %f\n",
				bonecontroller->index, bone->name, motion_types,
				bonecontroller->start, bonecontroller->end );
	}
}

static void WriteSkinFamilyInfo(FILE* fp)
{
	int16_t* skinIndices = NULL;
	int16_t skinIndex = 0;
	mdlv14_texture_t* texture = NULL;

	if( nightfireHeader->skinFamilies  <= 1 )
	{
		return;
	}

	fprintf( fp, "\n// %i skin families\n", nightfireHeader->skinFamilies );
	fputs( "$texturegroup skinfamilies \n{\n", fp );

	skinIndices = (int16_t*)((byte*)nightfireHeader + nightfireHeader->skinsOffset);

	for ( int skinFamilyIndex = 0; skinFamilyIndex < nightfireHeader->skinFamilies; ++skinFamilyIndex )
	{
		fputs( "{", fp );

		for ( int skinRefIndex = 0; skinRefIndex < nightfireHeader->skinReferences; ++skinRefIndex )
		{
			skinIndex = *(skinIndices + (skinFamilyIndex * nightfireHeader->skinReferences) + skinRefIndex);

			for ( int checkSkinFamilyIndex = 0; checkSkinFamilyIndex < nightfireHeader->skinFamilies; ++checkSkinFamilyIndex )
			{
				if ( skinIndex == *(skinIndices + (checkSkinFamilyIndex * nightfireHeader->skinReferences) + skinRefIndex) )
				{
					continue;
				}

				texture = (mdlv14_texture_t*)((byte*)nightfireHeader + nightfireHeader->textures.offset) + skinIndex;

				fprintf( fp, " \"%s\" ", texture->textureName );
				break;
			}
		}

		fputs( "}\n", fp );
	}

	fputs( "}\n", fp );
}

static void WriteAttachmentInfo(FILE* fp)
{
	mstudioattachment_t* attachment = NULL;
	mstudiobone_t* bone = NULL;

	if( nightfireHeader->attachments.count < 1 )
	{
		return;
	}

	fprintf( fp, "\n// %i attachment(s)\n", nightfireHeader->attachments.count );

	for( int attachmentIndex = 0; attachmentIndex < nightfireHeader->attachments.count; ++attachmentIndex )
	{
		attachment = (mstudioattachment_t*)((byte*)nightfireHeader + nightfireHeader->attachments.offset) + attachmentIndex;
		bone = (mstudiobone_t*)((byte*)nightfireHeader + nightfireHeader->bones.count) + attachment->bone;

		fprintf( fp, "$attachment %i \"%s\" %f %f %f\n", attachmentIndex, bone->name, attachment->org[0], attachment->org[1], attachment->org[2] );
	}
}

static void WriteControllerInfo(FILE* fp)
{
	mstudiobonecontroller_t* bonecontroller = NULL;
	mstudiobone_t* bone = NULL;
	char motion_types[64];

	if ( !nightfireHeader->boneControllers.count )
	{
		return;
	}

	fprintf( fp, "\n// %i bone controller(s)\n", nightfireHeader->boneControllers.count );

	for ( int controllerIndex = 0; controllerIndex < nightfireHeader->boneControllers.count; ++controllerIndex )
	{
		bonecontroller = (mstudiobonecontroller_t*)((byte*)nightfireHeader + nightfireHeader->boneControllers.offset) + controllerIndex;
		bone = (mstudiobone_t*)((byte*)nightfireHeader + nightfireHeader->bones.offset) + bonecontroller->bone;

		GetMotionTypeString(bonecontroller->type & ~STUDIO_RLOOP, motion_types, false);

		fprintf( fp, "$controller %i \"%s\" %s %f %f\n",
			bonecontroller->index, bone->name, motion_types,
			bonecontroller->start, bonecontroller->end );
	}
}

static void WriteHitBoxInfo(FILE* fp)
{
	mstudiobbox_t* hitbox = NULL;
	mstudiobone_t* bone = NULL;

	if ( !nightfireHeader->hitboxes.count )
	{
		return;
	}

	fprintf( fp, "\n// %i hit box(es)\n", nightfireHeader->hitboxes.count );

	for ( int hitboxIndex = 0; hitboxIndex < nightfireHeader->hitboxes.count; ++hitboxIndex )
	{
		hitbox = (mstudiobbox_t*)((byte*)nightfireHeader + nightfireHeader->hitboxes.offset ) + hitboxIndex;
		bone = (mstudiobone_t*)((byte*)nightfireHeader + nightfireHeader->bones.offset ) + hitbox->bone;

		fprintf( fp, "$hbox %i \"%s\" %f %f %f %f %f %f\n",
			hitbox->group, bone->name,
			hitbox->bbmin[0], hitbox->bbmin[1], hitbox->bbmin[2],
			hitbox->bbmax[0], hitbox->bbmax[1], hitbox->bbmax[2] );
	}
}

static void WriteSequenceInfo(FILE* fp)
{
	const char* activity = NULL;
	char motion_types[256];
	mstudioevent_t* event = NULL;
	mdlv14_sequence_t* sequence = NULL;

	if ( nightfireHeader->sequenceGroups.count > 1 )
	{
		fputs( "\n$sequencegroupsize 64\n", fp );
	}

	if ( nightfireHeader->sequences.count > 0 )
	{
		fprintf( fp, "\n// %i animation sequence(s)\n", nightfireHeader->sequences.count );
	}

	for ( int sequenceIndex = 0; sequenceIndex < nightfireHeader->sequences.count; ++sequenceIndex )
	{
		sequence = (mdlv14_sequence_t*)((byte*)nightfireHeader + nightfireHeader->sequences.offset) + sequenceIndex;

		fprintf( fp, "$sequence \"%s\" ", sequence->name );

		if ( sequence->blendCount > 1 )
		{
			if ( sequence->blendCount > 2 )
			{
				fputs( "{\n", fp );

				for ( int blendIndex = 0; blendIndex < sequence->blendCount; ++blendIndex )
				{
					fputs( "          ", fp );
					fprintf( fp, "\"%s_blend%i\" ", sequence->name, blendIndex + 1 );
					fputs( "\n", fp );
				}

				fputs( "          ", fp );
			}
			else
			{
				fprintf( fp, "\"%s_blend1\" ", sequence->name );
				fprintf( fp, "\"%s_blend2\" ", sequence->name );
			}

			GetMotionTypeString(sequence->blendType[0], motion_types, false);

			fprintf( fp, "blend %s %.0f %.0f",
				motion_types, sequence->blendStart[0], sequence->blendEnd[0] );
		}
		else
		{
			fprintf( fp, "\"%s\"", sequence->name );
		}

		if ( sequence->motionType )
		{
			GetMotionTypeString(sequence->motionType, motion_types, true);
			fprintf( fp, "%s", motion_types );
		}

		fprintf( fp, " fps %.0f ", sequence->fps );

		if ( sequence->flags & STUDIO_LOOPING )
		{
			fputs( "loop ", fp );
		}

		if ( sequence->activity )
		{
			activity = FindActivityName(sequence->activity);

			if ( activity && Q_strncmp(activity, "ACT_", sizeof("ACT_") - 1) != 0 )
			{
				// This is a custom activity - map to ACT_INVALID
				activity = "ACT_INVALID";
			}

			if( activity )
			{
				fprintf( fp, "%s %i ", activity, sequence->activityWeight );
			}
			else
			{
				printf( "WARNING: Sequence %s has a custom activity flag (ACT_%i %i).\n",
					sequence->name, sequence->activity, sequence->activityWeight );

				fprintf( fp, "ACT_%i %i ", sequence->activity, sequence->activityWeight );
			}
		}

		if ( sequence->nodeEntry && sequence->nodeExit )
		{
			if ( sequence->nodeEntry == sequence->nodeExit )
			{
				fprintf( fp, "node %i ", sequence->nodeEntry );
			}
			else if ( sequence->nodeFlags )
			{
				fprintf( fp, "rtransition %i %i ", sequence->nodeEntry, sequence->nodeExit );
			}
			else
			{
				fprintf( fp, "transition %i %i ", sequence->nodeEntry, sequence->nodeExit );
			}
		}

		if ( sequence->events.count > 2 )
		{
			fputs( "{\n ", fp );

			for ( int eventIndex = 0; eventIndex < sequence->events.count; ++eventIndex )
			{
				if ( sequence->blendCount <= 2 )
				{
					fputs( " ", fp );
				}
				else
				{
					fputs( "          ", fp );
				}

				event = (mstudioevent_t*)((byte*)nightfireHeader + sequence->events.count) + eventIndex;

				fprintf( fp, "{ event %i %i", event->event, event->frame );

				if( event->options[0] != '\0' )
				{
					fprintf( fp, " \"%s\"", event->options );
				}

				fputs( " }\n ", fp );
			}

			fputs( "}", fp );
		}
		else
		{
			for ( int eventIndex = 0; eventIndex < sequence->events.count; ++eventIndex )
			{
				event = (mstudioevent_t*)((byte*)nightfireHeader + sequence->events.offset) + eventIndex;

				fprintf( fp, "{ event %i %i", event->event, event->frame );

				if( event->options[0] != '\0')
				{
					fprintf( fp, " \"%s\"", event->options );
				}

				fputs( " } ", fp );
			}
		}

		fputs( "\n", fp );

		if ( sequence->blendCount > 2 )
		{
			fputs( "}\n", fp );
		}
	}
}

void NF_WriteQCScript(void)
{
	FILE* fp;
	char filename[MAX_SYSPATH];
	int len = 0;

	if ( !nightfireHeader )
	{
		fprintf(stderr, "INTERNAL ERROR: Missing model header pointer\n", modelfile);
		return;
	}

	len = Q_snprintf( filename, MAX_SYSPATH, "%s%s.qc", destdir, modelfile );

	if( len == -1 )
	{
		fprintf( stderr, "ERROR: Destination path is too long. Can't write %s.qc\n", modelfile );
		return;
	}

	fp = fopen( filename, "w" );

	if( !fp )
	{
		fprintf( stderr, "ERROR: Can't write %s\n", filename );
		return;
	}

	fputs( "/*\n", fp );
	fputs( "==============================================================================\n\n", fp );
	fputs( "QC script generated by Half-Life Studio Model Decompiler " APP_VERSION "\n", fp );

	fprintf( fp, "Copyright Flying With Gauss %s (c) \n\n", Q_timestamp( TIME_YEAR_ONLY ) );
	fprintf( fp, "%s.mdl\n\n", modelfile );

	fputs( "Original internal name:\n", fp );

	fprintf( fp, "\"%s\"\n\n", nightfireHeader->name );

	fputs( "==============================================================================\n", fp );
	fputs( "*/\n\n", fp );

	fprintf( fp, "$modelname \"%s.mdl\"\n", modelfile );

	fputs( "$cd \".\\\"\n", fp );
	fputs( "$cdtexture \".\\\"\n", fp );
	fputs( "$scale 1.0\n", fp );
	fputs( "$cliptotextures\n", fp );
	fputs( "\n", fp );

	fprintf( fp, "$flags %u\n", nightfireHeader->flags | STUDIO_NO_EMBEDDED_TEXTURES );

	fputs( "\n", fp );

	fprintf( fp, "$bbox %f %f %f", nightfireHeader->bboxMin[0], nightfireHeader->bboxMin[1], nightfireHeader->bboxMin[2] );
	fprintf( fp, " %f %f %f\n", nightfireHeader->bboxMax[0], nightfireHeader->bboxMax[1], nightfireHeader->bboxMax[2] );
	fprintf( fp, "$cbox %f %f %f", nightfireHeader->cboxMin[0], nightfireHeader->cboxMin[1], nightfireHeader->cboxMin[2] );
	fprintf( fp, " %f %f %f\n", nightfireHeader->cboxMax[0], nightfireHeader->cboxMax[1], nightfireHeader->cboxMax[2] );
	fprintf( fp, "$eyeposition %f %f %f\n", nightfireHeader->eyePosition[0], nightfireHeader->eyePosition[1], nightfireHeader->eyePosition[2] );

	fputs( "\n", fp );

	WriteBodyGroupInfo(fp);
	WriteTextureRenderMode(fp);
	WriteSkinFamilyInfo(fp);
	WriteAttachmentInfo(fp);
	WriteControllerInfo(fp);
	WriteHitBoxInfo(fp);
	WriteSequenceInfo(fp);

	fputs( "\n// End of QC script.\n", fp );
	fclose( fp );

	printf( "QC Script: %s\n", filename );
}
