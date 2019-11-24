#pragma once

#include "standard_includes.h"

class CHitboxSnapshot : public CBaseAnimating
{
public:
	static CHitboxSnapshot* Create(CBasePlayer* other);

	virtual void Spawn() override;
	void Initialise(CBasePlayer* other);
};
