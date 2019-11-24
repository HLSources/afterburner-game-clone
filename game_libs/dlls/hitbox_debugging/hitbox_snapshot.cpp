#include "hitbox_snapshot.h"
#include "mp_utils.h"

#define LOCAL_CLASSNAME debug_hitbox_snapshot
#define PREPROC_STR(x) #x

LINK_ENTITY_TO_CLASS(LOCAL_CLASSNAME, CHitboxSnapshot)

CHitboxSnapshot* CHitboxSnapshot::Create(CBasePlayer* other)
{
	string_t istrClassname = MAKE_STRING(PREPROC_STR(LOCAL_CLASSNAME));

	edict_t* edict = CREATE_NAMED_ENTITY(istrClassname);
	entvars_t* pev = VARS(edict);
	CHitboxSnapshot* ent = GetClassPtr<CHitboxSnapshot>(pev);

	ent->Spawn();
	ent->Initialise(other);

	return ent;
}

void CHitboxSnapshot::Spawn()
{
	CBaseAnimating::Spawn();

	pev->classname = MAKE_STRING(PREPROC_STR(LOCAL_CLASSNAME));
	pev->effects |= EF_NODRAW;
}

void CHitboxSnapshot::Initialise(CBasePlayer* other)
{
	if ( !other )
	{
		return;
	}

	CUtlString mpModelString = MPUtils::PlayerModelPath(other);

	if ( mpModelString.IsValid() )
	{
		SET_MODEL(ENT(pev), mpModelString.String());
	}

	entvars_t* otherPev = other->pev;

	pev->angles = otherPev->angles;
	pev->frame = otherPev->frame;
	pev->colormap = otherPev->colormap;
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
	pev->velocity = 0;
	pev->avelocity = 0;
	pev->flags = 0;
	pev->deadflag = otherPev->deadflag;
	pev->effects = (otherPev->effects & ~EF_NODRAW) | EF_NOINTERP;
	pev->sequence = otherPev->sequence;
	pev->animtime = otherPev->animtime;
	pev->framerate = 0.0f;

	UTIL_SetOrigin(pev, otherPev->origin);
	UTIL_SetSize(pev, otherPev->mins, otherPev->maxs);

	InitBoneControllers();
	ResetSequenceInfo();
}
