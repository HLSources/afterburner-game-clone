#pragma once

#include "standard_includes.h"

class CHitboxSnapshot : public CBaseAnimating
{
public:
	static CHitboxSnapshot* Create();

	virtual void Spawn() override;
	void Set(CBasePlayer* other);
	void SetTrace(const Vector& begin, const Vector& end);
	void ClearTrace();
};
