#include "hitbox_snapshot.h"
#include "mp_utils.h"

LINK_ENTITY_TO_CLASS(debug_hitbox_snapshot, CHitboxSnapshot)

CHitboxSnapshot* CHitboxSnapshot::Create()
{
	string_t istrClassname = MAKE_STRING("debug_hitbox_snapshot");

	edict_t* edict = CREATE_NAMED_ENTITY(istrClassname);
	entvars_t* pev = VARS(edict);
	CHitboxSnapshot* ent = GetClassPtr<CHitboxSnapshot>(pev);

	ent->Spawn();
	return ent;
}

void CHitboxSnapshot::Spawn()
{
	CBaseAnimating::Spawn();

	pev->classname = MAKE_STRING("debug_hitbox_snapshot");
	pev->effects |= EF_NODRAW;
}

void CHitboxSnapshot::Set(CBasePlayer* other)
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
	pev->v_angle = otherPev->v_angle;
	pev->frame = otherPev->frame;
	pev->colormap = otherPev->colormap;
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
	pev->velocity = Vector();
	pev->avelocity = Vector();
	pev->flags = 0;
	pev->deadflag = otherPev->deadflag;
	pev->effects = (otherPev->effects & ~EF_NODRAW) | EF_NOINTERP;
	pev->sequence = otherPev->sequence;
	pev->gaitsequence = otherPev->gaitsequence;
	pev->animtime = otherPev->animtime;
	pev->framerate = 0.0f;

	UTIL_SetOrigin(pev, otherPev->origin);
	UTIL_SetSize(pev, otherPev->mins, otherPev->maxs);

	for ( uint32_t index = 0; index < ARRAYSIZE(pev->controller); ++index )
	{
		pev->controller[index] = otherPev->controller[index];
	}

	for ( uint32_t index = 0; index < ARRAYSIZE(pev->blending); ++index )
	{
		pev->blending[index] = otherPev->blending[index];
	}
}

void CHitboxSnapshot::SetTrace(const Vector& begin, const Vector& end)
{
	pev->vuser1 = begin;
	pev->vuser2 = end;
	pev->iuser1 = TRUE;
}

void CHitboxSnapshot::ClearTrace()
{
	pev->vuser1 = Vector();
	pev->vuser2 = Vector();
	pev->iuser1 = FALSE;
}
